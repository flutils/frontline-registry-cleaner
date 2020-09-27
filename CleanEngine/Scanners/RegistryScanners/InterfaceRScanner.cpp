#include "StdAfx.h"
#include "InterfaceRScanner.h"
#include "CleanEngine.h"
#include "helpers.h"

static const TCHAR * INTERFACE_KEY = _T( "Interface" );

/*************************************************************************
* Function Name:        CInterfaceRScanner::CInterfaceRScanner
* Function Purpose:     Constructor  
*************************************************************************/
CInterfaceRScanner::CInterfaceRScanner( CCleanEngine * pEngine, INotifier * pNotifier /* = NULL */, IScanProfiler * pProfiler /* = NULL */ ) :
	CRegistryScanner( pEngine, pNotifier, pProfiler ),
	CCleanScanner( pEngine, pNotifier, pProfiler ),
	CReportChecker( pEngine ),
    m_pfnAlgoSelector( NULL ),
    m_hScanningKey( NULL ),
    m_dwIndex( 0 )
{
}

/*************************************************************************
* Function Name:        CInterfaceRScanner::~CInterfaceRScanner
* Function Purpose:     Destructor
*************************************************************************/
CInterfaceRScanner::~CInterfaceRScanner()
{
}

/*************************************************************************
* Function Name:          CInterfaceRScanner::ReinitAlgorithm()
* Function Purpose:       
* Function Description: 
*************************************************************************/
void CInterfaceRScanner::ReinitAlgorithm()
{
    m_pfnAlgoSelector = &CInterfaceRScanner::algoStep1;
    m_dwIndex = 0;
}

/*************************************************************************
* Function Name:          CInterfaceRScanner::algorithm()
* Function Purpose:       
* Function Description: 
*************************************************************************/
void CInterfaceRScanner::algorithm()
{
    ( this->*m_pfnAlgoSelector )();
}

/*************************************************************************
* Function Name:    CInterfaceRScanner::postop()
*
* Function Purpose: Called by CRunnable after run() function completed. 
*                   Perform correctly thread completion (close opened 
*                   handles etc.)
*
* Arguments:        None
* Return value:     None
*                           
*************************************************************************/
void CInterfaceRScanner::postop( void )
{
    CRegistryScanner::postop();

    if( m_hScanningKey )
    {
        RegCloseKey( m_hScanningKey );
        m_hScanningKey = NULL;
    }
}

/*************************************************************************
* Function Name:          CInterfaceRScanner::algoStep1()
*
* Function Purpose:       Open HKCR\Interface registry key for scanning.
* Function Description:   
*
* Arguments:              None
* Return value:           None
*
*************************************************************************/
void CInterfaceRScanner::algoStep1()
{
    LONG lResult = RegOpenKeyEx(
                            HKEY_CLASSES_ROOT,
                            INTERFACE_KEY,
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
    m_pfnAlgoSelector = &CInterfaceRScanner::algoStep2;
}

/*************************************************************************
* Function Name:        CInterfaceRScanner::algoStep2()
*
* Function Purpose:     Enumerate items under m_hScanningKey
*                       (HKCR\Interface) key, previously opened by
*                       algoStep1() function.
*
* Arguments:            None
* Return value:         None
*                           
*************************************************************************/
void CInterfaceRScanner::algoStep2()
{
    TCHAR szKeyName[ MAX_KEY_SIZE ];
    DWORD dwSize = sizeof( szKeyName ) / sizeof( TCHAR );

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
        // Notify user about current scanning object
        //
        if( m_pNotifier )
        {
            ce_tstring strScanObject( _T("HKEY_CLASSES_ROOT\\") );
            strScanObject += INTERFACE_KEY;

            m_pNotifier->CallOnScan( ScannerId(), strScanObject.GetString(), szKeyName );
        }

        CheckInterface( szKeyName );
		LOG_TEXT(_T("-----------------------------------------------------------------"))
        return;
    }

	
	// Enumerate completed or any error occurred, stop scanning.
    //


    StopJustMe();
}

