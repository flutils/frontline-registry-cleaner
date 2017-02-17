#include "StdAfx.h"
#include "UninstallRScanner.h"
#include <atlpath.h>
#include "CleanEngine.h"

static const TCHAR * ARPCACHE_KEY = 
    _T( "Software\\Microsoft\\Windows\\CurrentVersion\\"
        "App Management\\ARPCache" );

static const TCHAR * UNINSTALL_KEY = 
    _T( "Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall" );

/*************************************************************************
* Function Name:        CUninstallRScanner::CUninstallRScanner()
* Function Purpose:     Constructor  
*************************************************************************/
CUninstallRScanner::CUninstallRScanner( CCleanEngine * pEngine, INotifier * pNotifier /* = NULL */, IScanProfiler * pProfiler /* = NULL */ ) :
	CRegistryScanner( pEngine, pNotifier, pProfiler ),
	CCleanScanner( pEngine, pNotifier, pProfiler ),
	CReportChecker ( pEngine ),
    m_pfnAlgoSelector( NULL ),
    m_dwIndex( 0 ),
    m_hScanningKey( NULL )
{
}

/*************************************************************************
* Function Name:        CUninstallRScanner::~CUninstallRScanner()
* Function Purpose:     Destructor
*************************************************************************/
CUninstallRScanner::~CUninstallRScanner()
{
}

/*************************************************************************
* Function Name:        CUninstallRScanner::ReinitAlgorithm()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CUninstallRScanner::ReinitAlgorithm()
{
    m_pfnAlgoSelector = &CUninstallRScanner::algoStep1;
    m_dwIndex = 0;
}

/*************************************************************************
* Function Name:        CTypeLibRScanner::algorithm()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CUninstallRScanner::algorithm()
{
    ( this->*m_pfnAlgoSelector )();
}

/*************************************************************************
* Function Name:    CUninstallRScanner::postop()
*
* Function Purpose: Called by CRunnable after run() function completed. 
*                   Perform correctly thread completion (close opened 
*                   handles etc.)
*
* Arguments:        None
* Return value:     None
*                       
*************************************************************************/
void CUninstallRScanner::postop( void )
{
    CRegistryScanner::postop();

    if( m_hScanningKey )
    {
        RegCloseKey( m_hScanningKey );
        m_hScanningKey = NULL;
    }
}

/*************************************************************************
* Function Name:        CUninstallRScanner::algoStep1()
* Function Purpose:     Open 'HKLM\Software' key for scanning.
*
* Arguments:            None
* Return value:         None
*************************************************************************/
void CUninstallRScanner::algoStep1()
{
    LONG lResult = RegOpenKeyEx(
                            HKEY_LOCAL_MACHINE,
                            _T("Software"),
                            0,
                            KEY_READ|KEY_WRITE| DELETE,
                            &m_hScanningKey);

    if( ERROR_SUCCESS != lResult )
    {
        // Can't open required key, stop scanning
        //

        StopJustMe();
        return;
    }

    m_dwIndex = 0;
    m_pfnAlgoSelector = &CUninstallRScanner::algoStep2;
}

/*************************************************************************
* Function Name:        CUninstallRScanner::algoStep2()
* Function Purpose:     Scan 'HKLM\Software' key for empty keys. Open
*                       'HKCU\Software' key for further scanning in step3.
* Arguments:            None
* Return value:         None
*************************************************************************/
void CUninstallRScanner::algoStep2()
{
    _ASSERT( m_hScanningKey );

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

        ce_tstring strScanObject( _T("HKEY_LOCAL_MACHINE\\Software\\") );
        strScanObject += szKeyName;

        // Notify user about current scanning object.
        //
        //if( m_pNotifier )
        //{
        //    m_pNotifier->CallOnScan( ScannerId(), strScanObject.GetString() );
        //}

        if( ERROR_SUCCESS == RegOpenKeyEx(
                                        m_hScanningKey,
                                        szKeyName,
                                        0,
                                        KEY_READ|KEY_WRITE| DELETE,
                                        &hOpenKey ) )
        {
            if( IsKeyEmpty( hOpenKey ) )
			{
				if( m_pNotifier )
				{
					m_pNotifier->CallOnScan( EMPTY_KEYS_SCANNER_ID, strScanObject.GetString() );
				}
                RecursivelyAddKey( hOpenKey, strScanObject, SEVERITY_MEDIUM, 0, true );
                RegCloseKey( hOpenKey );
                return;
            }else
				if( m_pNotifier )
				{
					m_pNotifier->CallOnScan( ScannerId(), strScanObject.GetString() );
				}
        }

        return;
    }

    // Enumerate completed or any error occurred, switch to next step.
    //
    RegCloseKey( m_hScanningKey );
    m_hScanningKey = NULL;

    lResult = RegOpenKeyEx(
                        HKEY_CURRENT_USER,
                        _T("Software"),
                        0,
                        KEY_READ|KEY_WRITE| DELETE,
                        &m_hScanningKey );

    if( ERROR_SUCCESS != lResult )
    {
        // Can't open required key, stop scanning
        //

        StopJustMe();
        return;
    }

    m_dwIndex = 0;
    m_pfnAlgoSelector = &CUninstallRScanner::algoStep3;
}

