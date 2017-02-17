#include "StdAfx.h"
#include "ShellExtRScanner.h"
#include "CleanEngine.h"
#include "ComObjectRScanner.h"


static const TCHAR * APPROVED_KEY = _T( "Software\\Microsoft\\Windows\\"
    "CurrentVersion\\Shell Extensions\\Approved" );

/*************************************************************************
* Function Name:        CShellExtRScanner::CShellExtRScanner()
* Function Purpose:     Constructor  
*************************************************************************/
CShellExtRScanner::CShellExtRScanner( CCleanEngine * pEngine, INotifier * pNotifier /* = NULL */, IScanProfiler * pProfiler /* = NULL */ ) :
	CRegistryScanner( pEngine, pNotifier, pProfiler ),
	CCleanScanner( pEngine, pNotifier, pProfiler ),
    m_pfnAlgoSelector( NULL ),
    m_dwIndex( 0 ),
    m_hScanningKey( NULL )
{
}

/*************************************************************************
* Function Name:        CShellExtRScanner::~CShellExtRScanner()
* Function Purpose:     Destructor
*************************************************************************/
CShellExtRScanner::~CShellExtRScanner()
{
}

/*************************************************************************
* Function Name:        CShellExtRScanner::ReinitAlgorithm()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CShellExtRScanner::ReinitAlgorithm()
{
    m_pfnAlgoSelector = &CShellExtRScanner::algoStep1;
    m_dwIndex = 0;
}

/*************************************************************************
* Function Name:        CShellExtRScanner::algorithm()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CShellExtRScanner::algorithm()
{
    ( this->*m_pfnAlgoSelector )();
}

/*************************************************************************
* Function Name:        CShellExtRScanner::postop()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CShellExtRScanner::postop()
{
    CRegistryScanner::postop();

    if( m_hScanningKey )
    {
        RegCloseKey( m_hScanningKey );
        m_hScanningKey = NULL;
    }
}

/*************************************************************************
* Function Name:        CShellExtRScanner::algoStep1()
*
* Function Purpose:     Open "HKLM\Software\Microsoft\Windows\
*                       CurrentVersion\Shell Extensions\Approved" 
*                       registry key for scanning.
*
* Arguments:            None
* Return value:         None
*************************************************************************/
void CShellExtRScanner::algoStep1()
{
    LONG lResult = RegOpenKeyEx(
                            HKEY_LOCAL_MACHINE,
                            APPROVED_KEY,
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

	// Notify user about current scanning key.
    //
    ce_tstring strScanObject(_T( "HKEY_LOCAL_MACHINE\\" ));
    strScanObject += APPROVED_KEY;

    if( m_pNotifier )
    {
        m_pNotifier->CallOnScan( ScannerId(), strScanObject.GetString() );
    }

    m_dwIndex = 0;
    m_pfnAlgoSelector = &CShellExtRScanner::algoStep2;
}

/*************************************************************************
* Function Name:        CShellExtRScanner::algoStep2()
*
* Function Purpose:     Approved shell extensions in HKEY_LOCAL_MACHINE\
*                       SOFTWARE\Microsoft\Windows\CurrentVersion\
*                       Shell Extensions\Approved must have corresponding 
*                       registered COM servers.
*
* Arguments:            None
* Return value:         None
*************************************************************************/
void CShellExtRScanner::algoStep2()
{
    TCHAR szValueName[ 16384 ];
    DWORD dwValueSize = sizeof( szValueName ) / sizeof( TCHAR );
    DWORD dwType;
	CCleanScanner * pScanner = NULL;
	CComObjectRScanner * pComScanner = NULL;

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

    if( !IsStringGuid( szValueName ))
    {
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

    ce_tstring strClsidKey(_T( "CLSID\\" ));
    strClsidKey += szValueName;

	if( m_pNotifier )
	{
		m_pNotifier->CallOnScan( ScannerId(), NULL, szValueName );
	}

	if( !IsKeyExist( HKEY_CLASSES_ROOT, const_cast<const LPTSTR>( strClsidKey.GetString() ) ) || 
		( (pComScanner == NULL) ? FALSE : pComScanner->CLSIDIsInReport( szValueName, _T( "HKEY_CLASSES_ROOT" ) ) ) )
    {
        ce_tstring strValueName( szValueName );
        ce_tstring strKeyName(_T( "HKEY_LOCAL_MACHINE\\" ));
        strKeyName += APPROVED_KEY;

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
