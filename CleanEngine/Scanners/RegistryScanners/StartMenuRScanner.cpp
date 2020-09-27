#include "StdAfx.h"
#include <lmcons.h>
#include "StartMenuRScanner.h"

static const TCHAR * STARTMENU_KEY = _T( "Software\\Microsoft\\Windows\\"
    "CurrentVersion\\Explorer\\MenuOrder\\Start Menu\\Programs" );

static const TCHAR * STARTMENU2_KEY = _T( "Software\\Microsoft\\Windows\\"
    "CurrentVersion\\Explorer\\MenuOrder\\Start Menu2\\Programs" );

/*************************************************************************
* Function Name:        CStartMenuRScanner::CStartMenuRScanner()
* Function Purpose:     Constructor
*************************************************************************/
CStartMenuRScanner::CStartMenuRScanner( CCleanEngine * pEngine, INotifier * pNotifier /* = NULL */, IScanProfiler * pProfiler /* = NULL */ ) :
	CRegistryScanner( pEngine, pNotifier, pProfiler ),
	CCleanScanner( pEngine, pNotifier, pProfiler ),
    m_pfnAlgoSelector( NULL ),
    m_dwIndex( 0 ),
    m_hScanningKey( NULL ),
    m_strAllUserFolder(_T( "" )),
    m_strCurrentUserFolder(_T( "" )),
    m_strStartMenu(_T( "" ))
{
}

/*************************************************************************
* Function Name:        CStartMenuRScanner::~CStartMenuRScanner()
* Function Purpose:     Destructor
*************************************************************************/
CStartMenuRScanner::~CStartMenuRScanner()
{
}

/*************************************************************************
* Function Name:        CStartMenuRScanner::ReinitAlgorithm()
* Function Purpose:       
* Function Description: 
*************************************************************************/
void CStartMenuRScanner::ReinitAlgorithm()
{
    m_pfnAlgoSelector = &CStartMenuRScanner::algoStep1;
    m_dwIndex = 0;
}

/*************************************************************************
* Function Name:        CStartMenuRScanner::algorithm()
* Function Purpose:       
* Function Description: 
*************************************************************************/
void CStartMenuRScanner::algorithm()
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
void CStartMenuRScanner::postop( void )
{
    CRegistryScanner::postop();

    if( m_hScanningKey )
    {
        RegCloseKey( m_hScanningKey );
        m_hScanningKey = NULL;
    }
}

/*************************************************************************
* Function Name:        CStartMenuRScanner::algoStep1()
*
* Function Purpose:     Open "HKEY_CURRENT_USER\Software\Microsoft\
*                       Windows\CurrentVersion\Explorer\MenuOrder\
*                       Start Menu\Programs" key for scanning.  
*
* Arguments:            None
* Return value:         None
*
*************************************************************************/
void CStartMenuRScanner::algoStep1()
{
    LONG lResult = RegOpenKeyEx(
                            HKEY_CURRENT_USER,
                            STARTMENU_KEY,
                            0,
                            KEY_READ|KEY_WRITE| DELETE,
                            &m_hScanningKey );

    if( ERROR_SUCCESS != lResult )
    {
        lResult = RegOpenKeyEx(
                            HKEY_CURRENT_USER,
                            STARTMENU2_KEY,
                            0,
                            KEY_READ|KEY_WRITE| DELETE,
                            &m_hScanningKey );

        if( ERROR_SUCCESS != lResult )
        {
            StopJustMe();
            return;
        }

        m_strStartMenu = STARTMENU2_KEY;
    }
    else
    {
        m_strStartMenu = STARTMENU_KEY;
    }

    TCHAR szSysDir[ MAX_PATH ];
    TCHAR szUserName[ UNLEN + 1 ];
        
    DWORD dwBufSize = sizeof( szUserName );

    if( !GetUserName( szUserName, &dwBufSize ))
    {
        StopJustMe();
        return;
    }

    dwBufSize = sizeof( szSysDir );

    if( !GetSystemDirectory( szSysDir, dwBufSize ))
    {
        StopJustMe();
        return;
    }

    szSysDir[ 3 ] = _T('\0');
        
    m_strCurrentUserFolder = szSysDir;
    m_strCurrentUserFolder += _T( "Documents and Settings\\" );
    m_strCurrentUserFolder += szUserName;
    m_strCurrentUserFolder += _T( "\\Start Menu\\Programs\\" );

    m_strAllUserFolder = szSysDir;
    m_strAllUserFolder += _T( "Documents and Settings\\All Users\\Start Menu\\Programs\\" );

    m_dwIndex = 0;
    m_pfnAlgoSelector = &CStartMenuRScanner::algoStep2;
}

/*************************************************************************
* Function Name:        CStartMenuRScanner::algoStep2()
*
* Function Purpose:     Verify that each key under "HKEY_CURRENT_USER\
*                       Software\Microsoft\Windows\CurrentVersion\
*                       Explorer\MenuOrder\Start Menu\Programs" has a 
*                       corresponding folder in “C:\Documents and Settings\
*                       All Users\Start Menu\Programs\” and 
*                       “C:\Documents and Settings\<Current User>\
*                       Start Menu\Programs\”.
*
* Arguments:            None
* Return value:         None
*
*************************************************************************/
void CStartMenuRScanner::algoStep2()
{
    _ASSERT( m_hScanningKey );

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
        ce_tstring strScanObject( _T( "HKEY_CURRENT_USER\\"));
        strScanObject += m_strStartMenu;
        strScanObject += _T( "\\" );
        strScanObject += szKeyName;

        // Notify user about current scanning object
        //
        if( m_pNotifier )
        {
            m_pNotifier->CallOnScan( ScannerId(), strScanObject.GetString() );
        }

		int uAufSize = m_strAllUserFolder.GetLength();
        int uCufSize = m_strCurrentUserFolder.GetLength();
        
        m_strAllUserFolder      += szKeyName;
        m_strCurrentUserFolder  += szKeyName;
        
        //if( !CheckPathExistence( m_strAllUserFolder.GetString() ) &&
        //    !CheckPathExistence( m_strCurrentUserFolder.GetString() ))

        if( !PathFileExists( m_strAllUserFolder.GetString() ) &&
            !PathFileExists( m_strCurrentUserFolder.GetString() ))
        {
            HKEY hOpenKey; 

            if( ERROR_SUCCESS == RegOpenKeyEx( 
                                        m_hScanningKey, 
                                        szKeyName, 
                                        0, 
                                        KEY_READ|KEY_WRITE| DELETE, 
                                        &hOpenKey ))
            {
                RecursivelyAddKey( hOpenKey, strScanObject );
                RegCloseKey( hOpenKey );
            }
        }

        m_strAllUserFolder.GetBufferSetLength( uAufSize );
        m_strCurrentUserFolder.GetBufferSetLength( uCufSize );

        return;
    }

    // Enumerate completed or any error occurred, stop scanning.
    //
    StopJustMe();
}
