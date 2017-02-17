#include "StdAfx.h"
#include "MruRScanner.h"

/*************************************************************************
* Function Name:        CMruRScanner::CMruRScanner()
* Function Purpose:     Constructor
*************************************************************************/
CMruRScanner::CMruRScanner( CCleanEngine * pEngine, INotifier * pNotifier /* = NULL */, IScanProfiler * pProfiler /* = NULL */ ) :
	CRegistryScanner( pEngine, pNotifier, pProfiler ),
	CCleanScanner( pEngine, pNotifier, pProfiler ),
    m_pfnAlgoSelector( NULL ),
    m_dwIndex( 0 ),
    m_hScanningKey( NULL ),
    m_vMruList( 0 ),
    m_strScanObject( _T("") )
{
    // Create list of default MRU keys.
    //
    /*AddMruKey( 
        HKEY_CURRENT_USER, _T(
        "Software\\Microsoft\\Windows\\" 
        "CurrentVersion\\Explorer\\FileExts" ));*/
    
    AddMruKey(
        HKEY_CURRENT_USER, _T(
        "Software\\Microsoft\\Windows\\"
        "CurrentVersion\\Explorer\\StreamMRU" ));

    AddMruKey(
        HKEY_CURRENT_USER, _T(
        "Software\\Microsoft\\Windows\\"
        "CurrentVersion\\Explorer\\FindComputerMRU" ));

    AddMruKey(
        HKEY_LOCAL_MACHINE, _T( "Software\\Microsoft\\"
        "Windows\\CurrentVersion\\Explorer\\FindComputerMRU" ));

    AddMruKey(
        HKEY_CURRENT_USER, _T(
        "Software\\Microsoft\\Windows\\CurrentVersion\\"
        "Explorer\\Doc Find Spec MRU" ));

    AddMruKey(
        HKEY_LOCAL_MACHINE, _T(
        "Software\\Microsoft\\Windows\\CurrentVersion\\"
        "Explorer\\Doc Find Spec MRU" ));

    AddMruKey(
        HKEY_CURRENT_USER, _T(
        "Software\\Microsoft\\Internet Explorer\\Explorer Bars\\"
        "{C4EE31F3-4768-11D2-BE5C-00A0C9A83DA1}\\FilesNamedMRU" ));

    AddMruKey(
        HKEY_LOCAL_MACHINE, _T(
        "Software\\Microsoft\\Internet Explorer\\Explorer Bars\\"
        "{C4EE31F3-4768-11D2-BE5C-00A0C9A83DA1}\\FilesNamedMRU" ));
    
    AddMruKey(
        HKEY_CURRENT_USER, _T(
        "Software\\Microsoft\\Internet Explorer\\Explorer Bars\\"
        "{C4EE31F3-4768-11D2-BE5C-00A0C9A83DA1}\\ContainingTextMRU" ));

    AddMruKey(
        HKEY_LOCAL_MACHINE, _T(
        "Software\\Microsoft\\Internet Explorer\\Explorer Bars\\"
        "{C4EE31F3-4768-11D2-BE5C-00A0C9A83DA1}\\ContainingTextMRU" ));

    AddMruKey(
        HKEY_CURRENT_USER, _T(
        "Software\\Microsoft\\Windows\\CurrentVersion\\"
        "Explorer\\Map Network Drive MRU" ));

    AddMruKey(
        HKEY_LOCAL_MACHINE, _T(
        "Software\\Microsoft\\Windows\\CurrentVersion\\"
        "Explorer\\Map Network Drive MRU" ));

    AddMruKey(
        HKEY_CURRENT_USER, _T(
        "Software\\Microsoft\\Windows\\CurrentVersion\\"
        "Explorer\\ComDlg32\\LastVisitedMRU" ));

    AddMruKey(
        HKEY_LOCAL_MACHINE, _T(
        "Software\\Microsoft\\Windows\\CurrentVersion\\"
        "Explorer\\ComDlg32\\LastVisitedMRU" ));

    AddMruKey(
        HKEY_CURRENT_USER, _T(
        "Software\\Microsoft\\Windows\\CurrentVersion\\"
        "Explorer\\ComDlg32\\OpenSaveMRU" ));

    AddMruKey(
        HKEY_LOCAL_MACHINE, _T(
        "Software\\Microsoft\\Windows\\CurrentVersion\\"
        "Explorer\\ComDlg32\\OpenSaveMRU" ));
    
    AddMruKey(
        HKEY_CURRENT_USER, _T(
        "Software\\Microsoft\\Windows\\CurrentVersion\\"
        "Explorer\\RunMRU" ));

    AddMruKey(
        HKEY_LOCAL_MACHINE, _T(
        "Software\\Microsoft\\Windows\\CurrentVersion\\"
        "Explorer\\RunMRU" ));

  /*  AddMruKey(
        HKEY_CURRENT_USER, _T(
        "Software\\Microsoft\\MediaPlayer\\Player\\RecentURLList" ));

    AddMruKey(
        HKEY_LOCAL_MACHINE, _T(
        "Software\\Microsoft\\MediaPlayer\\Player\\RecentURLList" ));
    
    AddMruKey(
        HKEY_CURRENT_USER, _T(
        "Software\\Microsoft\\MediaPlayer\\Player\\RecentFileList" ));

    AddMruKey(
        HKEY_LOCAL_MACHINE, _T(
        "Software\\Microsoft\\MediaPlayer\\Player\\RecentFileList" ));*/

    AddMruKey(
        HKEY_CURRENT_USER, _T(
        "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\"
        "Wallpaper\\MRU" ));

    AddMruKey(
        HKEY_LOCAL_MACHINE, _T(
        "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\"
        "Wallpaper\\MRU" ));
}