/*************************************************************************
* Function Name:        CInterfaceRScanner::CheckInterface()
*
* Function Purpose:     Searches COM class interfaces that referenced 
*                       inexistent GUIDs of TypeLib or ProxyStubClsid32.
*
* Arguments:            szKeyName - Zero-terminating string containing
*                       interface GUID that is need to be checked.
* Return value:         None                           
*************************************************************************/
void CInterfaceRScanner::CheckInterface( const TCHAR * szKeyName )
{
    HKEY hOpenKey = NULL;
    LONG lResult;

	LOG_TEXT3( _T("CheckInterface for "),szKeyName,_T( " started!"))
    
    lResult = RegOpenKeyEx(
                        m_hScanningKey, 
                        szKeyName, 
                        0, 
                        KEY_READ, 
                        &hOpenKey );

    if( ERROR_SUCCESS != lResult )
    {
		LOG_TEXT3( _T("RegOpenKeyEx for"),szKeyName,_T( "was failed!"))
		return;
    }

    if( IsKeyEmpty( hOpenKey ) )
    {
        ce_tstring strKeyName( _T("HKEY_CLASSES_ROOT\\") );
        strKeyName += INTERFACE_KEY;
        strKeyName += _T("\\");
        strKeyName += szKeyName;

		LOG_TEXT3(_T("Key "),strKeyName,_T( " is empty! This is TREAT!"))
	
        RecursivelyAddKey( hOpenKey, strKeyName );
        RegCloseKey( hOpenKey );
        return;
    }

	BOOL bHasBadProxyStub(FALSE), bHasBadTypeLib(FALSE);

    // Check 'ProxyStubClsid32' key.
    //
	LOG_TEXT2(_T("Check ProxyStubClsid32 of key "), szKeyName)

    HKEY hCheckedKey;
    lResult = RegOpenKeyEx( hOpenKey,
                            _T("ProxyStubClsid32"),
                            0,
                            KEY_READ,
                            &hCheckedKey );

    if( ERROR_SUCCESS == lResult )
    {
        countable< TCHAR > spDataBuffer;
        DWORD dwType        = REG_SZ;
        DWORD dwDataSize    = 0;

        lResult = RegReadValue(
                            hCheckedKey,
                            NULL,
                            &dwType,
                            spDataBuffer,
                            dwDataSize );

        if( ERROR_SUCCESS == lResult )
        {
            ce_tstring strKey = _T("CLSID\\");
            strKey += spDataBuffer.get();

			LOG_TEXT3(_T("Checking "),strKey,_T( " from ProxyStubClsid32 key"))

			if( !IsKeyExist( HKEY_CLASSES_ROOT, const_cast<const LPTSTR>( strKey.GetString() ) ) || CLSIDIsInReport( strKey ) ) 
            {
                ce_tstring strKeyName( _T("HKEY_CLASSES_ROOT\\") );
                strKeyName += INTERFACE_KEY;
                strKeyName += _T( "\\" );
                strKeyName += szKeyName;
				strKeyName +=_T("\\ProxyStubClsid32");

				LOG_TEXT3(_T("IsKeyExists for "),strKey,_T( " was failed! This is TREAT!"))

			    RecursivelyAddKey( hCheckedKey, strKeyName );
                RegCloseKey( hCheckedKey );
                
            }
			else
				LOG_TEXT3(_T("CLSID "),strKey,_T( " is present"))

        }

        RegCloseKey( hCheckedKey );
    }

	else
	LOG_TEXT2(_T("ProxyStubClsid32 key is not present for key "), szKeyName)

    // Check 'TypeLib' key.
    //
	LOG_TEXT2(_T("Check TypeLib of key "), szKeyName)

    lResult = RegOpenKeyEx( hOpenKey,
                            _T( "TypeLib" ),
                            0,
                            KEY_READ,
                            &hCheckedKey );

    if( ERROR_SUCCESS == lResult )
    {
        countable< TCHAR > spDataBuffer;
        DWORD dwType        = REG_SZ;
        DWORD dwDataSize    = 0;

        lResult = RegReadValue(
                            hCheckedKey,
                            NULL,
                            &dwType,
                            spDataBuffer,
                            dwDataSize );

        if( ERROR_SUCCESS == lResult )
        {
            ce_tstring strKey = _T("TypeLib\\");
            strKey += spDataBuffer.get();

			LOG_TEXT3(_T("Checking "),strKey,_T( " from interface key"))

			if( !IsKeyExist( HKEY_CLASSES_ROOT, const_cast<const LPTSTR>( strKey.GetString() ) ) || TypeLibIsInReport( strKey ) )
            {
                ce_tstring strKeyName( _T("HKEY_CLASSES_ROOT\\") );
                strKeyName += INTERFACE_KEY;
				strKeyName +=_T("\\");
				strKeyName +=szKeyName;
                strKeyName += _T("\\TypeLib");
                				
				LOG_TEXT3(_T("IsKeyExist for "),strKey,_T( " was failed! This is TREAT!"))
			
                RecursivelyAddKey( hCheckedKey, strKeyName );
				RegCloseKey( hCheckedKey );
            }
			else
				LOG_TEXT3(_T("TypeLib "),strKey,_T( " is present"))
        }
		
			

        
    }

	else
	LOG_TEXT2(_T("TypeLib key is not present for key "), szKeyName)

	//Check again for empty
	if( IsKeyEmpty( hOpenKey ) )
    {
        ce_tstring strKeyName( _T("HKEY_CLASSES_ROOT\\") );
        strKeyName += INTERFACE_KEY;
        strKeyName += _T("\\");
        strKeyName += szKeyName;

		LOG_TEXT3(_T("Key "),strKeyName,_T( " is empty! This is TREAT!"))
	
        RecursivelyAddKey( hOpenKey, strKeyName, SEVERITY_MEDIUM,20 );
        RegCloseKey( hOpenKey );
        return;
    }

    if( hOpenKey )
    {
        RegCloseKey( hOpenKey );
    }

	LOG_TEXT3(_T("CheckInterface for "),szKeyName,_T( " is OK!"))

}

BOOL CInterfaceRScanner::CheckPotentialThreat( HKEY hKey )
{
    _ASSERT( hKey );

    TCHAR szKeyName[ 16384 ];
    DWORD dwSize = sizeof( szKeyName ) / sizeof( TCHAR );
	DWORD dwIndex = 0;

	DWORD dwItemsCount = 0;

	LONG lResult;

    do
	{
		lResult = RegEnumKeyEx(
                            hKey,
                            dwIndex++,
                            szKeyName,
                            &dwSize,
                            NULL,
                            NULL,
                            NULL,
                            NULL );

		if( ERROR_SUCCESS == lResult )
			++dwItemsCount;

	} while( ERROR_SUCCESS == lResult );

	if( dwItemsCount >= 2 )
		return FALSE;

	return TRUE;
}
