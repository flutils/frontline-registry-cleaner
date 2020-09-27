#include "StdAfx.h"
#include "ComClassRScanner.h"
#include <atlpath.h>
#include "CleanEngine.h"

static const TCHAR * CLSID_KEY = _T( "CLSID" );

/*************************************************************************
* Function Name:        CComClassRScanner::CComClassRScanner
* Function Purpose:     Constructor  
*************************************************************************/
CComClassRScanner::CComClassRScanner( CCleanEngine * pEngine, INotifier * pNotifier /* = NULL */, IScanProfiler * pProfiler /* = NULL */ ) :
	CRegistryScanner( pEngine, pNotifier, pProfiler ),
	CCleanScanner( pEngine, pNotifier, pProfiler ),
    m_pfnAlgoSelector( NULL ),
    m_hScanningKey( NULL ),
    m_dwIndex( 0L )
{
	//hLogFile = CreateFile( "IncorrectPathLog.txt", GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	//if( !hLogFile )
	//{
	//	hLogFile = NULL;
	//}
	Checkers.clear();

	IComServerChecker* pChecker;

	pChecker = new CEmptyKeyChecker( this, pEngine);
	Checkers.push_back( pChecker );

	pChecker = new CNetSkipper( this, pEngine);
	Checkers.push_back( pChecker );

	pChecker = new CInprocServerChecker( this, pEngine);
	Checkers.push_back( pChecker );

	pChecker = new CLocalServerChecker( this, pEngine);
	Checkers.push_back( pChecker );

	pChecker = new CInprocHandlerChecker( this, pEngine);
	Checkers.push_back( pChecker );

	pChecker = new CProgIDChecker( this, pEngine);
	Checkers.push_back( pChecker );

	pChecker = new CAppIDChecker( this, pEngine);
	Checkers.push_back( pChecker );

	pChecker = new CAutoConvertToChecker( this, pEngine);
	Checkers.push_back( pChecker );

	pChecker = new CAutoTreatAsChecker( this, pEngine);
	Checkers.push_back( pChecker );

	pChecker = new CTreatAsChecker( this, pEngine);
	Checkers.push_back( pChecker );

}

/*************************************************************************
* Function Name:          CComClassRScanner::~CComClassRScanner
* Function Purpose:       Destructor
* Function Description: 
*************************************************************************/
CComClassRScanner::~CComClassRScanner()
{
	for ( unsigned int i = 0; i < Checkers.size(); i++)
	{
		delete Checkers[i];
	}
	Checkers.clear();
}

/*************************************************************************
* Function Name:          CComClassRScanner::ReinitAlgorithm
* Function Purpose:       
* Function Description: 
*************************************************************************/
void CComClassRScanner::ReinitAlgorithm()
{
    m_pfnAlgoSelector = &CComClassRScanner::algoStep1;
    m_dwIndex = 0;
}

/*************************************************************************
* Function Name:          CComClassRScanner::algorithm()
* Function Purpose:       
* Function Description: 
*************************************************************************/
void CComClassRScanner::algorithm()
{
    ( this->*m_pfnAlgoSelector )();
}

/*************************************************************************
* Function Name:          CComClassRScanner::algoStep1
*
* Function Purpose:       Open HKCR\CLSID registry key for scanning.
* Function Description:   
*
* Arguments:              None
* Return value:           None
*
*************************************************************************/
void CComClassRScanner::algoStep1()
{
    LONG lResult = RegOpenKeyEx(
                            HKEY_CLASSES_ROOT, 
                            CLSID_KEY,
                            0, 
                            KEY_READ,
                            &m_hScanningKey);

    if( ERROR_SUCCESS != lResult )
    {
        // Can't open required key, stop scanning
        //

        StopJustMe();   
        return;
    }
    
    m_dwIndex = 0;
    m_pfnAlgoSelector = &CComClassRScanner::algoStep2;
}

/*************************************************************************
* Function Name:        CComClassRScanner::algoStep2
*
* Function Purpose:     Searches COM class IDs that referenced inexistent 
*                       file of TypeLib, or that have no values.
*
* Arguments:            None
* Return value:         None
*                           
*************************************************************************/
void CComClassRScanner::algoStep2()
{
    TCHAR       szKeyName[ MAX_KEY_SIZE ];
    DWORD       dwSize = sizeof( szKeyName ) / sizeof( TCHAR );

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
        // Notify user about current scanning object
        //
        if( m_pNotifier )
        {
            ce_tstring strScanObject( _T("HKEY_CLASSES_ROOT\\") );
            strScanObject += CLSID_KEY;

			m_pNotifier->CallOnScan( ScannerId(), strScanObject.GetString(), szKeyName );
        }
      
        CheckCLSID( szKeyName );

		LOG_TEXT(_T("-----------------------------------------------------------------"))
		
        return;
    }

    // Enumerate completed or any error occurred, stop scanning.
    //

    StopJustMe();
}