/*************************************************************************
* Function Name:        CUninstallRScanner::algoStep3()
* Function Purpose:     Scan 'HKCU\Software' key for empty keys. Open
*                       'HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\
*                       CurrentVersion\App Management\ARPCache' key for 
*                       further scanning in step4.
*
* Arguments:            None
* Return value:         None
*************************************************************************/
void CUninstallRScanner::algoStep3()
{
    _ASSERT( m_hScanningKey );

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

        ce_tstring strScanObject( _T("HKEY_CURRENT_USER\\Software\\") );
        strScanObject += szKeyName;

        // Notify user about current scanning object.
        //
        if( m_pNotifier )
        {
		//	m_pNotifier->CallOnScan( ScannerId(), strScanObject.GetString() );
        }

        if( ERROR_SUCCESS == RegOpenKeyEx(
                                    m_hScanningKey,
                                    szKeyName,
                                    0,
                                    KEY_READ|KEY_WRITE| DELETE,
                                    &hOpenKey ) )
        {
            if( IsKeyEmpty( hOpenKey ) )
            {
				if( m_pNotifier )
				{
					m_pNotifier->CallOnScan( EMPTY_KEYS_SCANNER_ID, strScanObject.GetString() );
				}
                RecursivelyAddKey( hOpenKey, strScanObject, SEVERITY_MEDIUM, 0, true );
                RegCloseKey( hOpenKey );
                return;
            }else
				if( m_pNotifier )
				{
					m_pNotifier->CallOnScan( ScannerId(), strScanObject.GetString() );
				}
        }

        return;
    }

    // Enumerate completed or any error occurred, switch to next step.
    //
    //RegCloseKey( m_hScanningKey );
    //m_hScanningKey = NULL;
    //
    //lResult = RegOpenKeyEx(
    //                    HKEY_LOCAL_MACHINE,
    //                    ARPCACHE_KEY,
    //                    0,
    //                    KEY_READ,
    //                    &m_hScanningKey );

    //if( ERROR_SUCCESS != lResult )
    //{
    //    // Can't open required key, stop scanning
    //    //

    //    StopJustMe();
    //    return;
    //}

	RegCloseKey( m_hScanningKey );
	m_hScanningKey = NULL;

	lResult = RegOpenKeyEx(
		HKEY_LOCAL_MACHINE,
		UNINSTALL_KEY,
		0,
		KEY_READ|KEY_WRITE| DELETE,
		&m_hScanningKey );

	if( ERROR_SUCCESS != lResult )
	{
		// Can't open required key, stop scanning
		//

		StopJustMe();
		return;
	}

    m_dwIndex = 0;
    m_pfnAlgoSelector = &CUninstallRScanner::algoStep4;
}

