#include "StdAfx.h"
#include "BhoRScanner.h"
#include "CleanEngine.h"
#include "ComObjectRScanner.h"


static const TCHAR * BHO_KEY = _T( "Software\\Microsoft\\Windows\\"
    "CurrentVersion\\Explorer\\Browser Helper Objects" );

/*************************************************************************
* Function Name:        CBhoRScanner::CBhoRScanner()
* Function Purpose:     Constructor
*************************************************************************/
CBhoRScanner::CBhoRScanner( CCleanEngine * pEngine, INotifier * pNotifier /* = NULL */, IScanProfiler * pProfiler /* = NULL */ ) :
	CRegistryScanner( pEngine, pNotifier, pProfiler ),
	CCleanScanner( pEngine, pNotifier, pProfiler ),
    m_pfnAlgoSelector( NULL ),
    m_dwIndex( 0 ),
    m_hScanningKey( NULL )
{
}

/*************************************************************************
* Function Name:        CBhoRScanner::~CBhoRScanner()
* Function Purpose:     Destructor
*************************************************************************/
CBhoRScanner::~CBhoRScanner()
{
}

/*************************************************************************
* Function Name:            CBhoRScanner::ReinitAlgorithm()
* Function Purpose:       
* Function Description: 
*************************************************************************/
void CBhoRScanner::ReinitAlgorithm()
{
    m_pfnAlgoSelector = &CBhoRScanner::algoStep1;
    m_dwIndex = 0;
}

/*************************************************************************
* Function Name:    CBhoRScanner::postop()
*
* Function Purpose: Called by CRunnable after run() function completed. 
*                   Perform correctly thread completion (close opened 
*                   handles etc.)
*
* Arguments:        None
* Return value:     None
* 
*************************************************************************/
void CBhoRScanner::postop( void )
{
    CRegistryScanner::postop();

    if( m_hScanningKey )
    {
        RegCloseKey( m_hScanningKey );
        m_hScanningKey = NULL;
    }
}

/*************************************************************************
* Function Name:            CBhoRScanner::algorithm()
* Function Purpose:         
* Function Description:     
*************************************************************************/
void CBhoRScanner::algorithm()
{
    ( this->*m_pfnAlgoSelector )();
}

/*************************************************************************
* Function Name:        CBhoRScanner::algoStep1()
*
* Function Purpose:     Open "HKLM\Software\Microsoft\Windows\
*                       CurrentVersion\Explorer\Browser Helper Objects" 
*                       key for scanning.
*
* Arguments:            None
* Return value:         None
*
*************************************************************************/
void CBhoRScanner::algoStep1()
{
    LONG lResult = RegOpenKeyEx(
                            HKEY_LOCAL_MACHINE,
                            BHO_KEY,
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
    m_pfnAlgoSelector = &CBhoRScanner::algoStep2;
}

/*************************************************************************
* Function Name:        CBhoRScanner::algoStep2()
*
* Function Purpose:     Enumerate keys under m_hScanningKey, previously 
*                       opened by algoStep1() function; search BHOs that
*                       has no corresponding CLSID.
*
* Arguments:            None
* Return value:         None
*
*************************************************************************/
void CBhoRScanner::algoStep2()
{
    _ASSERT( m_hScanningKey );

    TCHAR  szKeyName[ MAX_KEY_SIZE ];
    DWORD  dwSize = sizeof( szKeyName ) / sizeof( TCHAR );

    LONG lResult = RegEnumKeyEx(
                                m_hScanningKey, 
                                m_dwIndex++ , 
                                szKeyName, 
                                &dwSize, 
                                NULL, 
                                NULL,
                                NULL,
                                NULL );

    if( ERROR_SUCCESS == lResult )
    {
        // Notify user about current scanning object.
        //
        ce_tstring strScanObject( _T("HKEY_LOCAL_MACHINE\\" ));
        strScanObject += BHO_KEY;
        strScanObject += _T( "\\" );
        strScanObject += szKeyName;
		CCleanScanner * pScanner = NULL;
		CComObjectRScanner * pComScanner = NULL;

        if( m_pNotifier )
        {
            m_pNotifier->CallOnScan( ScannerId(), strScanObject.GetString() );
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
        strClsidKey += szKeyName;
        
		if( !IsKeyExist( HKEY_CLASSES_ROOT, const_cast<const LPTSTR>( strClsidKey.GetString() ) ) || 
			( (pComScanner == NULL) ? FALSE : pComScanner->CLSIDIsInReport( szKeyName, _T( "HKEY_CLASSES_ROOT\\" ) ) ) )
        {
            HKEY hOpenKey;

            lResult = RegOpenKeyEx(
                                m_hScanningKey, 
                                szKeyName, 
                                0, 
                                KEY_READ|KEY_WRITE| DELETE, 
                                &hOpenKey );

            if( ERROR_SUCCESS == lResult )
            {
                RecursivelyAddKey( hOpenKey, strScanObject );
                RegCloseKey( hOpenKey );
            }
        }

        return;        
    }

    // Enumerate completed or any error occurred, stop scanning.
    //
    StopJustMe();
}