/*************************************************************************
* Function Name:        CMruRScanner::~CMruRScanner()
* Function Purpose:     Destructor
*************************************************************************/
CMruRScanner::~CMruRScanner()
{
}

/*************************************************************************
* Function Name:        CMruRScanner::AddMruKey()
* Function Purpose:     Add key to list of MRUs that must be cleaned.
*
* Arguments:            hKey - Must be one of the following:
*                                   HKEY_CLASSES_ROOT
*                                   HKEY_CURRENT_USER
*                                   HKEY_LOCAL_MACHINE
*                                   HKEY_USERS
*
*                       szMruKeyName - Zero-terminationg string 
*                       containing path to MRU key w/o root key name.
*                        
* Return value:         True if key was successfully added to MRU list, 
*                       false otherwise.
*************************************************************************/
bool CMruRScanner::AddMruKey( const HKEY hKey, const TCHAR * szMruKeyName )
{
    _ASSERT( hKey );

    _ASSERT((   ( hKey == HKEY_CLASSES_ROOT  ) ||
                ( hKey == HKEY_CURRENT_USER  ) || 
                ( hKey == HKEY_LOCAL_MACHINE ) || 
                ( hKey == HKEY_USERS ) ));
    
    std::vector< countable< MruKey > >::iterator it;
    it = m_vMruList.begin();

    for( ; it != m_vMruList.end(); it++ )
    {
        if( ( it->get()->hKey == hKey ) && 
            ( _tcsicmp( 
                it->get()->strKeyName.GetString(), szMruKeyName ) == 0 ))
        {
            return false;
        }
    }

    countable< MruKey > spMruKey( new MruKey );

    if( !spMruKey.get() ) 
    {
        return false;
    }
    
    spMruKey->hKey = hKey;
    spMruKey->strKeyName = szMruKeyName;
    m_vMruList.push_back( spMruKey );
    
    return true;
}

/*************************************************************************
* Function Name:        CMruRScanner::ReinitAlgorithm()
* Function Purpose:       
* Function Description: 
*************************************************************************/
void CMruRScanner::ReinitAlgorithm()
{
    m_pfnAlgoSelector = &CMruRScanner::algoStep1;
    m_theIterator = m_vMruList.begin();
    m_dwIndex = 0;
}

