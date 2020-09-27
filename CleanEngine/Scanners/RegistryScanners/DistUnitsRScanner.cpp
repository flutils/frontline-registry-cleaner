#include "StdAfx.h"
#include "DistUnitsRScanner.h"
#include "ComObjectRScanner.h"


static const TCHAR * DISTUNITS_KEY = _T( "SOFTWARE\\Microsoft\\"
    "Code Store Database\\Distribution Units" );

/*************************************************************************
* Function Name:        CDistUnitsRScanner::CDistUnitsRScanner()
* Function Purpose:     Constructor 
*************************************************************************/
CDistUnitsRScanner::CDistUnitsRScanner( CCleanEngine * pEngine, INotifier * pNotifier /* = NULL */, IScanProfiler * pProfiler /* = NULL */ ) :
	CRegistryScanner( pEngine, pNotifier, pProfiler ),
	CCleanScanner( pEngine, pNotifier, pProfiler ),
    m_pfnAlgoSelector( NULL ),
    m_dwIndex( 0 ),
    m_hScanningKey( NULL )
{
}

/*************************************************************************
* Function Name:        CDistUnitsRScanner::~CDistUnitsRScanner()
* Function Purpose:     Destructor
*************************************************************************/
CDistUnitsRScanner::~CDistUnitsRScanner()
{
}

/*************************************************************************
* Function Name:        CDistUnitsRScanner::ReinitAlgorithm()
* Function Purpose:       
* Function Description: 
*************************************************************************/
void CDistUnitsRScanner::ReinitAlgorithm()
{
    m_pfnAlgoSelector = &CDistUnitsRScanner::algoStep1;
    m_dwIndex = 0;
}

/*************************************************************************
* Function Name:        CDistUnitsRScanner::algorithm()
* Function Purpose:       
* Function Description: 
*************************************************************************/
void CDistUnitsRScanner::algorithm()
{
    ( this->*m_pfnAlgoSelector )();
}

/*************************************************************************
* Function Name:    CDistUnitsRScanner::postop()
*
* Function Purpose: Called by CRunnable after run() function completed. 
*                   Perform correctly thread completion (close opened 
*                   handles etc.)
*
* Arguments:        None
* Return value:     None
*                           
*************************************************************************/
void CDistUnitsRScanner::postop( void )
{
    CRegistryScanner::postop();

    if( m_hScanningKey )
    {
        RegCloseKey( m_hScanningKey );
        m_hScanningKey = NULL;
    }
}

/*************************************************************************
* Function Name:        CDistUnitsRScanner::algoStep1()
*
* Function Purpose:     Open 'HKLM\SOFTWARE\Microsoft\Code Store Database\
*                       Distribution Units' key for scanning.
*
* Arguments:            None
* Return Value:         None
*************************************************************************/
void CDistUnitsRScanner::algoStep1()
{
    LONG lResult = RegOpenKeyEx(
                            HKEY_LOCAL_MACHINE,
                            DISTUNITS_KEY,
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

    m_pfnAlgoSelector = &CDistUnitsRScanner::algoStep2;
}

/*************************************************************************
* Function Name:        CDistUnitsRScanner::algoStep2()
* Function Purpose:     Check distribution units.
*
* Arguments:            None
* Return Value:         None
*************************************************************************/
void CDistUnitsRScanner::algoStep2()
{
    _ASSERT( m_hScanningKey );
    
    TCHAR  szKeyName[ MAX_KEY_SIZE ];
    DWORD  dwSize = sizeof( szKeyName ) / sizeof( TCHAR );
	CCleanScanner * pScanner = NULL;
	CComObjectRScanner * pComScanner = NULL;

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
        StopJustMe();
        return;
    }

    ce_tstring strScanObject( _T("HKEY_LOCAL_MACHINE\\") );
    strScanObject += DISTUNITS_KEY;
    strScanObject += _T("\\");
    strScanObject += szKeyName;

    // Notify user about current scanning object.
    //
   
    
    HKEY hOpenKey;

    lResult = RegOpenKeyEx(
                        m_hScanningKey,
                        szKeyName,
                        0,
                        KEY_READ|KEY_WRITE| DELETE,
                        &hOpenKey );

    if( ERROR_SUCCESS != lResult )
    {
        return;
    }
    
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

    if( !IsStringGuid( szKeyName ) )
    {
        RegCloseKey( hOpenKey );
        return;
    }

	try
	{
		pScanner = m_pEngine->GetPointerToScanner( COMOBJECT_SCANNER_ID );
		if ( pScanner->IsScannerEnabled() )
		{
			pComScanner = dynamic_cast< CComObjectRScanner* >(pScanner);
		}
		else
		{
			pComScanner = NULL;
		}

	}
	catch (...)
	{
		pComScanner = NULL;
	}

	ce_tstring strClsidKey( _T("CLSID\\") );
	strClsidKey += szKeyName;

    HKEY hOpenKey2;
    lResult = RegOpenKeyEx(
                        HKEY_CLASSES_ROOT,
                        strClsidKey.GetString(),
                        0,
                        KEY_READ|KEY_WRITE| DELETE,
                        &hOpenKey2 );

    if( ERROR_SUCCESS != lResult )
    {
        RecursivelyAddKey( hOpenKey, strScanObject );
        RegCloseKey( hOpenKey );
        return;
    }
    
    if( IsKeyEmpty( hOpenKey2 )  || ( (pComScanner == NULL) ? FALSE : pComScanner->CLSIDIsInReport( szKeyName, _T( "HKEY_CLASSES_ROOT\\" ) ) ) )
    {
        RecursivelyAddKey( hOpenKey, strScanObject, SEVERITY_MEDIUM, 0, true );
    }

    RegCloseKey( hOpenKey );
    RegCloseKey( hOpenKey2 );
}
