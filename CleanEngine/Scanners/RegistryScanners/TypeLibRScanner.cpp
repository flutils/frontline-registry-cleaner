#include "StdAfx.h"
#include "TypeLibRScanner.h"
#include <atlpath.h>
#include "CleanEngine.h"


/*************************************************************************
* Function Name:        CTypeLibRScanner::CTypeLibRScanner()
* Function Purpose:     Constructor  
*************************************************************************/
CTypeLibRScanner::CTypeLibRScanner( CCleanEngine * pEngine, INotifier * pNotifier /* = NULL */, IScanProfiler * pProfiler /* = NULL */ ) :
	CRegistryScanner( pEngine, pNotifier, pProfiler ),
	CCleanScanner( pEngine, pNotifier, pProfiler ),
	CReportChecker( pEngine ),
    m_pfnAlgoSelector( NULL ),
    m_dwIndex( 0 ),
    m_hScanningKey( NULL )
{
}

/*************************************************************************
* Function Name:        CTypeLibRScanner::~CTypeLibRScanner()
* Function Purpose:     Destructor
*************************************************************************/
CTypeLibRScanner::~CTypeLibRScanner()
{
}

/*************************************************************************
* Function Name:        CTypeLibRScanner::ReinitAlgorithm()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CTypeLibRScanner::ReinitAlgorithm()
{
    m_pfnAlgoSelector = &CTypeLibRScanner::algoStep1;
    m_dwIndex = 0;
}

/*************************************************************************
* Function Name:        CTypeLibRScanner::algorithm()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CTypeLibRScanner::algorithm()
{
    ( this->*m_pfnAlgoSelector )();
}

/*************************************************************************
* Function Name:    CTypeLibRScanner::postop()
*
* Function Purpose: Called by CRunnable after run() function completed. 
*                   Perform correctly thread completion (close opened 
*                   handles etc.)
*
* Arguments:        None
* Return value:     None
*                       
*************************************************************************/
void CTypeLibRScanner::postop( void )
{
    CRegistryScanner::postop();

    if( m_hScanningKey )
    {
        RegCloseKey( m_hScanningKey );
        m_hScanningKey = NULL;
    }
}

/*************************************************************************
* Function Name:        CTypeLibRScanner::algoStep1()
* Function Purpose:     Open HKCR\TypeLib key for scanning.
* Arguments:            None
* Return value:         None
*************************************************************************/
void CTypeLibRScanner::algoStep1()
{
    LONG lResult = RegOpenKeyEx(
                            HKEY_CLASSES_ROOT,
                            _T("TypeLib"),
                            0,
                            KEY_READ,
                            &m_hScanningKey );

    if( ERROR_SUCCESS != lResult )
    {
        // Can't open required key, stop scanning
        //

        StopJustMe();
        return;
    }

    m_dwIndex = 0;
    m_pfnAlgoSelector = &CTypeLibRScanner::algoStep2;
}

/*************************************************************************
* Function Name:        CTypeLibRScanner::algoStep2()
* Function Purpose:     Search inexistent files in HKCR\TypeLib\xxx\win32
* Arguments:            None
* Return value:         None
*************************************************************************/
void CTypeLibRScanner::algoStep2()
{
    TCHAR  szKeyName[ MAX_KEY_SIZE ];
    DWORD  dwSize = sizeof( szKeyName ) / sizeof( TCHAR );

    LONG lResult = RegEnumKeyEx(
                            m_hScanningKey,
                            m_dwIndex++,
                            szKeyName,
                            &dwSize,
                            NULL,
                            NULL,
                            NULL,
                            NULL );

    if( ERROR_SUCCESS == lResult )
    {
        HKEY hOpenKey;

        ce_tstring strScanObject( _T("HKEY_CLASSES_ROOT\\TypeLib\\") );
        strScanObject += szKeyName;

        // Notify user about current scanning object.
        //
        if( m_pNotifier )
        {
            m_pNotifier->CallOnScan( ScannerId(), strScanObject.GetString() );
        }

        if( ERROR_SUCCESS == RegOpenKeyEx(
                                        m_hScanningKey,
                                        szKeyName,
                                        0,
                                        KEY_READ,
                                        &hOpenKey ) )
        {
            if( IsKeyEmpty( hOpenKey ) )
            {
				LOG_TEXT3(_T("IsKeyEmpty for "),szKeyName,_T( " was failed! This is TREAT!"))

				RecursivelyAddKey( hOpenKey, strScanObject );
                RegCloseKey( hOpenKey );
                return;
            }
        
            CheckTypeLib( hOpenKey, ( TCHAR* )strScanObject.GetString() );
            RegCloseKey( hOpenKey );
			LOG_TEXT(_T("-----------------------------------------------------------------"))
        }

        return;
    }

	
	// Enumerate completed or any error occurred, stop scanning.
    //
    StopJustMe();
}

