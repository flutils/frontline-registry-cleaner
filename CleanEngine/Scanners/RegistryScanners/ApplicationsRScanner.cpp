#include "StdAfx.h"
#include "ApplicationsRScanner.h"
#include "CleanEngine.h"

static const TCHAR * APPLICATIONS_KEY = _T( "Applications" );

/*************************************************************************
* Function Name:        CApplicationRScanner::CApplicationRScanner()
* Function Purpose:     Constructor  
*************************************************************************/
CApplicationsRScanner::CApplicationsRScanner( CCleanEngine * pEngine, INotifier * pNotifier /* = NULL */, IScanProfiler * pProfiler /* = NULL */ ) :
	CRegistryScanner( pEngine, pNotifier, pProfiler ),
	CCleanScanner( pEngine, pNotifier, pProfiler ),
	m_pfnAlgoSelector( NULL ),
	m_dwIndex( 0 ),
	m_hScanningKey( NULL )
{
}

/*************************************************************************
* Function Name:        CApplicationRScanner::CApplicationRScanner()
* Function Purpose:     Destructor
*************************************************************************/
CApplicationsRScanner::~CApplicationsRScanner()
{
}

/*************************************************************************
* Function Name:          CComClassRScanner::ReinitAlgorithm()
* Function Purpose:       
* Function Description: 
*************************************************************************/
void CApplicationsRScanner::ReinitAlgorithm()
{
    m_pfnAlgoSelector = &CApplicationsRScanner::algoStep1;
    m_dwIndex = 0;
}

/*************************************************************************
* Function Name:          CComClassRScanner::algorithm()
* Function Purpose:       
* Function Description: 
*************************************************************************/
void CApplicationsRScanner::algorithm()
{
    ( this->*m_pfnAlgoSelector )();
}

/*************************************************************************
* Function Name:    CApplicationsRScanner::postop()
*
* Function Purpose: Called by CRunnable after run() function completed. 
*                   Perform correctly thread completion (close opened 
*                   handles etc.)
*
* Arguments:        None
* Return value:     None
*                           
*************************************************************************/
void CApplicationsRScanner::postop( void )
{
    CRegistryScanner::postop();

    if( m_hScanningKey )
    {
        RegCloseKey( m_hScanningKey );
        m_hScanningKey = NULL;
    }
}

/*************************************************************************
* Function Name:          CApplicationsRScanner::algoStep1()
*
* Function Purpose:       Open HKCR\Applications registry key for scanning.
* Function Description:   
*
* Arguments:              None
* Return value:           None
*
*************************************************************************/
void CApplicationsRScanner::algoStep1()
{
    LONG lResult = RegOpenKeyEx(
                            HKEY_CLASSES_ROOT,
                            APPLICATIONS_KEY,
                            0,
                            KEY_READ|KEY_WRITE| DELETE,
                            &m_hScanningKey);

    if( ERROR_SUCCESS != lResult )
    {
        //
        // Can't open required key, stop scanning
        //

        StopJustMe();
        return;
    }

    m_dwIndex = 0;
    m_pfnAlgoSelector = &CApplicationsRScanner::algoStep2;
}

/*************************************************************************
* Function Name:        CApplicationRScanner::algoStep2()
*
* Function Purpose:     Enumerate items under m_hScanningKey 
*                       (HKCR\Applications) key, previously opened by
*                       algoStep1() function.
*
* Arguments:            None
* Return value:         None
*                           
*************************************************************************/
void CApplicationsRScanner::algoStep2()
{
    TCHAR  szKeyName[ MAX_KEY_SIZE ];
    DWORD  dwSize = sizeof( szKeyName ) / sizeof( TCHAR );

    LONG lResult = RegEnumKeyEx(
                            m_hScanningKey, 
                            m_dwIndex++ , 
                            szKeyName, 
                            &dwSize, 
                            NULL, 
                            NULL,
                            NULL,
                            NULL );

    if( ERROR_SUCCESS == lResult )
    {
        // Notify user about current scan object
        //
        if( m_pNotifier )
        {
            ce_tstring strScanObject(_T( "HKEY_CLASSES_ROOT\\" ));
            strScanObject += APPLICATIONS_KEY;

			m_pNotifier->CallOnScan( ScannerId(), strScanObject.GetString(), szKeyName );
        }

        CheckApplication( szKeyName );
		LOG_TEXT(_T("------------------------------------------------------------------"))
        return;
    }

    // Enumerate completed or any error occurred, stop scanning.
    //
    StopJustMe();
}

