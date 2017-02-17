#include "StdAfx.h"
#include "HelpRScanner.h"
#include "CleanEngine.h"

static const TCHAR * WINHELP_KEY = 
    _T( "SOFTWARE\\Microsoft\\Windows\\Help" );

static const TCHAR * HTMLHELP_KEY = 
    _T( "SOFTWARE\\Microsoft\\Windows\\HTML Help" );

/*************************************************************************
* Function Name:        CHelpRScanner::CHelpRScanner()
* Function Purpose:     Constructor 
*************************************************************************/
CHelpRScanner::CHelpRScanner( CCleanEngine * pEngine, INotifier * pNotifier /* = NULL */, IScanProfiler * pProfiler /* = NULL */ ) :
	CRegistryScanner( pEngine, pNotifier, pProfiler ),
	CCleanScanner( pEngine, pNotifier, pProfiler ),
    m_pfnAlgoSelector( NULL ),
    m_hScanningKey( NULL )
{
}

/*************************************************************************
* Function Name:        CHelpRScanner::~CHelpRScanner()
* Function Purpose:     Destructor
*************************************************************************/
CHelpRScanner::~CHelpRScanner()
{
}

/*************************************************************************
* Function Name:          CHelpRScanner::ReinitAlgorithm()
* Function Purpose:       
* Function Description: 
*************************************************************************/
void CHelpRScanner::ReinitAlgorithm()
{
    m_pfnAlgoSelector = &CHelpRScanner::algoStep1;
}

/*************************************************************************
* Function Name:        CHelpRScanner::algorithm()
* Function Purpose:     
* Arguments:            None
* Return Value:         None
*************************************************************************/
void CHelpRScanner::algorithm()
{
    ( this->*m_pfnAlgoSelector )();
}

/*************************************************************************
* Function Name:        CHelpRScanner::algoStep1()
*
* Function Purpose:     Search help entries in 'HKLM\SOFTWARE\Microsoft\
*                       Windows\Help' & in 'HKLM\SOFTWARE\Microsoft\
*                       Windows\HTML Help' that does not exist on hard 
*                       drive.
*
* Arguments:            None
* Return Value:         None
*************************************************************************/
void CHelpRScanner::algoStep1()
{
    LONG lResult = RegOpenKeyEx(
                            HKEY_LOCAL_MACHINE,
                            WINHELP_KEY,
                            0,
                            KEY_READ|KEY_WRITE| DELETE,
                            &m_hScanningKey );

    if( ERROR_SUCCESS != lResult )
    {
        // Can't open required key, stop scanning.
        //

        StopJustMe();
        return;
    }

    // Notify user about current scanning object.
    //
    ce_tstring strScanObject(_T( "HKEY_LOCAL_MACHINE\\" ));
    strScanObject += WINHELP_KEY;

    if( m_pNotifier )
    {
        m_pNotifier->CallOnScan( ScannerId(), strScanObject.GetString() );
    }
    
    CheckHelp( m_hScanningKey, WINHELP_KEY );
    RegCloseKey( m_hScanningKey );

    lResult = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
                            HTMLHELP_KEY,
                            0,
                            KEY_READ|KEY_WRITE| DELETE,
                            &m_hScanningKey );

    if( ERROR_SUCCESS != lResult )
    {
        // Can't open required key, stop scanning.
        //

        StopJustMe();
        return;
    }
    
    strScanObject = _T( "HKEY_LOCAL_MACHINE\\" );
    strScanObject += HTMLHELP_KEY;

    if( m_pNotifier )
    {
        m_pNotifier->CallOnScan( ScannerId(), strScanObject.GetString() );
    }

    CheckHelp( m_hScanningKey, HTMLHELP_KEY );
    RegCloseKey( m_hScanningKey );
    
    StopJustMe();
}

/*************************************************************************
* Function Name:        CHelpRScanner::CheckHelp()
* Function Purpose:     
*
* Arguments:            hKey - Handle to current open key.
*                       szFullKeyName - Zero-terminating string 
*                       containing key name is need to be scanned.
*
* Return Value:         None
*************************************************************************/
void CHelpRScanner::CheckHelp( HKEY hKey, const TCHAR * szFullKeyName )
{
    // Registry value names are limited to 32767 bytes. The ANSI version 
    // of RegEnumValue treats this param as a USHORT value. Therefore, 
    // if you specify a value greater than 32767 bytes, there is an 
    // overflow and the function may return ERROR_MORE_DATA.
    //
    TCHAR infoBuf[ 32767 ];
    TCHAR szValueName[ 16384 ];
    TCHAR szValueData[ MAX_PATH ];
    DWORD dwValueSize;
    DWORD dwDataSize;
    DWORD dwIndex = 0;
    DWORD dwType;

    while( 1 )
    {
        dwValueSize = sizeof( szValueName ) / sizeof( TCHAR );
        dwDataSize  = sizeof( szValueData ) / sizeof( TCHAR );

		if( ABANDONNED == InterrogateMngr() )
		{
			break;
		}

        LONG lResult = RegEnumValue(   
                            m_hScanningKey, 
                            dwIndex++ , 
                            szValueName, 
                            &dwValueSize, 
                            NULL, 
                            &dwType, 
                            reinterpret_cast< PBYTE >(&szValueData[ 0 ]),
                            &dwDataSize );

        if( ERROR_SUCCESS != lResult )
        {
            break;
        }

		if( m_pNotifier )
		{
			m_pNotifier->CallOnScan( ScannerId(), NULL,szValueName );
		}

        if(( REG_SZ != dwType ) && ( REG_EXPAND_SZ != dwType ))
        {
            // TODO: ignore or fix items that have other type than REG_SZ ?
            //

            continue;
        }

        szValueData[ sizeof( szValueData ) - 1 ] = _T('\0');
        DWORD bufCharCount = ExpandEnvironmentStrings(
                                                    szValueData,
                                                    infoBuf,
                                                    sizeof( infoBuf ) );

		ce_tstring strKeyName( _T("HKEY_LOCAL_MACHINE\\") );
        ce_tstring strValueName( szValueName );

        strKeyName += szFullKeyName;

        if( ( bufCharCount > sizeof( infoBuf ) ) || ( !bufCharCount ) )
        {
			m_pEngine->AddItem(
				countable<IReportItem>(
					CREATE_REG_ITEM(
						CRegistryScanner::CleanupObject,
						ScannerId(),
						SEVERITY_MEDIUM,
						strKeyName,
						strValueName,
						dwType
					) ) );
            continue;
        }
            
        ce_tstring strPath( infoBuf );
        strPath += _T( "\\" );
        strPath += szValueName;

        if( !CheckPaths( strPath, FALSE ))
        {
			m_pEngine->AddItem(
				countable<IReportItem>(
					CREATE_REG_ITEM(
						CRegistryScanner::CleanupObject,
						ScannerId(),
						SEVERITY_MEDIUM,
						strKeyName,
						strValueName,
						dwType
					) ) );
        }
    }
}
