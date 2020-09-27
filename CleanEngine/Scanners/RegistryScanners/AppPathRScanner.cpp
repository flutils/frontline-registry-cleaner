#include "StdAfx.h"
#include "AppPathRScanner.h"
#include "CleanEngine.h"

static const TCHAR * APPPATHS_KEY = 
    _T( "Software\\Microsoft\\Windows\\CurrentVersion\\App Paths" );

/*************************************************************************
* Function Name:        CAppPathRScanner::CAppPathRScanner()
* Function Purpose:     Constructor
*************************************************************************/
CAppPathRScanner::CAppPathRScanner( CCleanEngine * pEngine, INotifier * pNotifier /* = NULL */, IScanProfiler * pProfiler /* = NULL */ ) :
	CRegistryScanner( pEngine, pNotifier, pProfiler ),
	CCleanScanner( pEngine, pNotifier, pProfiler ),
    m_pfnAlgoSelector( NULL ),
    m_dwIndex( 0 ),
    m_hScanningKey( NULL )
{
	//AddExcluded( ce_tstring(_T( "cmmgr32.exe" )));
}

/*************************************************************************
* Function Name:        CAppPathRScanner::~CAppPathRScanner()
* Function Purpose:     Destructor
*************************************************************************/
CAppPathRScanner::~CAppPathRScanner()
{
}

/*************************************************************************
* Function Name:        CAppPathRScanner::ReinitAlgorithm()
* Function Purpose:
* Arguments:            None
* Return value:         None
*************************************************************************/
void CAppPathRScanner::ReinitAlgorithm()
{
    m_pfnAlgoSelector = &CAppPathRScanner::algoStep1;
}

/*************************************************************************
* Function Name:        CAppPathRScanner::algorithm()
* Function Purpose:
* Arguments:            None
* Return value:         None
*************************************************************************/
void CAppPathRScanner::algorithm()
{
    ( this->*m_pfnAlgoSelector )();
}

/*************************************************************************
* Function Name:    CAppPathRScanner::postop()
*
* Function Purpose: Called by CRunnable after run() function completed. 
*                   Perform correctly thread completion (close opened 
*                   handles etc.)
*
* Arguments:        None
* Return value:     None
*
*************************************************************************/
void CAppPathRScanner::postop( void )
{
    CRegistryScanner::postop();

    if( m_hScanningKey )
    {
        RegCloseKey( m_hScanningKey );
        m_hScanningKey = NULL;
    }
}

/*************************************************************************
* Function Name:    CAppPathRScanner::algoStep1()
*
* Function Purpose: Open 'HKLM\Software\Microsoft\Windows\CurrentVersion\
*                   App Paths' registry key for scanning.
*
* Arguments:        None
* Return value:     None
*
*************************************************************************/
void CAppPathRScanner::algoStep1()
{
    LONG lResult = RegOpenKeyEx(
                            HKEY_LOCAL_MACHINE,
                            APPPATHS_KEY,
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
    m_pfnAlgoSelector = &CAppPathRScanner::algoStep2;
}

/*************************************************************************
* Function Name:        CAppPathRScanner::algoStep2()
*
* Function Purpose:     Enumerate items under m_hScanningKey key,
*                       previously opened by algoStep1() function and 
*                       check them.
*
* Arguments:            None
* Return value:         None
*
*************************************************************************/
void CAppPathRScanner::algoStep2()
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
        // Notify user about current scan object
        //
        if( m_pNotifier )
        {
            ce_tstring strScanObject(_T( "HKEY_LOCAL_MACHINE\\" ));
            strScanObject += APPPATHS_KEY;

			m_pNotifier->CallOnScan( ScannerId(), strScanObject.GetString(), szKeyName );
        }

        CheckAppPath( szKeyName );
		LOG_TEXT(_T("---------------------------------------------------------------------------------"))
        return;
    }

    //
    // Enumerate completed or any error occurred, stop scanning.
    //
    StopJustMe();
}

/*************************************************************************
* Function Name:        CAppPathRScanner::CheckAppPath()
*
* Function Purpose:     Search applications’ paths in 'HKLM\Software\
*                       Microsoft\Windows\CurrentVersion\App Paths' 
*                       that does not exist on hard drive.
*
* Arguments:            szKeyName - Zero-terminating string containing
*                                   key name is to be checked.
* Return value:         None
*************************************************************************/
void CAppPathRScanner::CheckAppPath( const TCHAR * szKeyName )
{
    _ASSERT( m_hScanningKey );

    HKEY hOpenKey;
    LONG lResult = RegOpenKeyEx(
                            m_hScanningKey, 
                            szKeyName, 
                            0, 
                            KEY_READ | KEY_WRITE| DELETE , 
                            &hOpenKey );
    
    if( ERROR_SUCCESS != lResult )
    {
        return;
    }

    DWORD dwType;
    DWORD dwDataSize = 0;
    countable< TCHAR > spDataBuffer;

    lResult = RegReadValue( hOpenKey, 
                            NULL, 
                            &dwType, 
                            spDataBuffer, 
                            dwDataSize );

    if( ERROR_SUCCESS != lResult )
    {
        RegCloseKey( hOpenKey );
        return;
    }

    if( !CheckPaths( spDataBuffer, FALSE ))
    {
        ce_tstring strKeyName(_T( "HKEY_LOCAL_MACHINE\\" ));
        strKeyName += APPPATHS_KEY;
        strKeyName += _T( "\\" );
        strKeyName += szKeyName;

		LOG_TEXT3(strKeyName, _T(" contains TREAT path "),spDataBuffer.get())

        RecursivelyAddKey( hOpenKey, strKeyName );
    }
	else
		LOG_TEXT3(szKeyName, _T(" contains OK path "),spDataBuffer.get())


    RegCloseKey( hOpenKey );
}