/*************************************************************************
* Function Name:    CTypeLibRScanner::CheckTypeLib()
* Function Purpose: Check TypeLib file existing for HKCR\TypeLib\<TypeLib>
*
* Arguments:        hKey - Handle to a current open key.
*
*                   szKeyName - Pointer to a null-terminated string 
*                   containing the full name of the opened key.
*                   
* Return value:     None
*                           
*************************************************************************/
void CTypeLibRScanner::CheckTypeLib( 
    HKEY            hKey, 
    const TCHAR *   szKeyName )
{
    _ASSERT( hKey );
    
    TCHAR szSubKey[ MAX_KEY_SIZE ];
    DWORD dwIndex = 0;
    LONG lResult;

    bool bGoodKeyFound = false;
    std::vector< countable< ce_tstring > >  vBadKeys;

	LOG_TEXT3( _T("Checking TypeLib for "),szKeyName,_T(""))

    while( 1 ) 
    {
        DWORD dwNameSize = sizeof( szSubKey ) / sizeof( TCHAR );
        
        lResult = RegEnumKeyEx( hKey,
                                dwIndex++,
                                szSubKey,
                                &dwNameSize,
                                NULL,
                                NULL,
                                NULL,
                                NULL );

        if( ERROR_SUCCESS != lResult )
        {
			break;
        }

        // Assume every key contains only one path :-/
        //
        switch( CheckKeyForPath( hKey, szSubKey ) )
        {

        case NO_PATH:
            {
				// SubKey doesn't contain path.
                //
				LOG_TEXT(_T("SubKey doesn't contain path"))
            }
            break;

        case CORRECT_PATH:
            {
                // SubKey contain path and this path exist.
                //
				LOG_TEXT3(_T("SubKey "),szSubKey,_T(" contain path and this path exist"))
                bGoodKeyFound = true;
            }
            break;

        case INVALID_PATH:
            {
                // SubKey contain path and path is invalid.
                //
				
				LOG_TEXT3(_T("CheckKeyForPath for "),szSubKey,_T(" contain path and path is invalid"))

			    countable< ce_tstring > spBadKey( new ce_tstring( szSubKey ));
                if( !spBadKey.get() )
                {
                    return;
                }

                vBadKeys.push_back( spBadKey );
            }
            break;

        // We should never reach this code!
        //
        default:
            EXCEPTION( "Unhandled return value!" );
            break;
        }
    }

    if( vBadKeys.size() )
    {
        if( !bGoodKeyFound )
        {
            // There are no any good SubKeys found, add whole key.
            //
			LOG_TEXT3(_T("No good sub keys found for "),szKeyName, _T(" this is TREAT!"))
		
            ce_tstring strKeyName( szKeyName );
            RecursivelyAddKey( hKey, strKeyName );
        }
        else
        {
            // Key contains SubKeys with valid paths, add bad keys only.
            //
            for( unsigned i = 0; i < vBadKeys.size(); i++ )
            {
                HKEY hOpenKey;
                countable< ce_tstring > spSubKey = vBadKeys[ i ];
                ce_tstring strKeyName( szKeyName );
                strKeyName += _T( "\\" );
                strKeyName += spSubKey.get()->GetString();

                if( ERROR_SUCCESS == RegOpenKeyEx( 
                                            hKey, 
											spSubKey.get()->GetString(), 
                                            0, 
                                            KEY_READ, 
                                            &hOpenKey ))
                {

				    LOG_TEXT3(_T("Adding subkey "),*spSubKey.get(),_T( " because of invalid path. This is TREAT!"))
				
					RecursivelyAddKey( hOpenKey, strKeyName );
                    RegCloseKey( hOpenKey );
                }
            }
        }
    }
}

