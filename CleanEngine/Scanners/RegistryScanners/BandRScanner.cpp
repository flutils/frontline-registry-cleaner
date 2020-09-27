#include "StdAfx.h"
#include "BandRScanner.h"
#include "CleanEngine.h"
#include "ComObjectRScanner.h"


static const TCHAR * TOOLBAR_KEY = \
    _T( "SOFTWARE\\Microsoft\\Internet Explorer\\Toolbar" );

static const TCHAR * EXPLORERBARS_KEY = \
    _T( "SOFTWARE\\Microsoft\\Internet Explorer\\Explorer Bars" );

/*************************************************************************
* Function Name:        CBandRScanner::CBandRScanner()
* Function Purpose:     Constructor  
*************************************************************************/
CBandRScanner::CBandRScanner( CCleanEngine * pEngine, INotifier * pNotifier /* = NULL */, IScanProfiler * pProfiler /* = NULL */ ) :
	CRegistryScanner( pEngine, pNotifier, pProfiler ),
	CCleanScanner( pEngine, pNotifier, pProfiler ),
    m_pfnAlgoSelector( NULL ),
    m_dwIndex( 0 ),
    m_hScanningKey( NULL )
{
}

/*************************************************************************
* Function Name:        CBandRScanner::~CBandRScanner()
* Function Purpose:     Destructor
*************************************************************************/
CBandRScanner::~CBandRScanner()
{
}

/*************************************************************************
* Function Name:        CBandRScanner::ReinitAlgorithm()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CBandRScanner::ReinitAlgorithm()
{
    m_pfnAlgoSelector = &CBandRScanner::algoStep1;
    m_dwIndex = 0;
}

/*************************************************************************
* Function Name:        CBandRScanner::algorithm()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CBandRScanner::algorithm()
{
    ( this->*m_pfnAlgoSelector )();
}

/*************************************************************************
* Function Name:        CBandRScanner::postop()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CBandRScanner::postop()
{
    CRegistryScanner::postop();

    if( m_hScanningKey )
    {
        RegCloseKey( m_hScanningKey );
        m_hScanningKey = NULL;
    }
}

/*************************************************************************
* Function Name:        CBandsRScanner::algoStep1()
*
* Function Purpose:     Open 'HKLM\Software\Microsoft\Internet Explorer\
*                       Toolbar' registry key for scanning.
*
* Arguments:            None
* Return value:         None
*************************************************************************/
void CBandRScanner::algoStep1()
{
    LONG lResult = RegOpenKeyEx(
                            HKEY_LOCAL_MACHINE,
                            TOOLBAR_KEY,
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
    m_pfnAlgoSelector = &CBandRScanner::algoStep2;
}

/*************************************************************************
* Function Name:        CBandsRScanner::algoStep2()
*
* Function Purpose:     Enumerate items under m_hScanningKey key,
*                       previously opened by algoStep1() function and 
*                       check them. After that open 'HKCU\Software\
*                       Microsoft\Internet Explorer\Explorer Bars' key 
*                       for scan.
*
* Arguments:            None
* Return value:         None
*
*************************************************************************/
void CBandRScanner::algoStep2()
{
    _ASSERT( m_hScanningKey );

    TCHAR  szKeyName[ MAX_KEY_SIZE ];
    DWORD  dwSize = sizeof( szKeyName ) / sizeof( TCHAR );

    LONG lResult = RegEnumValue(
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
        ce_tstring strScanObject( _T( "HKEY_LOCAL_MACHINE\\" ));
        strScanObject += TOOLBAR_KEY;
        strScanObject += _T("\\");
        strScanObject += szKeyName;
		CCleanScanner * pScanner = NULL;
		CComObjectRScanner * pComScanner = NULL;

        // Notify user about currently scan object
        //
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
        
		ce_tstring strKeyName = _T( "CLSID\\" );
        strKeyName += szKeyName;

        if( !IsKeyExist( HKEY_CLASSES_ROOT, const_cast<const LPTSTR>( strKeyName.GetString() ) ) || 
			( (pComScanner == NULL) ? FALSE : pComScanner->CLSIDIsInReport( szKeyName, _T( "HKEY_CLASSES_ROOT\\" ) ) ) )
        {
            HKEY hOpenKey;

            if( ERROR_SUCCESS == RegOpenKeyEx(
                                            m_hScanningKey,
                                            szKeyName,
                                            0,
                                            KEY_READ|KEY_WRITE| DELETE,
                                            &hOpenKey ) )
            {
                RecursivelyAddKey( hOpenKey, strScanObject );
                RegCloseKey( hOpenKey );
            }
        }

        return;
    }

    // Enumerate completed or any error occurred, switch to next step.
    //
    RegCloseKey( m_hScanningKey );
    m_hScanningKey = NULL;

    lResult = RegOpenKeyEx(
                        HKEY_CURRENT_USER,
                        EXPLORERBARS_KEY,
                        0,
                        KEY_READ|KEY_WRITE| DELETE,
                        &m_hScanningKey );

    if( ERROR_SUCCESS != lResult )
    {
        // Can't open required key, stop scanning
        //

        StopJustMe();
        return ;
    }

    m_dwIndex = 0;
    m_pfnAlgoSelector = &CBandRScanner::algoStep3;
}

/*************************************************************************
* Function Name:        CBandsRScanner::algoStep3()
*
* Function Purpose:     Enumerate items under m_hScanningKey key,
*                       previously opened by algoStep2() function.
*
* Arguments:            None
* Return value:         None
*************************************************************************/
void CBandRScanner::algoStep3()
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
        ce_tstring strScanObject( _T("HKEY_CURRENT_USER\\") );
        strScanObject += EXPLORERBARS_KEY;
        strScanObject += _T( "\\" );
        strScanObject += szKeyName;

        //
        // Notify user about currently scan object
        //
        if( m_pNotifier )
        {
            m_pNotifier->CallOnScan( ScannerId(), strScanObject.GetString() );
        }
        
        ce_tstring strKeyName = _T( "CLSID\\" );
        strKeyName += szKeyName;

        if( !IsKeyExist( HKEY_CLASSES_ROOT, const_cast<const LPTSTR>( strKeyName.GetString() ) ) )
        {
            HKEY hOpenKey;

            if( ERROR_SUCCESS == RegOpenKeyEx(
                                            m_hScanningKey,
                                            szKeyName,
                                            0,
                                            KEY_READ|KEY_WRITE| DELETE,
                                            &hOpenKey ) )
            {
                RecursivelyAddKey( hOpenKey, strScanObject );
                RegCloseKey( hOpenKey );
            }
        }

        return;
    }
    
    //
    // Enumerate completed or any error occurred, end scanning.
    //
    StopJustMe();
}
