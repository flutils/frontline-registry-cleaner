#include "StdAfx.h"
#include "SharedDllRScanner.h"
#include "CleanEngine.h"

static const TCHAR * SHAREDDLL_KEY = _T( "SOFTWARE\\Microsoft\\"
    "Windows\\CurrentVersion\\SharedDLLs" );

/*************************************************************************
* Function Name:        CSharedDllRScanner::CCSharedDllRScanner()
* Function Purpose:     Constructor 
*************************************************************************/
CSharedDllRScanner::CSharedDllRScanner( CCleanEngine * pEngine, INotifier * pNotifier /* = NULL */, IScanProfiler * pProfiler /* = NULL */ ) :
	CRegistryScanner( pEngine, pNotifier, pProfiler ),
	CCleanScanner( pEngine, pNotifier, pProfiler ),
    m_pfnAlgoSelector( NULL ),
    m_dwIndex( 0 ),
    m_hScanningKey( NULL )
{
}

/*************************************************************************
* Function Name:        CSharedDllRScanner::~CCSharedDllRScanner()
* Function Purpose:     Destructor
*************************************************************************/
CSharedDllRScanner::~CSharedDllRScanner()
{
}

/*************************************************************************
* Function Name:          CSharedDllRScanner::ReinitAlgorithm()
* Function Purpose:       
* Function Description: 
*************************************************************************/
void CSharedDllRScanner::ReinitAlgorithm()
{
    m_pfnAlgoSelector = &CSharedDllRScanner::algoStep1;
}

/*************************************************************************
* Function Name:          CSharedDllRScanner::algorithm()
* Function Purpose:       
* Function Description: 
*************************************************************************/
void CSharedDllRScanner::algorithm()
{
    ( this->*m_pfnAlgoSelector )();
}

/*************************************************************************
* Function Name:    CSharedDllRScanner::postop()
*
* Function Purpose: Called by CRunnable after run() function completed. 
*                   Perform correctly thread completion (close opened 
*                   handles etc.)
*
* Arguments:        None
* Return value:     None
*                           
*************************************************************************/
void CSharedDllRScanner::postop( void )
{
    CRegistryScanner::postop();

    if( m_hScanningKey )
    {
        RegCloseKey( m_hScanningKey );
        m_hScanningKey = NULL;
    }
}

/*************************************************************************
* Function Name:            CSharedDllRScanner::algoStep1()
* Function Purpose:         Open 'HKLM\SOFTWARE\Microsoft\Windows\
*                           CurrentVersion\SharedDLLs' key for scanning.
* Function Description:     
* Arguments:                None
* Return value:             None
*************************************************************************/
void CSharedDllRScanner::algoStep1()
{
    LONG lResult = RegOpenKeyEx(
                            HKEY_LOCAL_MACHINE,
                            SHAREDDLL_KEY,
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

	// Notify user about current scanning object.
    //
	//m_pNotifier->
    if( m_pNotifier )
    {
        ce_tstring strScanObject( _T("HKEY_LOCAL_MACHINE\\") );
        strScanObject += SHAREDDLL_KEY;

        m_pNotifier->CallOnScan( ScannerId(), strScanObject.GetString() );
    }

    m_dwIndex = 0;
    m_pfnAlgoSelector = &CSharedDllRScanner::algoStep2;
}

/*************************************************************************
* Function Name:        CSharedDllRScanner::algoStep2()
* Function Purpose:     Scan opened key for searching DLL items that 
*                       does not exist on hard disk.
*
* Arguments:            None
* Return value:         None
*
*************************************************************************/
void CSharedDllRScanner::algoStep2()
{
    TCHAR szValueName[ 16384 ];
    DWORD dwValueSize = sizeof( szValueName ) / sizeof( TCHAR );
    DWORD dwType;

    LONG lResult = RegEnumValue(
                            m_hScanningKey,
                            m_dwIndex++,
                            szValueName,
                            &dwValueSize,
                            NULL,
                            &dwType,
                            NULL,
                            NULL );

    if( ERROR_SUCCESS != lResult )
    {
        StopJustMe();
        return;
    }

	if( m_pNotifier )
	{
		m_pNotifier->CallOnScan( ScannerId(), NULL, szValueName );
	}

    if( !CheckPaths( szValueName, FALSE ) )
    {
        ce_tstring strKeyName( _T("HKEY_LOCAL_MACHINE\\") );
        ce_tstring strValueName( szValueName );
        strKeyName += SHAREDDLL_KEY;

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
