#include "StdAfx.h"
#include "ClientsRScanner.h"
#include "CleanEngine.h"

static const TCHAR * CLIENTS_KEY = _T( "SOFTWARE\\Clients" );

/*************************************************************************
* Function Name:        CClientsRScanner::CClientsRScanner()
* Function Purpose:     Constructor 
*************************************************************************/
CClientsRScanner::CClientsRScanner( CCleanEngine * pEngine, INotifier * pNotifier /* = NULL */, IScanProfiler * pProfiler /* = NULL */ ) :
	CRegistryScanner( pEngine, pNotifier, pProfiler ),
	CCleanScanner( pEngine, pNotifier, pProfiler ),
    m_pfnAlgoSelector( NULL ),
    m_dwIndex( 0 ),
    m_hScanningKey( NULL )
{
}

/*************************************************************************
* Function Name:        CClientsRScanner::~CClientsRScanner()
* Function Purpose:     Destructor
*************************************************************************/
CClientsRScanner::~CClientsRScanner()
{
}

/*************************************************************************
* Function Name:          CClientsRScanner::ReinitAlgorithm()
* Function Purpose:       
* Function Description: 
*************************************************************************/
void CClientsRScanner::ReinitAlgorithm()
{
    m_pfnAlgoSelector = &CClientsRScanner::algoStep1;
    m_dwIndex = 0;
}

/*************************************************************************
* Function Name:          CClientsRScanner::algorithm()
* Function Purpose:       
* Function Description: 
*************************************************************************/
void CClientsRScanner::algorithm()
{
    ( this->*m_pfnAlgoSelector )();
}

/*************************************************************************
* Function Name:    CClientsRScanner::postop()
*
* Function Purpose: Called by CRunnable after run() function completed. 
*                   Perform correctly thread completion (close opened 
*                   handles etc.)
*
* Arguments:        None
* Return value:     None
*                           
*************************************************************************/
void CClientsRScanner::postop( void )
{
    CRegistryScanner::postop();

    if( m_hScanningKey )
    {
        RegCloseKey( m_hScanningKey );
        m_hScanningKey = NULL;
    }
}

/*************************************************************************
* Function Name:            CClientsRScanner::algoStep1()
* Function Purpose:         Open HKLM\SOFTWARE\Clients key for scanning.
* Function Description:   
* Arguments:                None
* Return value:             None
*************************************************************************/
void CClientsRScanner::algoStep1()
{
    LONG lResult = RegOpenKeyEx(
                            HKEY_LOCAL_MACHINE,
                            CLIENTS_KEY,
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
    m_pfnAlgoSelector = &CClientsRScanner::algoStep2;
}

/*************************************************************************
* Function Name:        CClientsRScanner::algoStep2()
*
* Function Purpose:     Enumerate keys under m_hScanningKey, previously 
*                       opened by algoStep1() function; calls checking 
*                       function for each key.
*
* Arguments:            None
* Return value:         None
*
*************************************************************************/
void CClientsRScanner::algoStep2()
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
        LONG lResult = RegOpenKeyEx(
                                m_hScanningKey, 
                                szKeyName, 
                                0, 
                                KEY_READ|KEY_WRITE| DELETE, 
                                &hOpenKey );

        if( ERROR_SUCCESS == lResult )
        {
            CheckClientsGroup( hOpenKey, szKeyName );
            RegCloseKey( hOpenKey );
			LOG_TEXT(_T("---------------------------------------------------------------------------------"))
            return;
        }

        return;
    }

    // Enumerate completed or any error occurred, stop scanning.
    //
    StopJustMe();
}

/*************************************************************************
* Function Name:        CClientsRScanner::CheckClientsGroup()
* Function Purpose:     Verify each application in the group for correct
*                       shell command.
*
* Arguments:            hKey - Handle to an open key.
*
*                       szKeyName - zero-terminating string with the key
*                       name.
*
* Return value:         None
*************************************************************************/
void CClientsRScanner::CheckClientsGroup( 
    HKEY            hKey,
    const TCHAR *   szKeyName )
{
    _ASSERT( hKey );

    TCHAR szSubKey[ MAX_KEY_SIZE ];
    DWORD dwIndex = 0;
    DWORD dwSize;

    while( 1 )
    {
        dwSize = sizeof( szSubKey ) / sizeof( TCHAR );

        LONG lResult = RegEnumKeyEx(
                                hKey,
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

        // Notify user about current scanning object.
        //
        ce_tstring strScanObject( _T("HKEY_LOCAL_MACHINE\\" ));
        strScanObject += CLIENTS_KEY;
        strScanObject += _T( "\\" );
        strScanObject += szKeyName;
        strScanObject += _T( "\\" );
        strScanObject += szSubKey;

        if( m_pNotifier )
        {
            m_pNotifier->CallOnScan( ScannerId(), strScanObject.GetString() );
        }

        switch( CheckKeyForCommand( hKey, szSubKey ) )
        {

        case NO_PATH:
        case CORRECT_PATH:
            {
                // No 'command' found in the key or all paths exist.
                //
				LOG_TEXT3(_T("Checked key "),strScanObject,_T(" doesn't contain any paths or all paths are OK"))
            }
            break;

        case INVALID_PATH:
            {
                // SubKey contain at least one invalid 'command'.
                //
                HKEY hOpenKey;

                lResult = RegOpenKeyEx(
                                    hKey,
                                    szSubKey,
                                    0,
                                    KEY_READ|KEY_WRITE| DELETE,
                                    &hOpenKey );

                if( ERROR_SUCCESS == lResult )
                {
					LOG_TEXT3(_T("Checked key "),strScanObject,_T(" contains at least one incorrect path. This is TREAT"))
                    RecursivelyAddKey( hOpenKey, strScanObject );
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

		LOG_TEXT(_T("---------------------------------------------------------------------------------"))
    }
}

/*************************************************************************
* Function Name:    CApplicationsRScanner::CheckKeyForCommand()
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
*               INVALID_PATH:   SubKey contain at least one invalid path.
*************************************************************************/
CHECKPATH_RESULT CClientsRScanner::CheckKeyForCommand( 
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

        if( _tcsicmp( szSubKey, _T( "command" ) ) == 0 )
        {
            HKEY hOpenKey2;

            if( ERROR_SUCCESS == RegOpenKeyEx(
                                            hOpenKey,
                                            szSubKey,
                                            0,
                                            KEY_READ|KEY_WRITE| DELETE,
                                            &hOpenKey2 ) )
            {
                DWORD dwType        = REG_SZ;
                DWORD dwDataSize    = 0;
                countable< TCHAR > spDataBuffer;

                if( ERROR_SUCCESS == RegReadValue(
                                            hOpenKey2,
                                            NULL,
                                            &dwType,
                                            spDataBuffer,
                                            dwDataSize ) )
                {
                    if( CheckPaths( spDataBuffer, FALSE ) )
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