/*************************************************************************
* Function Name:    CComClassRScanner::CheckCLSID()
*
* Function Purpose: Searches COM class IDs in HKCR\CLSID that referenced 
*                   inexistent file of TypeLib, or that have no values.
*
* Arguments:        szKeyName - Name of sub-key is need to be checked.
*
* Return value:     None
*                           
*************************************************************************/
BOOL CComClassRScanner::CheckCLSID( const TCHAR * szKeyName )
{

	LOG_TEXT3(_T("Checking CLSID for "),szKeyName,_T(""))

	CHECK_RESULT Res = C_SUCCESS;

    _ASSERT( m_hScanningKey );

    HKEY hOpenKey = NULL;
    LONG lResult = RegOpenKeyEx(
                            m_hScanningKey,
                            szKeyName,
                            0,
                            KEY_READ,
                            &hOpenKey );
    
    if( ERROR_SUCCESS != lResult )
    {
		LOG_TEXT3(_T("RegOpenKeyEx for "),szKeyName,_T( " was failed!"))
		
        return TRUE;
    }

	size_t Size = Checkers.size();

	ce_tstring strScanObject(_T( "HKEY_CLASSES_ROOT\\" ));
	strScanObject += CLSID_KEY;
	strScanObject += _T( "\\" );
	strScanObject += szKeyName;

	BOOL ScanSuccess = FALSE;

	for (unsigned int i = 0; i < Size; i++)
	{
		Res = Checkers[i]->Check( m_hScanningKey, szKeyName);
		if ( C_SKIP == Res )
		{
			return TRUE;
		}

		if ( C_TREAT == Res )
		{
			LOG_TEXT3(_T("Checker for "),szKeyName,_T(" was succeeded! This is TREAT!"))

			RecursivelyAddKey( hOpenKey, strScanObject );
			RegCloseKey( hOpenKey );

			return FALSE;
		}
		if ( C_SUCCESS == Res )
		{
			ScanSuccess = TRUE;
		}
	}

	if ( !ScanSuccess )
	{
		LOG_TEXT3(_T("Checker for "),szKeyName,_T(" was failed for all checkers! This is TREAT!"))

		RecursivelyAddKey( hOpenKey, strScanObject );
		RegCloseKey( hOpenKey );

		return FALSE;
	}
    
 //   ce_tstring strScanObject(_T( "HKEY_CLASSES_ROOT\\" ));
 //   strScanObject += CLSID_KEY;
 //   strScanObject += _T( "\\" );
 //   strScanObject += szKeyName;

 //   if( IsKeyEmpty( hOpenKey ) )
 //   {
	//	LOG_TEXT3(_T("IsKeyEmpty for "),szKeyName,_T(" was succeeded! This is TREAT!"))
	//	
 //       RecursivelyAddKey( hOpenKey, strScanObject );
 //       RegCloseKey( hOpenKey );
 //       return FALSE;
 //   }
 //   
	//CHECKITEM_RESULT chkInprocServer32 = CheckInProcServer32( hOpenKey );

	//#ifdef _DEBUG
	//if (ITEM_OK != chkInprocServer32)
	//{
	//	LOG_TEXT3(_T("CheckInProcServer32 for "),strScanObject,_T( " was failed!"))
	//}
	//#endif


 //   CHECKITEM_RESULT chkLocalServer32  = CheckLocalServer32( hOpenKey );

	//#ifdef _DEBUG
	//if (ITEM_OK != chkLocalServer32)
	//{
	//	LOG_TEXT3(_T("CheckLocalServer32 for "),strScanObject,_T( " was failed!"))
	//}
	//#endif

 //   if(( ITEM_INCORRECT == chkInprocServer32 ) || 
 //      ( ITEM_INCORRECT == chkLocalServer32 ) || 
	//   ( CHECK_ERROR == chkInprocServer32 && CHECK_ERROR == chkLocalServer32))
 //   {

	//	LOG_TEXT3(_T("CLSID "),strScanObject, _T( " is TREAT!"))
	//	  
 //       RecursivelyAddKey( hOpenKey, strScanObject );
 //       RegCloseKey( hOpenKey );
 //       return FALSE;
 //   }

 //   // TODO: check AppId, ProgId, TypeLib

	LOG_TEXT3(_T("CLSID "),strScanObject, _T( " is OK!"))

    RegCloseKey( hOpenKey );

	return TRUE;
}