/*************************************************************************
* Function Name:    CTypeLibRScanner::CheckKeyForPath()
* Function Purpose: 
*
* Arguments:        hKey - Handle to a current open key.
*                   
*                   szKeyName - Pointer to a null-terminating string
*                   containing SubKey name that is need to be checked.
*                   
*                   nDepth - Recurse level.
*                   
* Return value:     NO_PATH:        SubKey doesn't contain any path.
*                   CORRECT_PATH:   SubKey contain path and path exist.
*                   INVALID_PATH:   SubKey contain path and path invalid.
*
*************************************************************************/
CHECKPATH_RESULT
    CTypeLibRScanner::CheckKeyForPath(
        HKEY        hKey,
        TCHAR *     szKeyName,
        unsigned    nDepth )
{
    _ASSERT( hKey );

    if( nDepth > MaxRecurseLevel )
    {
        return NO_PATH;
    }
	LOG_TEXT3(_T("CheckKeyForPath for "),szKeyName,_T( " started"))
    
    HKEY hOpenKey;
    LONG lResult = RegOpenKeyEx(
                                hKey,
                                szKeyName,
                                0,
                                KEY_READ,
                                &hOpenKey );

    if( ERROR_SUCCESS != lResult )
    {
		LOG_TEXT3(_T("RefOpenKeyEx for "),szKeyName,_T( " was failed!"))
		return NO_PATH;
    }

    TCHAR szSubKey[ MAX_KEY_SIZE ];
    DWORD dwIndex = 0;
    DWORD dwSize;

    while( 1 )
    {
        dwSize = sizeof( szSubKey ) / sizeof( TCHAR );
        lResult = RegEnumKeyEx(
                            hOpenKey,
                            dwIndex++,
                            szSubKey,
                            &dwSize,
                            NULL,
                            NULL,
                            NULL,
                            NULL );

        if( ERROR_SUCCESS != lResult )
        {
			LOG_TEXT3(_T("Key "),szKeyName, _T( " does not contain path!"))
			break;
        }

        if( _tcsicmp( szSubKey, _T("win32") ) == 0 )
        {    
            HKEY hOpenKey2;

            if( ERROR_SUCCESS == RegOpenKeyEx(  hOpenKey,
                                                szSubKey,
                                                0,
                                                KEY_READ,
                                                &hOpenKey2 ) )
            {
                DWORD dwDataSize    = 0;
                DWORD dwType        = REG_SZ; 
                countable< TCHAR > spDataBuffer;

                if( ERROR_SUCCESS == RegReadValue(
                                                hOpenKey2,
                                                NULL,
                                                &dwType,
                                                spDataBuffer,
                                                dwDataSize ) )
                {
                    if( CheckPaths( spDataBuffer ) )
                    {
                        if (PathIsInReport( spDataBuffer.get() ))
                        {
							LOG_TEXT3( _T("Key "),spDataBuffer.get(),_T( " is fake path!"))

							RegCloseKey( hOpenKey );
							RegCloseKey( hOpenKey2);
							return INVALID_PATH;
                        }
						else
						{
							LOG_TEXT3( _T("Key "),spDataBuffer.get(),_T( " is good path!"))
							RegCloseKey( hOpenKey );
							RegCloseKey( hOpenKey2);
							return CORRECT_PATH;
						}
                    }
                    else
                    {
						const UINT DATA_BUFFER_SIZE = 32768;
						TCHAR infoBuf[ DATA_BUFFER_SIZE ];
						DWORD bufCharCount = ExpandEnvironmentStrings( spDataBuffer.get(), infoBuf, DATA_BUFFER_SIZE ); 
						CString str = infoBuf;

						LOG_TEXT3(_T("CheckPath for "),spDataBuffer.get(),_T( " was failed!"))
					
						if ( str.Find( _T(":\\") )== 1 )
                        {
							LOG_TEXT3( _T("Key "),spDataBuffer.get(),_T( " is bad path!"))
                        	RegCloseKey( hOpenKey );
							RegCloseKey( hOpenKey2);
							return INVALID_PATH;
						}
						else
						{
							LOG_TEXT3( _T("Key "),spDataBuffer.get(),_T( " is good path!"))
							RegCloseKey( hOpenKey );
							RegCloseKey( hOpenKey2);
							return CORRECT_PATH;
						}
                    }
                }

                RegCloseKey( hOpenKey2 );
            }
        }

        CHECKPATH_RESULT chkResult;
        chkResult = CheckKeyForPath(
                                hOpenKey,
                                szSubKey,
                                nDepth + 1 );

        if( ( INVALID_PATH == chkResult ) ||
            ( CORRECT_PATH == chkResult ) )
        {
            RegCloseKey( hOpenKey );
            return chkResult;
        }
    }

    RegCloseKey( hOpenKey );

	LOG_TEXT3(_T("CheckKeyForPath for "),szKeyName,_T( " finished!"))

    return NO_PATH;
}