/*************************************************************************
* Function Name:    CMruRScanner::postop()
* 
* Function Purpose: Called by CRunnable after run() function completed. 
*                   Perform correctly thread completion (close opened 
*                   handles etc.)
* 
* Arguments:        None
* Return value:     None
* 
*************************************************************************/
void CMruRScanner::postop( void )
{
    CRegistryScanner::postop();

    if( m_hScanningKey )
    {
        RegCloseKey( m_hScanningKey );
        m_hScanningKey = NULL;
    }

	m_strScanObject.Empty();
}

/*************************************************************************
* Function Name:        CMruRScanner::algorithm()
* Function Purpose:       
* Function Description: 
*************************************************************************/
void CMruRScanner::algorithm()
{
    ( this->*m_pfnAlgoSelector )();
}

/*************************************************************************
* Function Name:    CMruRScanner::algoStep1()
* Function Purpose: Walk throwg MRU list and call scanner for each key.
*
* Arguments:        None
* Return value:     None
*************************************************************************/
void CMruRScanner::algoStep1()
{
    if( m_theIterator != m_vMruList.end() )
    {
        _ASSERT( m_theIterator->get()->hKey );
        
        HKEY hKey = m_theIterator->get()->hKey;

        if( HKEY_CURRENT_USER == hKey )
        {
            m_strScanObject = _T( "HKEY_CURRENT_USER\\" );
        }
        else if ( HKEY_LOCAL_MACHINE == hKey )
        {
            m_strScanObject = _T( "HKEY_LOCAL_MACHINE\\" );
        }        
        else if( HKEY_CLASSES_ROOT == hKey )
        {
            m_strScanObject = _T( "HKEY_CLASSES_ROOT\\" );
        }
        else if( HKEY_USERS == hKey )
        {
            m_strScanObject = _T( "HKEY_USERS\\" );
        }
        else
        {
            // Incorrect hKey: We should never reach this code
            //

            _ASSERT( 0 );

            m_theIterator++ ;
            return;
        }

        LONG lResult = RegOpenKeyEx(
                                hKey,
                                m_theIterator->get()->strKeyName.GetString(),
                                0,
                                KEY_READ|KEY_WRITE| DELETE,
                                &m_hScanningKey );

        if( ERROR_SUCCESS != lResult )
        {
            m_theIterator++ ;
            return;
        }
        
        m_strScanObject += m_theIterator->get()->strKeyName;
        m_pfnAlgoSelector = m_pfnAlgoSelector = &CMruRScanner::algoStep2;
        m_dwIndex = 0;

        return;
    }

    StopJustMe();
}

/*************************************************************************
* Function Name:      CMruRScanner::algoStep2()
* Function Purpose:   Add all items under current MRU key to report list.
*
* Arguments:          None
* Return value:       None
*************************************************************************/
void CMruRScanner::algoStep2()
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

    if( ERROR_SUCCESS != lResult )
    {
        m_theIterator++;
        RegCloseKey( m_hScanningKey );
        m_hScanningKey = NULL;
        m_pfnAlgoSelector = &CMruRScanner::algoStep1;
        return;
    }

    int size = m_strScanObject.GetLength();
    m_strScanObject += _T( "\\" );
    m_strScanObject += szKeyName;

    // Notify user about current scanning object
    //
    if( m_pNotifier )
    {
        m_pNotifier->CallOnScan( ScannerId(), m_strScanObject.GetString() );
    }
    
    HKEY hOpenKey;

    if( ERROR_SUCCESS == RegOpenKeyEx(
                                m_hScanningKey,
                                szKeyName,
                                0,
                                KEY_READ|KEY_WRITE| DELETE,
                                &hOpenKey ) )
    {
        RecursivelyAddKey( hOpenKey, m_strScanObject );
        RegCloseKey( hOpenKey );
    }

    m_strScanObject.GetBufferSetLength( size );
}
