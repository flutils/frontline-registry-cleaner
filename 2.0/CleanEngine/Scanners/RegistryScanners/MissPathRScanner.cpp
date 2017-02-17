#include "StdAfx.h"
#include "MissPathRScanner.h"
#include "CleanEngine.h"

static const TCHAR * SOFTWARE_KEY = _T( "Software" );

/*************************************************************************
* Function Name:        CMissPathRScanner::CMissPathRScanner()
* Function Purpose:     Constructor
*************************************************************************/
CMissPathRScanner::CMissPathRScanner( CCleanEngine * pEngine, INotifier * pNotifier /* = NULL */, IScanProfiler * pProfiler /* = NULL */ ) :
	CRegistryScanner( pEngine, pNotifier, pProfiler ),
	CCleanScanner( pEngine, pNotifier, pProfiler ),
    m_pfnAlgoSelector( NULL ),
    m_dwIndex( 0 ),
    m_hScanningKey( NULL )
{
}

/*************************************************************************
* Function Name:        CMissPathRScanner::CMissPathRScanner()
* Function Purpose:     Destructor
*************************************************************************/
CMissPathRScanner::~CMissPathRScanner()
{
}

/*************************************************************************
* Function Name:        CComProgRScanner::ReinitAlgorithm()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CMissPathRScanner::ReinitAlgorithm()
{
    m_pfnAlgoSelector = &CMissPathRScanner::algoStep1;
    m_dwIndex = 0;
}

/*************************************************************************
* Function Name:    CMissPathRScanner::postop()
*
* Function Purpose: Called by CRunnable after run() function completed. 
*                   Perform correctly thread completion (close opened 
*                   handles etc.)
*
* Arguments:        None
* Return value:     None
*                           
*************************************************************************/
void CMissPathRScanner::postop( void )
{
    CRegistryScanner::postop();

    if( m_hScanningKey )
    {
        RegCloseKey( m_hScanningKey );
        m_hScanningKey = NULL;
    }
}

/*************************************************************************
* Function Name:        CMissPathRScanner::algorithm()
* Function Purpose:         
* Function Description:     
*************************************************************************/
void CMissPathRScanner::algorithm()
{
    ( this->*m_pfnAlgoSelector )();
}

/*************************************************************************
* Function Name:            CMissPathRScanner::algoStep1()
* Function Purpose:         Open "HKEY_LOCAL_MACHINE\Software" key for 
*                           scanning.
* Function Description:   
* Arguments:                None
* Return value:             None
*************************************************************************/
void CMissPathRScanner::algoStep1()
{
    LONG lResult = RegOpenKeyEx(
                            HKEY_LOCAL_MACHINE,
                            SOFTWARE_KEY,
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
    m_pfnAlgoSelector = &CMissPathRScanner::algoStep2;
}

/*************************************************************************
* Function Name:            CMissPathRScanner::algoStep2()
* Function Purpose:         Scan "HKEY_LOCAL_MACHINE\Software" for missed
*                           paths and after that open 
*                           "HKEY_CURRENT_USER\Software" key for scanning.
*
* Arguments:                None
* Return value:             None
*************************************************************************/
void CMissPathRScanner::algoStep2()
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
        // Notify user about current scan object
        //
		if ( !StrCmp( szKeyName, _T( "Classes" ) ) )
		{
			return;
		}
		ce_tstring strScanObject( _T("HKEY_LOCAL_MACHINE") );
        strScanObject += _T("\\");
        strScanObject += SOFTWARE_KEY;
        strScanObject += _T("\\");
        strScanObject += szKeyName;

        if( m_pNotifier )
        {
            m_pNotifier->CallOnScan( ScannerId(), strScanObject.GetString() );
        }

        HKEY hOpenKey;
        LONG lResult = RegOpenKeyEx(
                                m_hScanningKey,
                                szKeyName,
                                0,
                                KEY_READ|KEY_WRITE| DELETE,
                                &hOpenKey );

        if( ERROR_SUCCESS == lResult )
        {
            CheckMissedPaths(
                        hOpenKey,
                        strScanObject,
						strScanObject.GetLength() );

            RegCloseKey( hOpenKey );
            return;
        }

        return;
    }

    RegCloseKey( m_hScanningKey );
    m_hScanningKey = NULL;

    lResult = RegOpenKeyEx(
                        HKEY_CURRENT_USER,
                        SOFTWARE_KEY,      
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
    m_pfnAlgoSelector = &CMissPathRScanner::algoStep3;
}

