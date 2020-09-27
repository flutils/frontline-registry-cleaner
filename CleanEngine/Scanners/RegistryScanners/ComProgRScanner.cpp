#include "StdAfx.h"
#include "ComProgRScanner.h"
#include "CleanEngine.h"

/*************************************************************************
* Function Name:          CComProgRScanner::CComProgRScanner
* Function Purpose:       Constructor
*************************************************************************/
CComProgRScanner::CComProgRScanner( CCleanEngine * pEngine, INotifier * pNotifier /* = NULL */, IScanProfiler * pProfiler /* = NULL */ ) : 
	CRegistryScanner( pEngine, pNotifier, pProfiler ),
	CCleanScanner( pEngine, pNotifier, pProfiler ),
	CReportChecker( pEngine ),
    m_pfnAlgoSelector( NULL ),
    m_dwIndex( 0 )
{
}

/*************************************************************************
* Function Name:          CComProgRScanner::~CComProgRScanner
* Function Purpose:       Destructor
*************************************************************************/
CComProgRScanner::~CComProgRScanner()
{
}

/*************************************************************************
* Function Name:        CComProgRScanner::ReinitAlgorithm
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CComProgRScanner::ReinitAlgorithm()
{
    m_pfnAlgoSelector = &CComProgRScanner::algoStep1;
    m_dwIndex = 0;
}

/*************************************************************************
* Function Name:        CComProgRScanner::algorithm
* Function Purpose:     
* Function Description:
* Arguments:            None
* Return value:         None
*************************************************************************/
void CComProgRScanner::algorithm()
{
    ( this->*m_pfnAlgoSelector )();
}

/*************************************************************************
* Function Name:        CComProgRScanner::algoStep1
* Function Purpose:     
* Function Description:
* Arguments:            None
* Return value:         None
*************************************************************************/
void CComProgRScanner::algoStep1()
{
    TCHAR  szKeyName[ MAX_KEY_SIZE ];
    DWORD  dwSize = sizeof( szKeyName ) / sizeof( TCHAR );

    LONG lResult = RegEnumKeyEx(
                            HKEY_CLASSES_ROOT,
                            m_dwIndex++,
                            szKeyName,
                            &dwSize,
                            NULL,
                            NULL,
                            NULL,
                            NULL );

    if( ERROR_SUCCESS == lResult )
    {
        if( _T('.') != szKeyName[ 0 ] )
        {
            // Notify user about current scanning object.
            //
            ce_tstring strScanObject(_T( "HKEY_CLASSES_ROOT\\" ) );
            strScanObject += szKeyName;

            if( m_pNotifier )
            {
                m_pNotifier->CallOnScan( ScannerId(), strScanObject.GetString() );
            }

            // Add empty keys also.
            //
            HKEY hOpenKey;
            if( ERROR_SUCCESS == RegOpenKeyEx(
                                            HKEY_CLASSES_ROOT,
                                            szKeyName,
                                            0,
                                            KEY_READ,
                                            &hOpenKey ) )
            {
				LOG_TEXT3(_T("Checking key "),szKeyName,_T( " for empty!"))
			
				if( IsKeyEmpty( hOpenKey ) )
                {
					LOG_TEXT3( _T("Key "),szKeyName, _T( " is empty! This is TREAT!"))

					RecursivelyAddKey( hOpenKey, strScanObject );
                    RegCloseKey( hOpenKey );
                    return;
                }
				else
					LOG_TEXT3( _T("Key "),szKeyName, _T( " is not empty!"))

                RegCloseKey( hOpenKey );
            }
			else
			  LOG_TEXT(_T("Can't open comprog info key")) 


            ce_tstring strCheckedKey( szKeyName );
            strCheckedKey += _T( "\\CLSID" );

			LOG_TEXT2(_T("Try to check clsid for key "), strCheckedKey) 

			if( IsKeyExist( HKEY_CLASSES_ROOT, const_cast<const LPTSTR>( strCheckedKey.GetString() ) ) )
            {   
                CheckCLSID( szKeyName );
            }
			else
			{
				LOG_TEXT3(_T("CLSID key of "),szKeyName,_T( " is not present!"))
			
			}

			LOG_TEXT(_T("-----------------------------------------------------------------"))
        }
        
		
        return;
    }

	    
	// Enumeration completed or error occurred; stop scanning.
    //
    StopJustMe();
}

/*************************************************************************
* Function Name:        CComProgRScanner::CheckCLSID
* Function Purpose:     
* Function Description: Function checks that HKCR\<AppId>\CLSID\<value> 
*                       exist in HKCR\CLSID
*
* Arguments:            szKeyName - 'AppId' name which is need to be 
*                       checked.
*
* Return value:         None
*************************************************************************/
void CComProgRScanner::CheckCLSID( const TCHAR * szKeyName )
{
    HKEY hOpenKey = NULL;
    ce_tstring strKeyName( szKeyName );
    strKeyName += _T( "\\CLSID" );

	LOG_TEXT2(_T("Checking CLSID for "),strKeyName)
	
    LONG lResult = RegOpenKeyEx(
                            HKEY_CLASSES_ROOT,
                            strKeyName.GetString(),
                            0,
                            KEY_READ,
                            &hOpenKey );

    if( ERROR_SUCCESS != lResult )
    {
		LOG_TEXT3(_T("RegOpenKeyEx for "),strKeyName, _T( " was failed!"))
		return;
    }

    countable< TCHAR > spDataBuffer;
    DWORD dwType = REG_SZ;
    DWORD dwDataSize = 0;

    lResult = RegReadValue( hOpenKey,
                            NULL,
                            &dwType,
                            spDataBuffer,
                            dwDataSize );
    
    RegCloseKey( hOpenKey );

    if( ERROR_SUCCESS != lResult )
    {
		LOG_TEXT3(_T("RegReadValue for "),strKeyName, _T( " was failed!"))
		return;
    }

    ce_tstring strClsidKey = _T( "CLSID\\" );
    strClsidKey += spDataBuffer.get();

	if( !IsKeyExist( HKEY_CLASSES_ROOT, const_cast<const LPTSTR>( strClsidKey.GetString() ) ) || CLSIDIsInReport( strClsidKey ) )
    {
        HKEY hOpenKey;

		LOG_TEXT3(_T("HKEY_CLASSES_ROOT\\"),strClsidKey, _T( " is not present!"))
	
        lResult = RegOpenKeyEx(
                            HKEY_CLASSES_ROOT,
                            szKeyName,
                            0,
                            KEY_READ,
                            &hOpenKey );

        if( ERROR_SUCCESS == lResult )
        {
            ce_tstring strKeyName( _T( "HKEY_CLASSES_ROOT\\" ) );
            strKeyName += szKeyName;

			LOG_TEXT3(_T("Key HKEY_CLASSES_ROOT\\"),szKeyName, _T( " is TREAT!"))

            RecursivelyAddKey( hOpenKey, strKeyName );
            RegCloseKey( hOpenKey );
        }
		else
		{
			LOG_TEXT3(_T("RegOpenKeyEx HKEY_CLASSES_ROOT\\"),szKeyName, _T( " was failed!"))
		}
    }

	LOG_TEXT3(_T("CLSID "),strClsidKey,_T( " is OK!"))

}