/*************************************************************************
* Function Name:    CComClassRScanner::CheckLocalServer32
*
* Function Purpose: Check file existing referenced from 'LocalServer32'
*                   sub-key.
*
* Arguments:        hKey - Handle to an opened CLSID key.
* Return value:     ITEM_OK or error code defined in CHECK_RESULT enum.                           
*************************************************************************/



//CHECKITEM_RESULT CComClassRScanner::CheckLocalServer32( const HKEY hKey )
//{
//    return CheckServer( hKey, _T( "LocalServer32" ));
//}




/*************************************************************************
* Function Name:    CComClassRScanner::CheckInProcServer32
*
* Function Purpose: Check file existing referenced from 'InprocServer32'
*                   sub-key.
*
* Arguments:        hKey - Handle to an opened CLSID key.
* Return value:     ITEM_OK or error code defined in CHECK_RESULT enum.
*                           
*************************************************************************/



//CHECKITEM_RESULT CComClassRScanner::CheckInProcServer32( const HKEY hKey )
//{
//    return CheckServer( hKey, _T( "InProcServer32" ));
//}



/*************************************************************************
* Function Name:    CComClassRScanner::CheckServer()
*
* Function Purpose: Check file existing referenced from szSubKey.
*                   szSubKey is either InprocServer(32) or LocalServer(32)
*                   sub-key.
*
* Arguments:        hKey - Handle to an opened CLSID key.
*                   szServer - SubKey name is need to be checked.
*
* Return value:     ITEM_OK or error code defined in CHECK_RESULT enum.
*                           
*************************************************************************/




//CHECKITEM_RESULT CComClassRScanner::CheckServer( 
//            const HKEY hKey, const TCHAR * szServer )
//{
//    _ASSERT( hKey );
//    
//    HKEY hOpenKey = NULL;
//    LONG lResult = RegOpenKeyEx(
//                            hKey,
//                            szServer,
//                            0,
//                            KEY_READ,
//                            &hOpenKey );
//
//    if( ERROR_SUCCESS != lResult )
//    {
//		return CHECK_ERROR;
//    }
//    
//    countable< TCHAR > spDataBuffer;
//    DWORD dwType, dwDataSize = 0;
//
//    lResult = RegReadValue(
//                            hOpenKey,
//                            NULL,
//                            &dwType,
//                            spDataBuffer,
//                            dwDataSize );
//
//    RegCloseKey( hOpenKey );
//
//    if( ERROR_SUCCESS != lResult )
//    {
//		LOG_TEXT(_T("RegReadValue for default value was failed!"))
//	
//        return CHECK_ERROR;
//    }
//    
//	if( m_pNotifier )
//	{
//		m_pNotifier->CallOnScan( ScannerId(), NULL, spDataBuffer.get() );
//	}
//    if ( !CheckPaths( spDataBuffer ) )
//    {
//		LOG_TEXT3(_T("CheckPaths for file "),spDataBuffer.get(),_T(" failed!"))
//		
//		return ITEM_INCORRECT;
//        
//	}
//	else
//	{
//		LOG_TEXT3(_T("File "),spDataBuffer.get(),_T(" present on the system"))
//	
//		if ( IsFakePath( spDataBuffer.get() ) )
//		{
//			LOG_TEXT3(_T("File path "),spDataBuffer.get(),_T(" is fake path"))
//			
//			return ITEM_INCORRECT;
//		}
//	}
//
//    return ITEM_OK;
//}

/*************************************************************************
* Function Name:    CComClassRScanner::postop()
*
* Function Purpose: Called by CRunnable after run() function completed. 
*                   Perform correctly thread completion (close opened 
*                   handles etc.)
*
* Arguments:        None
* Return value:     None
*                           
*************************************************************************/
void CComClassRScanner::postop( void )
{
    CRegistryScanner::postop();

    if( m_hScanningKey )
    {
        RegCloseKey( m_hScanningKey );
        m_hScanningKey = NULL;
    }
}