/*************************************************************************
* Function Name:        CUninstallRScanner::algoStep5()
* Function Purpose:     Scan 'HKLM\Software\Microsoft\Windows\
*                       CurrentVersion\App Management\ARPCache' for 
*                       inexistent corresponding keys under real  
*                       uninstall key. After that open 'HKLM\Software\
*                       Microsoft\Windows\CurrentVersion\Uninstall' key 
*                       for scanning.
*
* Arguments:            None
* Return value:         None
*************************************************************************/
void CUninstallRScanner::algoStep5()
{
    _ASSERT( m_hScanningKey );
    
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
        ce_tstring strScanObject( _T("HKEY_LOCAL_MACHINE\\") );
        strScanObject += ARPCACHE_KEY;
        strScanObject += _T("\\");
        strScanObject += szKeyName;

        // Notify user about current scanning object.
        //
        if( m_pNotifier )
        {
            m_pNotifier->CallOnScan( ScannerId(), strScanObject.GetString() );
        }
        
        ce_tstring strRealKey( UNINSTALL_KEY );
        strRealKey += _T("\\");
        strRealKey += szKeyName;

		ce_tstring strFullPath( _T( "HKEY_LOCAL_MACHINE\\" ) );
		strFullPath += strRealKey;

		if( !IsKeyExist( HKEY_LOCAL_MACHINE, const_cast<const LPTSTR>( strRealKey.GetString() ) ) || UninstallIsInReport( strFullPath ) )
        {
            HKEY hOpenKey;
            if( ERROR_SUCCESS == RegOpenKeyEx(
                                    m_hScanningKey,
                                    szKeyName,
                                    0,
                                    KEY_READ|KEY_WRITE| DELETE,
                                    &hOpenKey ) )
            {
                RecursivelyAddKey( hOpenKey, strScanObject );
                RegCloseKey( hOpenKey );
                return;
            }
        }

        return;
    }

    // Enumerate completed or any error occurred, switch to next step.
    //
    //RegCloseKey( m_hScanningKey );
    //m_hScanningKey = NULL;

    //lResult = RegOpenKeyEx(
    //                    HKEY_LOCAL_MACHINE,
    //                    UNINSTALL_KEY,
    //                    0,
    //                    KEY_READ,
    //                    &m_hScanningKey );

    //if( ERROR_SUCCESS != lResult )
    //{
    //    // Can't open required key, stop scanning
    //    //

    //    StopJustMe();
    //    return;
    //}

    //m_dwIndex = 0;
    //m_pfnAlgoSelector = &CUninstallRScanner::algoStep5;

	StopJustMe();
}

/*************************************************************************
* Function Name:        CUninstallRScanner::algoStep4()
* Function Purpose:     Scan 'HKLM\Software\Microsoft\Windows\
*                       CurrentVersion\Uninstall' key. Search keys that 
*                       have “UninstallString” value with inexistent 
*                       executable.
*
* Arguments:            None
* Return value:         None
*************************************************************************/
void CUninstallRScanner::algoStep4()
{
    _ASSERT( m_hScanningKey );
    
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
        ce_tstring strScanObject( _T("HKEY_LOCAL_MACHINE\\") );
        strScanObject += UNINSTALL_KEY;
        strScanObject += _T("\\");
        strScanObject += szKeyName;

        // Notify user about current scanning object.
        //
        if( m_pNotifier )
        {
            //m_pNotifier->CallOnScan( ScannerId(), strScanObject.GetString() );
        }

        HKEY hOpenKey;
        if( ERROR_SUCCESS == RegOpenKeyEx(
                                    m_hScanningKey,
                                    szKeyName,
                                    0,
                                    KEY_READ|KEY_WRITE| DELETE,
                                    &hOpenKey ) )
        {
            if( IsKeyEmpty( hOpenKey ) )
			{
				if( m_pNotifier )
				{
					m_pNotifier->CallOnScan( EMPTY_KEYS_SCANNER_ID, strScanObject.GetString() );
				}
                RecursivelyAddKey( hOpenKey, strScanObject, SEVERITY_MEDIUM, 0, true );
                RegCloseKey( hOpenKey );
                return;
            }else
				if( m_pNotifier )
				{
					m_pNotifier->CallOnScan( ScannerId(), strScanObject.GetString() );
				}

            DWORD dwDataSize = 0;
            DWORD dwType = REG_SZ;
            countable< TCHAR > spDataBuffer;

            lResult = RegReadValue(
                                hOpenKey,
                                _T("UninstallString"),
                                &dwType,
                                spDataBuffer,
                                dwDataSize );
            
            if( ERROR_SUCCESS == lResult )
            {
                if( !CheckPaths( spDataBuffer, FALSE ) || PathIsInReport( spDataBuffer.get() ))
                {
                    RecursivelyAddKey( hOpenKey, strScanObject );
                }
            }

            RegCloseKey( hOpenKey );
        }

        return;
    }

    // Enumerate completed or any error occurred, end scanning.
    //
	RegCloseKey( m_hScanningKey );
	m_hScanningKey = NULL;

	lResult = RegOpenKeyEx(
		HKEY_LOCAL_MACHINE,
		ARPCACHE_KEY,
		0,
		KEY_READ|KEY_WRITE| DELETE,
		&m_hScanningKey );

	if( ERROR_SUCCESS != lResult )
	{
		// Can't open required key, stop scanning
		//

		StopJustMe();
		return;
	}

	m_dwIndex = 0;
	m_pfnAlgoSelector = &CUninstallRScanner::algoStep5;
    //StopJustMe();
}

/*
*
*
*
*
*/