/*************************************************************************
* Function Name:    CApplicationRScanner::CheckApplicataion()
* Function Purpose: Searches entries that have incorrect shell "command".
*
* Arguments:        szKeyName - Zero-terminating string containing
*                   key name that is need to be checked.
* Return value:     None 
*************************************************************************/
void CApplicationsRScanner::CheckApplication( const TCHAR * szKeyName )
{
    switch( CheckKeyForCommand( m_hScanningKey, szKeyName ))
    {
    
    case NO_PATH:
    case CORRECT_PATH:
        {
            // Checked key doesn't contain any paths or all paths 
            // are valid.
			LOG_TEXT3(_T("Checked key "),szKeyName,_T(" doesn't contain any paths or all paths are OK"))
            
        }
        break;

    case INVALID_PATH:
        {
            // Checked key contains at least one incorrect path.
            //
            HKEY hOpenKey;
            ce_tstring strKeyName(_T( "HKEY_CLASSES_ROOT\\Applications\\" ));
            strKeyName += szKeyName;

            if( ERROR_SUCCESS == RegOpenKeyEx(  m_hScanningKey,
                                                szKeyName,
                                                0,
                                                KEY_READ|KEY_WRITE| DELETE,
                                                &hOpenKey ) )
            {
				LOG_TEXT3(_T("Checked key "),szKeyName,_T(" contains at least one incorrect path. This is TREAT"))
                RecursivelyAddKey( hOpenKey, strKeyName );
                RegCloseKey( hOpenKey );
            }
        }
        break;

    // We should never reach this code!
    //
    default:
        EXCEPTION( "Unhandled return value!" );
        break;
    }
}

/*************************************************************************
* Function Name:    CApplicationRScanner::CheckKeyForCommand()
* Function Purpose: Searches entries that have incorrect shell "command".
*
* Arguments:    hKey - Handle to a currently open key.
*                   
*               szKeyName - Pointer to a null-terminating string
*               containing SubKey name that is need to be checked.
*                   
*               nDepth - Recurse level.
*                   
* Return value: 
*               NO_PATH:        SubKey doesn't contain any path.
*               CORRECT_PATH:   SubKey contain path and path exist.
*               INVALID_PATH:   SubKey contain path and path is invalid.
*************************************************************************/
CHECKPATH_RESULT CApplicationsRScanner::CheckKeyForCommand(
        HKEY            hKey, 
        const TCHAR *   szKeyName,
        unsigned        nDepth )
{
    _ASSERT( hKey );

    if( nDepth > MaxRecurseLevel )
    {
        return NO_PATH;
    }

    HKEY hOpenKey;
    LONG lResult = RegOpenKeyEx(
                            hKey,
                            szKeyName,
                            0,
                            KEY_READ|KEY_WRITE| DELETE,
                            &hOpenKey );

    if( ERROR_SUCCESS != lResult )
    {
        return NO_PATH;
    }

    TCHAR szSubKey[ MAX_KEY_SIZE ];
    DWORD dwIndex = 0;
    DWORD dwSize;

    while( 1 )
    {
        dwSize = sizeof( szSubKey ) / sizeof( TCHAR );
        lResult = RegEnumKeyEx( hOpenKey,
                                dwIndex++,
                                szSubKey,
                                &dwSize,
                                NULL,
                                NULL,
                                NULL,
                                NULL );

        if( ERROR_SUCCESS != lResult )
        {
            break;
        }

        if( _tcsicmp( szSubKey, _T( "command" )) == 0 )
        {
            HKEY hOpenKey2;

            if( ERROR_SUCCESS == RegOpenKeyEx(
                                            hOpenKey, 
                                            szSubKey, 
                                            0, 
                                            KEY_READ|KEY_WRITE| DELETE, 
                                            &hOpenKey2 ))
            {
                DWORD dwType        = REG_SZ;
                DWORD dwDataSize    = 0;
                countable< TCHAR > spDataBuffer;

                if( ERROR_SUCCESS == RegReadValue( 
                                                hOpenKey2, 
                                                NULL, 
                                                &dwType, 
                                                spDataBuffer, 
                                                dwDataSize ))
                {
                    if( CheckPaths( spDataBuffer, FALSE ))
                    {
                        RegCloseKey( hOpenKey );
                        RegCloseKey( hOpenKey2);

                        return CORRECT_PATH;
                    }
                    else
                    {
                        RegCloseKey( hOpenKey );
                        RegCloseKey( hOpenKey2);

                        return INVALID_PATH;
                    }
                }

                RegCloseKey( hOpenKey2 );
            }
        }

        CHECKPATH_RESULT chkResult;
        chkResult = CheckKeyForCommand(
                                    hOpenKey, 
                                    szSubKey, 
                                    nDepth + 1 );

        if( INVALID_PATH == chkResult )
        {
            RegCloseKey( hOpenKey );
            return chkResult;
        }
    }

    RegCloseKey( hOpenKey );
    return NO_PATH;
}