/*************************************************************************
* Function Name:            CMissPathRScanner::algoStep1()
* Function Purpose:         Scan "HKEY_CURRENT_USER\Software" key for 
*                           missed paths.
*
* Arguments:                None
* Return value:             None
*************************************************************************/
void CMissPathRScanner::algoStep3()
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
        // Notify user about current scan object.
        //
		if ( !StrCmp( szKeyName, _T( "Classes" ) ) )
		{
			return;
		}

        ce_tstring strScanObject( _T("HKEY_CURRENT_USER") );
        strScanObject += _T("\\");
        strScanObject += SOFTWARE_KEY;
        strScanObject += _T("\\");
        strScanObject += szKeyName;

        if( m_pNotifier )
        {
            m_pNotifier->CallOnScan( ScannerId(), strScanObject.GetString() );
        }

        HKEY hOpenKey;
        LONG lResult = RegOpenKeyEx(
                                m_hScanningKey,
                                szKeyName,
                                0,
                                KEY_READ|KEY_WRITE| DELETE,
                                &hOpenKey );

        if( ERROR_SUCCESS == lResult )
        {
            CheckMissedPaths(
                        hOpenKey,
                        strScanObject,
                        strScanObject.GetLength() );

            RegCloseKey( hOpenKey );
            return;
        }

        return;
    }

    RegCloseKey( m_hScanningKey );
    m_hScanningKey = NULL;

    StopJustMe();
}

/*************************************************************************
* Function Name:            CMissPathRScanner::CheckMissedPaths()
* Function Purpose:         Scan key for missed paths.
* Arguments:
* Return value:             None
*************************************************************************/
void CMissPathRScanner::CheckMissedPaths(
    HKEY                    hKey,
    ce_tstring&             rstrKeyName,
    int						nameSize,
    unsigned                nRecurseLevel )
{
    if( nRecurseLevel > MaxRecurseLevel )
    {
        return;
    }

    // Notify user about current scan object
    //

    if( m_pNotifier /*&& ( 0 == nRecurseLevel )*/ )
    {
        m_pNotifier->CallOnScan( ScannerId(), rstrKeyName.GetString() );
    }

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

        LONG lResult = RegEnumValue(
                            hKey,
                            dwIndex++,
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
			m_pNotifier->CallOnScan( ScannerId(), rstrKeyName, szValueName );
		}

        if( REG_SZ != dwType )
        {
            continue;
        }

        szValueData[ sizeof( szValueData ) - 1 ] = _T( '\0' );

        // Reset opening quotes
        //

        TCHAR * pValueData = szValueData;
        unsigned n = 0;  // quotes counter

        if( _T('\"') == *pValueData )
        {
            while( _T('\"') == *pValueData )
            {
                pValueData ++;
                n ++;
            }
        }

        // Analyze strings that starts from [D:\] symbols only
        //

        if( ( dwDataSize > ( 2 + n )) &&
            ( pValueData[ 1 ] == _T(':') ) &&
            ( pValueData[ 2 ] == _T('\\') ) )
        {
            TCHAR szDriveRoot[ 4 ];
            _tcsncpy( szDriveRoot, pValueData, 3 );
            szDriveRoot[ 3 ] = _T( '\0' );

            if( GetDriveType( szDriveRoot ) == DRIVE_FIXED )
            {
				LOG_TEXT3(rstrKeyName, _T(" has "), pValueData)

                if( !CheckPathExistence( pValueData ) )
                {
					LOG_TEXT2(pValueData, _T(" is TREAT!!!"))
                    ce_tstring strValueName( szValueName );

					m_pEngine->AddItem(
						countable<IReportItem>(
							CREATE_REG_ITEM(
								CRegistryScanner::CleanupObject,
								ScannerId(),
								SEVERITY_MEDIUM,
								rstrKeyName,
								strValueName,
								dwType
							) ) );
                }
            }
        }
    }
    
    TCHAR szKeyName[ MAX_KEY_SIZE ];
    DWORD dwSize;
    
    dwIndex = 0;

    while( 1 )
    {
		if( ABANDONNED == InterrogateMngr() )
		{
			break;
		}

        dwSize = sizeof( szKeyName ) / sizeof( TCHAR );

        LONG lResult = RegEnumKeyEx(
                                hKey,
                                dwIndex ++,
                                szKeyName,
                                &dwSize,
                                NULL,
                                NULL,
                                NULL,
                                NULL );

        if( ERROR_SUCCESS != lResult )
        {
            break;
        }
        
        HKEY hOpenKey;
    
        if( ERROR_SUCCESS == RegOpenKeyEx(
                                    hKey,
                                    szKeyName,
                                    0,
                                    KEY_READ|KEY_WRITE| DELETE,
                                    &hOpenKey ))
        {
            rstrKeyName += _T( "\\" );
            rstrKeyName += szKeyName;

            CheckMissedPaths(
                        hOpenKey,
                        rstrKeyName,
                        rstrKeyName.GetLength(),
                        nRecurseLevel + 1 );
            
            rstrKeyName.GetBufferSetLength( nameSize );
            RegCloseKey( hOpenKey );
        }
    }
}
