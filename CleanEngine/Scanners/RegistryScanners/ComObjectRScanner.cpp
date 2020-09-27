#include "StdAfx.h"
#include "ComObjectRScanner.h"
#include <atlpath.h>
#include <algorithm>
#include "CleanEngine.h"

static const TCHAR * CLSID_KEY = _T( "CLSID" );
static const TCHAR * INTERFACE_KEY = _T( "Interface" );
static const TCHAR * TYPELIB_KEY = _T( "TypeLib" );


/*************************************************************************
* Function Name:        CComClassRScanner::CComClassRScanner
* Function Purpose:     Constructor  
*************************************************************************/

CComObjectRScanner::CComObjectRScanner( CCleanEngine * pEngine, INotifier * pNotifier /* = NULL */, IScanProfiler * pProfiler /* = NULL */ ) :
CRegistryScanner( pEngine, pNotifier, pProfiler ),
CCleanScanner( pEngine, pNotifier, pProfiler ),
m_pfnAlgoSelector( NULL ),
m_hScanningKey( NULL ),
m_dwIndex( 0L ),
m_dwStep( 0L ),
m_dwTop( 2L )
{
	Checkers.clear();
	Checkers.reserve( 14 );

	IComServerChecker* pChecker;

	pChecker = new CEmptyKeyChecker( this, pEngine );
	Checkers.push_back( pChecker );

	pChecker = new CNetSkipper( this, pEngine );
	Checkers.push_back( pChecker );

	pChecker = new CInprocServerChecker( this, pEngine );
	Checkers.push_back( pChecker );

	pChecker = new CLocalServerChecker( this, pEngine );
	Checkers.push_back( pChecker );

	pChecker = new CInprocHandlerChecker( this, pEngine );
	Checkers.push_back( pChecker );

	pChecker = new CProgIDChecker( this, pEngine );
	Checkers.push_back( pChecker );

	pChecker = new CAppIDChecker( this, pEngine );
	Checkers.push_back( pChecker );

	pChecker = new CTypeLibChecker( this, pEngine );
	Checkers.push_back( pChecker );

	pChecker = new CAutoConvertToChecker( this, pEngine );
	Checkers.push_back( pChecker );

	pChecker = new CAutoTreatAsChecker( this, pEngine );
	Checkers.push_back( pChecker );

	pChecker = new CTreatAsChecker( this, pEngine );
	Checkers.push_back( pChecker );

	pChecker = new CPersistentHandlerChecker( this, pEngine );
	Checkers.push_back( pChecker );

	pChecker = new CPersistentAddinsRegisteredChecker( this, pEngine );
	Checkers.push_back( pChecker );

	pChecker = new CImplementedCategoriesChecker( this, pEngine );
	Checkers.push_back( pChecker );

	TreatCLSID.reserve(256);
	TreatTypeLib.reserve(100);
	CheckingNowCLSID.reserve(50);

	SubKeys.clear();

	SubKeys.insert( std::make_pair(0, _T( "" ) ) );
	SubKeys.insert( std::make_pair(1, _T( "InprocServer32" ) ) );
	SubKeys.insert( std::make_pair(2, _T( "InprocServer32" ) ) );
	SubKeys.insert( std::make_pair(3, _T( "LocalServer32" ) ) );
	SubKeys.insert( std::make_pair(4, _T( "InprocHandler32" ) ) );
	SubKeys.insert( std::make_pair(5, _T( "ProgID" ) ) );
	SubKeys.insert( std::make_pair(6, _T( "" ) ) );
	SubKeys.insert( std::make_pair(7, _T( "TypeLib" ) ) );
	SubKeys.insert( std::make_pair(8, _T( "AutoConvertTo" ) ) );
	SubKeys.insert( std::make_pair(9, _T( "AutoTreatAs" ) ) );
	SubKeys.insert( std::make_pair(10, _T( "TreatAs" ) ) );
	SubKeys.insert( std::make_pair(11, _T( "PersistentHandler" ) ) );
	SubKeys.insert( std::make_pair(12, _T( "PersistentAddinsRegistered" ) ) );
	SubKeys.insert( std::make_pair(13, _T( "ImplementedCategories" ) ) );

	Roots.clear();
	Roots.reserve(3);

	Roots.push_back( _T( "HKEY_CURRENT_USER\\Software\\Classes\\" ) );
	Roots.push_back( _T( "HKEY_CLASSES_ROOT\\" ) );
	Roots.push_back( _T( "HKEY_LOCAL_MACHINE\\Software\\Classes\\" ) );
}

/*************************************************************************
* Function Name:          CComClassRScanner::~CComClassRScanner
* Function Purpose:       Destructor
* Function Description: 
*************************************************************************/
CComObjectRScanner::~CComObjectRScanner()
{
	/*for ( unsigned int i = 0; i < Checkers.size(); i++)
	{
		delete Checkers[i];
	}
	Checkers.clear();
	TreatCLSID.clear();
	TreatTypeLib.clear();
	Roots.clear();
	SubKeys.clear();
	CheckingNowCLSID.clear();*/
}
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
void CComObjectRScanner::postop( void )
{
	CRegistryScanner::postop();

	if( m_hScanningKey )
	{
		RegCloseKey( m_hScanningKey );
		m_hScanningKey = NULL;
	}
}

/*************************************************************************
* Function Name:          CComClassRScanner::ReinitAlgorithm
* Function Purpose:       
* Function Description: 
*************************************************************************/
void CComObjectRScanner::ReinitAlgorithm()
{
	m_pfnAlgoSelector = &CComObjectRScanner::algoStep1;
	TreatCLSID.clear();
	TreatCLSID.reserve(256);
	TreatTypeLib.clear();
	TreatTypeLib.reserve(100);
	CheckingNowCLSID.clear();
	CheckingNowCLSID.reserve(50);
	m_dwIndex = 0;
	m_dwStep = 0;
	m_hScanningKey = NULL;
}

/*************************************************************************
* Function Name:          CComClassRScanner::algorithm()
* Function Purpose:       
* Function Description: 
*************************************************************************/
void CComObjectRScanner::algorithm()
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
void CComObjectRScanner::algoStep1()
{
	if ( m_pProfiler )
	{
		m_pProfiler->GetDWORD( ID_REG_SCANNER_TYPE, m_dwTop );
	}
	else
	{
		m_dwTop = 2;
	}
	
	CString strSubKey( Roots[ m_dwStep ] );
	int i = strSubKey.Find( '\\' );
	strSubKey = strSubKey.Right( strSubKey.GetLength() - ( i + 1 ) );
	switch ( m_dwStep )
	{
	case 0:
		{
			LONG lResult = RegOpenKeyEx(
				HKEY_CURRENT_USER, 
				strSubKey,
				0, 
				KEY_READ|KEY_WRITE| DELETE,
				&m_hRootKey);

			if( ERROR_SUCCESS != lResult )
			{
				if ( m_dwStep < m_dwTop)
				{
					m_dwStep++;
				}
				else
				{
					StopJustMe();
				}
				
				return;
			}
			break;
		}
	case 1:
		{
			m_hRootKey = HKEY_CLASSES_ROOT;
			break;
		}
	case 2:
		{
			LONG lResult = RegOpenKeyEx(
				HKEY_LOCAL_MACHINE, 
				strSubKey,
				0, 
				KEY_READ|KEY_WRITE| DELETE,
				&m_hRootKey);

			if ( ERROR_SUCCESS != lResult)
			{
				StopJustMe();
				return;
			}
			break;
		}
	}

	if ( m_hScanningKey != NULL)
	{
		RegCloseKey( m_hScanningKey );
		m_hScanningKey = NULL;
	}

	LONG lResult = RegOpenKeyEx(
		m_hRootKey, 
		TYPELIB_KEY,
		0, 
		KEY_READ|KEY_WRITE| DELETE,
		&m_hScanningKey);

	if( ERROR_SUCCESS == lResult )
	{
		// Can't open required key, stop scanning
		//
		m_dwIndex = 0;
		m_pfnAlgoSelector = &CComObjectRScanner::algoStep2;

		return;
	}

	lResult = RegOpenKeyEx(
		m_hRootKey, 
		CLSID_KEY,
		0, 
		KEY_READ|KEY_WRITE| DELETE,
		&m_hScanningKey);

	if( ERROR_SUCCESS == lResult )
	{
		// Can't open required key, stop scanning
		//
		m_dwIndex = 0;
		m_pfnAlgoSelector = &CComObjectRScanner::algoStep3;

		return;
	}

	m_dwIndex = 0;
	m_pfnAlgoSelector = &CComObjectRScanner::algoStep4;

	//StopJustMe();   
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
void CComObjectRScanner::algoStep3()
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
			ce_tstring strScanObject( Roots[ m_dwStep ] );
			strScanObject += CLSID_KEY;

			m_pNotifier->CallOnScan( ScannerId(), strScanObject.GetString(), szKeyName );
		}

		CheckCLSID( szKeyName, Roots[ m_dwStep ], TRUE );

		LOG_TEXT(_T("-----------------------------------------------------------------"))

			return;
	}

	// Enumerate completed or any error occurred, stop scanning.
	//

	m_dwIndex = 0;
	m_pfnAlgoSelector = &CComObjectRScanner::algoStep4;

	//StopJustMe();
}

/*************************************************************************
* Function Name:        CComClassRScanner::algoStep3
*
* Function Purpose:     Searches COM class IDs that referenced inexistent 
*                       file of TypeLib, or that have no values.
*
* Arguments:            None
* Return value:         None
*                           
*************************************************************************/
void CComObjectRScanner::algoStep4()
{
	TCHAR  szKeyName[ MAX_KEY_SIZE ];
	DWORD  dwSize = sizeof( szKeyName ) / sizeof( TCHAR );

	LONG lResult = RegEnumKeyEx(
		m_hRootKey,
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
			if (_tcsicmp( szKeyName, _T("CLSID") ) == 0)
			{
				return;
			}

			if (_tcsicmp( szKeyName, _T("Interface") ) == 0)
			{
				return;
			}
			if (_tcsicmp( szKeyName, _T("TypeLib") ) == 0)
			{
				return;
			}
			ce_tstring strScanObject( Roots[ m_dwStep ] );
			strScanObject += szKeyName;

			if( m_pNotifier )
			{
				m_pNotifier->CallOnScan( ScannerId(), strScanObject.GetString() );
			}

			// Add empty keys also.
			//
			CheckProgID( szKeyName, Roots[ m_dwStep ], TRUE );
			
			LOG_TEXT(_T("-----------------------------------------------------------------"))
		}


		return;
	}

	RegCloseKey(m_hScanningKey);
	m_hScanningKey = NULL;

	// Setting function of next step( Intreface check )
	//

	//if ( 2 == m_dwStep)
	//{
	//	StopJustMe();
	//	return;
	//}

	lResult = RegOpenKeyEx(
		m_hRootKey,
		INTERFACE_KEY,
		0,
		KEY_READ|KEY_WRITE| DELETE | STANDARD_RIGHTS_ALL,
		&m_hScanningKey );

	if( ERROR_SUCCESS != lResult )
	{
		// Can't open required key, stop scanning
		//

		m_dwIndex = 0;
		m_pfnAlgoSelector = &CComObjectRScanner::algoStep1;

		if ( m_dwTop == m_dwStep )
		{
			StopJustMe();   
		}
		else
		{
			m_dwStep++;
		}
	
		return;
	}

		m_dwIndex = 0;
		m_pfnAlgoSelector = &CComObjectRScanner::algoStep5;
}

/*************************************************************************
* Function Name:        CComClassRScanner::algoStep4
*
* Function Purpose:     Searches COM class IDs that referenced inexistent 
*                       file of TypeLib, or that have no values.
*
* Arguments:            None
* Return value:         None
*                           
*************************************************************************/

void CComObjectRScanner::algoStep2()
{
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

		ce_tstring strScanObject( Roots[ m_dwStep ] );
		strScanObject += TYPELIB_KEY;
		strScanObject += _T( "\\" );
		strScanObject += szKeyName;

		// Notify user about current scanning object.
		//
		if( m_pNotifier )
		{
			m_pNotifier->CallOnScan( ScannerId(), strScanObject.GetString() );
		}

		if( ERROR_SUCCESS == RegOpenKeyEx(
			m_hScanningKey,
			szKeyName,
			0,
			KEY_READ|KEY_WRITE| DELETE /*| STANDARD_RIGHTS_ALL*/,
			&hOpenKey ) )
		{
			if( IsKeyEmpty( hOpenKey ) )
			{
				LOG_TEXT3(_T("IsKeyEmpty for "),szKeyName,_T( " was failed! This is TREAT!"))

				RecursivelyAddKey( hOpenKey, strScanObject );
				RegCloseKey( hOpenKey );

				TreatTypeLib.push_back( strScanObject );
				return;
			}

			CheckTypeLib( hOpenKey, ( TCHAR* )strScanObject.GetString() );
			RegCloseKey( hOpenKey );
			LOG_TEXT(_T("-----------------------------------------------------------------"))
		}

		return;
	}

	RegCloseKey(m_hScanningKey);
	m_hScanningKey = NULL;

	lResult = RegOpenKeyEx(
		m_hRootKey,
		CLSID_KEY,
		0,
		KEY_READ|KEY_WRITE| DELETE /*| STANDARD_RIGHTS_ALL*/,
		&m_hScanningKey );

	if( ERROR_SUCCESS != lResult )
	{
		// Can't open required key, stop scanning
		//

		m_dwIndex = 0;
		m_pfnAlgoSelector = &CComObjectRScanner::algoStep4; 
		return;
	}

	m_dwIndex = 0;
	m_pfnAlgoSelector = &CComObjectRScanner::algoStep3;

}

/*************************************************************************
* Function Name:        CComClassRScanner::algoStep5
*
* Function Purpose:     Searches COM class IDs that referenced inexistent 
*                       file of TypeLib, or that have no values.
*
* Arguments:            None
* Return value:         None
*                           
*************************************************************************/

void CComObjectRScanner::algoStep5()
{
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
		// Notify user about current scanning object
		//
		if( m_pNotifier )
		{
			ce_tstring strScanObject( Roots[ m_dwStep ] );
			strScanObject += INTERFACE_KEY;

			m_pNotifier->CallOnScan( ScannerId(), strScanObject.GetString(), szKeyName );
		}

		CheckInterface( szKeyName,  Roots[ m_dwStep ] );
		LOG_TEXT(_T("-----------------------------------------------------------------"))
			return;
	}


	// Enumerate completed or any error occurred, stop scanning.
	//

	if ( m_dwTop == m_dwStep )
	{
		StopJustMe();
	}
	else
	{
		m_dwStep++;
		m_dwIndex = 0;
		m_pfnAlgoSelector = &CComObjectRScanner::algoStep1;
	}
	
}
/*************************************************************************
* Function Name:    CComObjectRScanner::CheckCLSID()
*
* Function Purpose: Searches COM class IDs in HKCR\CLSID that referenced 
*                   inexistent file of TypeLib, or that have no values.
*
* Arguments:        szKeyName - Name of sub-key is need to be checked.
*
* Return value:     None
*                           
*************************************************************************/
BOOL CComObjectRScanner::CheckCLSID( const TCHAR * szKeyName, const TCHAR * szRoot, BOOL bAddKeys )
{

	ce_tstring strScanObject( szRoot );
	strScanObject += CLSID_KEY;
	strScanObject += _T( "\\" );
	strScanObject += szKeyName;

	LOG_TEXT3(_T("Checking CLSID for "),/*szKeyName/**/strScanObject,_T(""))

	if( CLSIDIsCheckingNow( strScanObject ) )
	{
		return TRUE;
	}

	CheckingNowCLSID.push_back( strScanObject );

	CHECK_RESULT Res[14];

	_ASSERT( m_hScanningKey );

	if ( CLSIDIsInReport( szKeyName, szRoot ) )
	{
		CheckingNowCLSID.pop_back();
		return FALSE;
	}

	HKEY hOpenKey = NULL;
	LONG lResult = RegOpenKeyEx(
		m_hScanningKey,
		szKeyName,
		0,
		KEY_READ|KEY_WRITE| DELETE | STANDARD_RIGHTS_ALL,
		&hOpenKey );

	if( ERROR_SUCCESS != lResult )
	{
		LOG_TEXT3(_T("RegOpenKeyEx for "),/*szKeyName/**/ strScanObject,_T( " was failed!"))

		CheckingNowCLSID.pop_back();
		return TRUE;
	}

	size_t Size = Checkers.size();

	BOOL ScanSuccess = FALSE;
	BOOL TreatFound = FALSE;


	for (unsigned int i = 0; i < Size; i++)
	{
		Res[i] = Checkers[i]->Check( m_hScanningKey, szKeyName, m_hRootKey, szRoot );
		if ( C_SKIP == Res[i] )
		{
			CheckingNowCLSID.pop_back();

			RegCloseKey( hOpenKey );

			return TRUE;
		}

		if ( C_TREAT == Res[i] )
		{
			TreatFound = TRUE;
		}

		if ( C_SUCCESS == Res[i] )
		{
			ScanSuccess = TRUE;
		}
	}

	if ( !bAddKeys )
	{
		RegCloseKey( hOpenKey );
		CheckingNowCLSID.pop_back();
		return ScanSuccess || !TreatFound;
	}

	if ( !ScanSuccess )
	{
		if ( TreatFound )
		{
			LOG_TEXT3(_T("Checker for "),szKeyName,_T(" was failed for all known subkeys! This is TREAT!"))

			RecursivelyAddKey( hOpenKey, strScanObject );
			RegCloseKey( hOpenKey );

			//CString str( szKeyName );
			TreatCLSID.push_back( strScanObject );

			CheckingNowCLSID.pop_back();

			return FALSE;
		}
		else
		{
			LOG_TEXT3(_T("CLSID "),strScanObject, _T( " is OK!"))
			
			CheckingNowCLSID.pop_back();

			RegCloseKey( hOpenKey );

			return TRUE;
		}
	}
	else
	{
		if ( TreatFound )
		{
			if( ( C_TREAT == Res[0] ) /*|| ( ( C_TREAT == Res[2] ) && ( C_SUCCESS != Res[3] ) )/**/ )
			{
				LOG_TEXT3(_T("Checker for "),szKeyName,_T(" was failed! This is TREAT!"))

				RecursivelyAddKey( hOpenKey, strScanObject );
				RegCloseKey( hOpenKey );

				//CString str( szKeyName );
				TreatCLSID.push_back( strScanObject );

				CheckingNowCLSID.pop_back();

				return FALSE;
			}

			for (unsigned int i = 1; i < Checkers.size(); i++)
			{
				if ( 6 == i)
				{
					continue;
				}
				HKEY hSubKey;
				if ( C_TREAT == Res[i])
				{

					LONG lResult = RegOpenKeyEx(
					hOpenKey,
					SubKeys[i],
					0,
					KEY_READ|KEY_WRITE| DELETE | STANDARD_RIGHTS_ALL,
					&hSubKey );

					if( ERROR_SUCCESS != lResult )
					{
						LOG_TEXT3(_T("RegOpenKeyEx for "),SubKeys[i],_T( " was failed!"))

						continue;
					}

					ce_tstring strSubScanObject(strScanObject);
					strSubScanObject += _T( "\\" );
					strSubScanObject += SubKeys[i];

					RecursivelyAddKey( hSubKey, strSubScanObject );
					RegCloseKey( hSubKey );
				}

			}

			LOG_TEXT3(_T("CLSID check for "),strScanObject, _T( "completed successfully but with some TREAT SUBKEYS. CLSID is OK!"))

			RegCloseKey( hOpenKey );

			CheckingNowCLSID.pop_back();
			
			return TRUE;
		}
		else
		{
			LOG_TEXT3(_T("CLSID "),strScanObject, _T( " is OK!"))

			RegCloseKey( hOpenKey );

			CheckingNowCLSID.pop_back();
			
			return TRUE;
		}
	}

	//   // TODO: check AppId, ProgId, TypeLib

	LOG_TEXT3(_T("CLSID "),strScanObject, _T( " is OK!"))

	RegCloseKey( hOpenKey );

	CheckingNowCLSID.pop_back();
	

	return TRUE;
}

/*************************************************************************
* Function Name:    CComObjectRScanner::CheckTypeLib()
* Function Purpose: Check TypeLib file existing for HKCR\TypeLib\<TypeLib>
*
* Arguments:        hKey - Handle to a current open key.
*
*                   szKeyName - Pointer to a null-terminated string 
*                   containing the full name of the opened key.
*                   
* Return value:     None
*                           
*************************************************************************/
void CComObjectRScanner::CheckTypeLib( HKEY hKey, const TCHAR * szKeyName )
{
	_ASSERT( hKey );

	TCHAR szSubKey[ MAX_KEY_SIZE ];
	DWORD dwIndex = 0;
	LONG lResult;

	bool bGoodKeyFound = false;
	std::vector< countable< ce_tstring > >  vBadKeys;

	LOG_TEXT3( _T("Checking TypeLib for "),szKeyName,_T(""))

	if( TypeLibIsInReport( szKeyName ) )
	{
		return;
	}

	while( 1 ) 
	{
		DWORD dwNameSize = sizeof( szSubKey ) / sizeof( TCHAR );

		lResult = RegEnumKeyEx( hKey,
			dwIndex++,
			szSubKey,
			&dwNameSize,
			NULL,
			NULL,
			NULL,
			NULL );

		if( ERROR_SUCCESS != lResult )
		{
			break;
		}

		// Assume every key contains only one path :-/
		//
		switch( CheckKeyForPath( hKey, szSubKey ) )
		{

		case NO_PATH:
			{
				// SubKey doesn't contain path.
				//
				LOG_TEXT(_T("SubKey doesn't contain path"))
			}
			break;

		case CORRECT_PATH:
			{
				// SubKey contain path and this path exist.
				//
				LOG_TEXT3(_T("SubKey "),szSubKey,_T(" contain path and this path exist"))
					bGoodKeyFound = true;
			}
			break;

		case INVALID_PATH:
			{
				// SubKey contain path and path is invalid.
				//

				LOG_TEXT3(_T("CheckKeyForPath for "),szSubKey,_T(" contain path and path is invalid"))

				countable< ce_tstring > spBadKey( new ce_tstring( szSubKey ));
				if( !spBadKey.get() )
				{
					return;
				}

				vBadKeys.push_back( spBadKey );
			}
			break;

			// We should never reach this code!
			//
		default:
			EXCEPTION( "Unhandled return value!" );
			break;
		}
	}

	if( vBadKeys.size() )
	{
			if( !bGoodKeyFound )
			{
				// There are no any good SubKeys found, add whole key.
				//
				LOG_TEXT3(_T("No good sub keys found for "),szKeyName, _T(" this is TREAT!"))

				ce_tstring strKeyName( szKeyName );
				RecursivelyAddKey( hKey, strKeyName );

				TreatTypeLib.push_back( strKeyName );

			}
			else
			{
				// Key contains SubKeys with valid paths, add bad keys only.
				//
			for( unsigned i = 0; i < vBadKeys.size(); i++ )
			{
					HKEY hOpenKey;
					countable< ce_tstring > spSubKey = vBadKeys[ i ];
					ce_tstring strKeyName( szKeyName );
					strKeyName += _T( "\\" );
					strKeyName += spSubKey.get()->GetString();

					if( ERROR_SUCCESS == RegOpenKeyEx( 
						hKey, 
						spSubKey.get()->GetString(), 
						0, 
						KEY_READ|KEY_WRITE| DELETE | STANDARD_RIGHTS_ALL, 
						&hOpenKey ))
					{

						LOG_TEXT3(_T("Adding subkey "),*spSubKey.get(),_T( " because of invalid path. This is TREAT!"))

						RecursivelyAddKey( hOpenKey, strKeyName );
						RegCloseKey( hOpenKey );

						/*CString str( strKeyName );
						GetCLSIDFromPath( CString( szKeyName ), str );
						TreatTypeLib.push_back(str);*/
					}
				}
			}
		}

}

/*************************************************************************
* Function Name:        CComObjectRScanner::CheckInterface()
*
* Function Purpose:     Searches COM class interfaces that referenced 
*                       inexistent GUIDs of TypeLib or ProxyStubClsid32.
*
* Arguments:            szKeyName - Zero-terminating string containing
*                       interface GUID that is need to be checked.
* Return value:         None                           
*************************************************************************/
void CComObjectRScanner::CheckInterface( const TCHAR * szKeyName, const TCHAR * szRoot )
{
	HKEY hOpenKey = NULL;
	LONG lResult;
	//BOOL ProxyCheckSuccess = FALSE;
	ce_tstring strKeyName( szRoot );
	strKeyName += INTERFACE_KEY;
	strKeyName += _T("\\");
	strKeyName += szKeyName;

	LOG_TEXT3( _T("CheckInterface for "),szKeyName,_T( " started!"))

		lResult = RegOpenKeyEx(
		m_hScanningKey, 
		szKeyName, 
		0, 
		KEY_READ|KEY_WRITE| DELETE, 
		&hOpenKey );

	if( ERROR_SUCCESS != lResult )
	{
		LOG_TEXT3( _T("RegOpenKeyEx for"),szKeyName,_T( "was failed!"))
			return;
	}

	if( IsKeyEmpty( hOpenKey ) )
	{

		LOG_TEXT3(_T("Key "),strKeyName,_T( " is empty! This is TREAT!"))

		RecursivelyAddKey( hOpenKey, strKeyName );
		RegCloseKey( hOpenKey );
		return;
	}

	CHECK_RESULT bHasBadProxyStub32 = C_FAILED, bHasBadTypeLib = C_FAILED,bHasBadProxyStub = C_FAILED;

	// Check 'ProxyStubClsid32' key.
	//
	LOG_TEXT2(_T("Check ProxyStubClsid32 of key "), szKeyName)

	HKEY hCheckedKey;
	lResult = RegOpenKeyEx( hOpenKey,
		_T("ProxyStubClsid32"),
		0,
		KEY_READ|KEY_WRITE| DELETE,
		&hCheckedKey );

	if( ERROR_SUCCESS == lResult )
	{
		countable< TCHAR > spDataBuffer;
		DWORD dwType        = REG_SZ;
		DWORD dwDataSize    = 0;

		lResult = RegReadValue(
			hCheckedKey,
			NULL,
			&dwType,
			spDataBuffer,
			dwDataSize );

		if( ERROR_SUCCESS == lResult )
		{
			//ce_tstring strKey = _T("CLSID\\");
			//strKey += spDataBuffer.get();

			ce_tstring strKey ( szRoot );
			strKey += CLSID_KEY;
			strKey += _T( "\\" );
			strKey += spDataBuffer.get();

			LOG_TEXT3(_T("Checking "),strKey,_T( " from ProxyStubClsid32 key"))

			//if( !IsKeyExist( HKEY_CLASSES_ROOT, const_cast<const LPTSTR>( strKey.GetString() ) ) || CLSIDIsInReport( spDataBuffer.get(), szRoot ) ) 
			if( !IsKeyExist( const_cast<const LPTSTR>( strKey.GetString() ) ) || CLSIDIsInReport( spDataBuffer.get(), szRoot ) ) 
			{
				//ce_tstring strKeyName( _T("HKEY_CLASSES_ROOT\\") );
				//strKeyName += INTERFACE_KEY;
				//strKeyName += _T( "\\" );
				//strKeyName += szKeyName;
				//strKeyName +=_T("\\ProxyStubClsid32");

				LOG_TEXT3(_T("IsKeyExists for "),strKey,_T( " was failed! This is TREAT!"))

				//RecursivelyAddKey( hCheckedKey, strKeyName );
				//RegCloseKey( hCheckedKey );

				bHasBadProxyStub32 = C_TREAT;

			}
			else
			{
				LOG_TEXT3(_T("CLSID "),strKey,_T( " is present"))

				//RegCloseKey( hCheckedKey );
				bHasBadProxyStub32 = C_SUCCESS;
			}

		}
		else
		{
			LOG_TEXT2(_T("ProxyStubClsid32 key is empty for key "), szKeyName)
			bHasBadProxyStub32 = C_TREAT;
		}

		RegCloseKey( hCheckedKey );
	}
	else
	{
		LOG_TEXT2(_T("ProxyStubClsid32 key is not present for key "), szKeyName)
		bHasBadProxyStub32 = C_FAILED;
	}

	// Check 'ProxyStubClsid' key.
	//
	LOG_TEXT2(_T("Check ProxyStubClsid of key "), szKeyName)

	
	lResult = RegOpenKeyEx( hOpenKey,
		_T("ProxyStubClsid"),
		0,
		KEY_READ|KEY_WRITE| DELETE,
		&hCheckedKey );

	if( ERROR_SUCCESS == lResult )
	{
		countable< TCHAR > spDataBuffer;
		DWORD dwType        = REG_SZ;
		DWORD dwDataSize    = 0;

		lResult = RegReadValue(
			hCheckedKey,
			NULL,
			&dwType,
			spDataBuffer,
			dwDataSize );

		if( ERROR_SUCCESS == lResult )
		{
			//ce_tstring strKey = _T("CLSID\\");
			//strKey += spDataBuffer.get();

			ce_tstring strKey ( szRoot );
			strKey += CLSID_KEY;
			strKey += _T( "\\" );
			strKey += spDataBuffer.get();

			LOG_TEXT3(_T("Checking "),strKey,_T( " from ProxyStubClsid key"))

			//if( !IsKeyExist( HKEY_CLASSES_ROOT, const_cast<const LPTSTR>( strKey.GetString() ) ) || CLSIDIsInReport( spDataBuffer.get(), szRoot ) ) 
			if( !IsKeyExist( const_cast<const LPTSTR>( strKey.GetString() ) ) || CLSIDIsInReport( spDataBuffer.get(), szRoot ) ) 
			{
				//ce_tstring strKeyName( _T("HKEY_CLASSES_ROOT\\") );
				//strKeyName += INTERFACE_KEY;
				//strKeyName += _T( "\\" );
				//strKeyName += szKeyName;
				//strKeyName +=_T("\\ProxyStubClsid32");

				LOG_TEXT3(_T("IsKeyExists for "),strKey,_T( " was failed! This is TREAT!"))

				//RecursivelyAddKey( hCheckedKey, strKeyName );
				//RegCloseKey( hCheckedKey );

				bHasBadProxyStub = C_TREAT;

			}
			else
			{
				LOG_TEXT3(_T("CLSID "),strKey,_T( " is present"))

				//RegCloseKey( hCheckedKey );
				bHasBadProxyStub = C_SUCCESS;
			}

		}
		else
		{
			LOG_TEXT2(_T("ProxyStubClsid key is empty for key "), szKeyName)
				bHasBadProxyStub = C_TREAT;
		}

		RegCloseKey( hCheckedKey );
	}
	else
	{
		LOG_TEXT2(_T("ProxyStubClsid key is not present for key "), szKeyName)
			bHasBadProxyStub = C_FAILED;
	}

	// Check 'TypeLib' key.
	//
	LOG_TEXT2(_T("Check TypeLib of key "), szKeyName)

	lResult = RegOpenKeyEx( hOpenKey,
		_T( "TypeLib" ),
		0,
		KEY_READ|KEY_WRITE| DELETE,
		&hCheckedKey );

	if( ERROR_SUCCESS == lResult )
	{
		countable< TCHAR > spDataBuffer;
		DWORD dwType        = REG_SZ;
		DWORD dwDataSize    = 0;

		lResult = RegReadValue(
			hCheckedKey,
			NULL,
			&dwType,
			spDataBuffer,
			dwDataSize );

		if( ERROR_SUCCESS == lResult )
		{
			//ce_tstring strKey = _T("TypeLib\\");
			//strKey += spDataBuffer.get();

			ce_tstring strKey ( szRoot );
			strKey += TYPELIB_KEY;
			strKey += _T( "\\" );
			strKey += spDataBuffer.get();

			LOG_TEXT3(_T("Checking "),strKey,_T( " from interface key"))

			//if( !IsKeyExist( HKEY_CLASSES_ROOT, const_cast<const LPTSTR>( strKey.GetString() ) ) || TypeLibIsInReport( strKey ) )
			if( !IsKeyExist( const_cast<const LPTSTR>( strKey.GetString() ) ) || TypeLibIsInReport( strKey ) )
			{
				//ce_tstring strKeyName( _T("HKEY_CLASSES_ROOT\\") );
				//strKeyName += INTERFACE_KEY;
				//strKeyName +=_T("\\");
				//strKeyName +=szKeyName;
				//strKeyName += _T("\\TypeLib");

				LOG_TEXT3(_T("IsKeyExist for "),strKey,_T( " was failed! This is TREAT!"))

				//RecursivelyAddKey( hCheckedKey, strKeyName );
				//RegCloseKey( hCheckedKey );
				bHasBadTypeLib = C_TREAT;
			}
			else
			{
				LOG_TEXT3(_T("TypeLib "),strKey,_T( " is present"))
				bHasBadTypeLib = C_SUCCESS;
			}
		}
		else
		{
			bHasBadTypeLib = C_SUCCESS;
		}
		RegCloseKey( hCheckedKey );
	}
	else
	{
		LOG_TEXT2(_T("TypeLib key is not present for key "), szKeyName)
		bHasBadTypeLib = C_FAILED;
	}

		//Check again for empty
	if( IsKeyEmpty( hOpenKey ) )
	{
		//ce_tstring strKeyName( szRoot );
		//strKeyName += INTERFACE_KEY;
		//strKeyName += _T("\\");
		//strKeyName += szKeyName;

		LOG_TEXT3(_T("Key "),strKeyName,_T( " is empty! This is TREAT!"))

		RecursivelyAddKey( hOpenKey, strKeyName, SEVERITY_MEDIUM,20 );
		RegCloseKey( hOpenKey );
		return;
	}

	if( ( C_SUCCESS == bHasBadProxyStub && C_SUCCESS == bHasBadTypeLib && C_SUCCESS == bHasBadProxyStub32 ) || 
		( C_FAILED == bHasBadProxyStub && C_FAILED == bHasBadTypeLib && C_FAILED == bHasBadProxyStub32 ) )
	{
		if( hOpenKey )
		{
			RegCloseKey( hOpenKey );
		}

		LOG_TEXT3(_T("CheckInterface for "),szKeyName,_T( " is OK!"));
		return;
	}

	/*
	( C_TREAT == bHasBadProxyStub && C_TREAT == bHasBadTypeLib ) ||
	( C_TREAT == bHasBadProxyStub && C_FAILED == bHasBadTypeLib ) ||
	( C_FAILED == bHasBadProxyStub && C_TREAT == bHasBadTypeLib )
	/**/

	if( ( C_SUCCESS != bHasBadProxyStub32 ) && ( C_SUCCESS != bHasBadProxyStub ) && ( C_SUCCESS != bHasBadTypeLib) )
	{
		//ce_tstring strKeyName( _T("HKEY_CLASSES_ROOT\\") );
		//strKeyName += INTERFACE_KEY;
		//strKeyName += _T("\\");
		//strKeyName += szKeyName;

		LOG_TEXT3(_T("Key "),strKeyName,_T( " is empty! This is TREAT!"))

		RecursivelyAddKey( hOpenKey, strKeyName );
		RegCloseKey( hOpenKey );
		return;
	}

	if ( C_TREAT == bHasBadProxyStub )
	{
		lResult = RegOpenKeyEx( hOpenKey,
			_T("ProxyStubClsid"),
			0,
			KEY_READ|KEY_WRITE| DELETE,
			&hCheckedKey );

		if ( ERROR_SUCCESS == lResult)
		{
			ce_tstring strKeyName1( strKeyName );
			strKeyName1 +=_T("\\ProxyStubClsid");

			RecursivelyAddKey( hCheckedKey, strKeyName1 );
			RegCloseKey( hCheckedKey );
		}
	}

	if ( C_TREAT == bHasBadProxyStub32 )
	{
		lResult = RegOpenKeyEx( hOpenKey,
			_T("ProxyStubClsid32"),
			0,
			KEY_READ|KEY_WRITE| DELETE,
			&hCheckedKey );

		if ( ERROR_SUCCESS == lResult)
		{
			ce_tstring strKeyName1( strKeyName );
			//strKeyName += INTERFACE_KEY;
			//strKeyName += _T( "\\" );
			//strKeyName += szKeyName;
			strKeyName1 +=_T("\\ProxyStubClsid32");

			RecursivelyAddKey( hCheckedKey, strKeyName1 );
			RegCloseKey( hCheckedKey );
		}
	}

	if ( C_TREAT == bHasBadTypeLib )
	{
		lResult = RegOpenKeyEx( hOpenKey,
			_T("TypeLib"),
			0,
			KEY_READ|KEY_WRITE| DELETE,
			&hCheckedKey );

		if ( ERROR_SUCCESS == lResult)
		{
			ce_tstring strKeyName1( strKeyName );
			//strKeyName += INTERFACE_KEY;
			//strKeyName += _T( "\\" );
			//strKeyName += szKeyName;
			strKeyName1 +=_T("\\TypeLib");

			RecursivelyAddKey( hCheckedKey, strKeyName1 );
			RegCloseKey( hCheckedKey );
		}
	}

	if( hOpenKey )
	{
		RegCloseKey( hOpenKey );
	}

	LOG_TEXT3(_T("CheckInterface for "),strKeyName,_T( " has some treat subkeys!"))
}

/*
*
*
*
*/
BOOL CComObjectRScanner::CheckProgID( const TCHAR * szKeyName, const TCHAR * szRoot, BOOL bAddKeys )
{
	ce_tstring strScanObject( szRoot );
	strScanObject += szKeyName;
	HKEY hOpenKey = NULL;
	HKEY hSubKey = NULL;
	LONG lResult = ERROR_SUCCESS;
	CHECK_RESULT CurVerCheck = C_SUCCESS, CLSIDCheck = C_SUCCESS, ProtocolCheck = C_SUCCESS;

	if (CLSIDIsCheckingNow(strScanObject))
	{
		return TRUE;
	}

	CheckingNowCLSID.push_back(strScanObject);

	lResult = RegOpenKeyEx(
		m_hRootKey,
		szKeyName,
		0,
		KEY_READ|KEY_WRITE| DELETE | STANDARD_RIGHTS_ALL,
		&hOpenKey );

	if( ERROR_SUCCESS == lResult )
	{
		LOG_TEXT3(_T("Checking key "),szKeyName,_T( " for empty!"))

			if( IsKeyEmpty( hOpenKey ) )
			{
				LOG_TEXT3( _T("Key "),szKeyName, _T( " is empty! This is OK!"))
				//RecursivelyAddKey( hOpenKey, strScanObject );
				//RegCloseKey( hOpenKey );
				//CheckingNowCLSID.pop_back();
				//return FALSE;
				CheckingNowCLSID.pop_back();
				RegCloseKey( hOpenKey );
				return TRUE;
			}
			else
			{
				LOG_TEXT3( _T("Key "),szKeyName, _T( " is not empty!"))
			}

			//RegCloseKey( hOpenKey );
	}
	else
	{
		LOG_TEXT(_T("Can't open comprog info key")) 

		CheckingNowCLSID.pop_back();
		return FALSE;
	}

	countable< TCHAR > spDataBuffer;
	DWORD dwType, dwDataSize = 0;

	lResult = RegOpenKeyEx(
		hOpenKey,
		_T( "CurVer" ),
		0,
		KEY_READ|KEY_WRITE| DELETE | STANDARD_RIGHTS_ALL,
		&hSubKey );

	if ( ERROR_SUCCESS == lResult)
	{
		lResult = RegReadValue( 
			hSubKey,
			NULL,
			&dwType,
			spDataBuffer,
			dwDataSize);
		if ( ERROR_SUCCESS == lResult)
		{
			BOOL res = CheckProgID( spDataBuffer.get(), szRoot, FALSE );
			//if ( FALSE == res )
			//{
			//	RecursivelyAddKey( hOpenKey, strScanObject );
			//	RegCloseKey( hOpenKey );
			//	RegCloseKey( hSubKey );
			//}
			//CheckingNowCLSID.pop_back();
			//return res;
			CurVerCheck = ( ( res ) ? C_SUCCESS : C_TREAT );
		}
		else
		{
			//RecursivelyAddKey( hOpenKey, strScanObject );
			//RegCloseKey( hOpenKey );
			//RegCloseKey( hSubKey );
			//CheckingNowCLSID.pop_back();
			//return FALSE;
			CurVerCheck = C_TREAT;
		}
		RegCloseKey( hSubKey );
	}
	else
	{
		CurVerCheck = C_FAILED;
	}

	ce_tstring strCheckedKey( strScanObject );
	strCheckedKey += _T( "\\CLSID" );

	LOG_TEXT2(_T("Try to check clsid for key "), strCheckedKey) 

	if( IsKeyExist( const_cast<const LPTSTR>( strCheckedKey.GetString() ) ) )
	{   
		//CheckCLSID( szKeyName );
		RegOpenKeyEx(
			hOpenKey,
			_T( "CLSID" ),
			0,
			KEY_READ|KEY_WRITE| DELETE | STANDARD_RIGHTS_ALL,
			&hSubKey );

		lResult = RegReadValue(
			hSubKey,
			NULL,
			&dwType,
			spDataBuffer,
			dwDataSize );

		if ( ERROR_SUCCESS == lResult)
		{

			if ( !CheckCLSID( spDataBuffer.get(), Roots[ m_dwStep ], FALSE ) )
			{
				//RecursivelyAddKey( hOpenKey, strScanObject );
				//RegCloseKey( hOpenKey );
				//CheckingNowCLSID.pop_back();
				//return FALSE;
				CLSIDCheck = C_TREAT;
			}
			else
			{
				CLSIDCheck = C_SUCCESS;
			}
		}
		else
		{
			lResult = RegReadValue(
				hSubKey,
				_T( "CLSID" ),
				&dwType,
				spDataBuffer,
				dwDataSize );

			if ( ERROR_SUCCESS == lResult)
			{

				if ( !CheckCLSID( spDataBuffer.get(), Roots[ m_dwStep ], FALSE ) )
				{
					//RecursivelyAddKey( hOpenKey, strScanObject );
					//RegCloseKey( hOpenKey );
					//CheckingNowCLSID.pop_back();
					//return FALSE;
					CLSIDCheck = C_TREAT;
				}
				else
				{
					CLSIDCheck = C_SUCCESS;
				}
			}
			else
			{
				//RecursivelyAddKey( hOpenKey, strScanObject );
				//RegCloseKey( hOpenKey );
				//CheckingNowCLSID.pop_back();
				//return FALSE;
				CLSIDCheck = C_TREAT;
			}
		}
		RegCloseKey( hSubKey );
	}
	else
	{
		CLSIDCheck = C_FAILED;
		//RegCloseKey( hOpenKey );
	}

	strCheckedKey = strScanObject;
	strCheckedKey += _T( "\\protocol\\StdFileEditing\\server" );

	if ( IsKeyExist( const_cast<const LPTSTR>( strCheckedKey.GetString() ) ) )
	{
		RegOpenKeyEx(
			hOpenKey,
			_T( "protocol\\StdFileEditing\\server" ),
			0,
			KEY_READ|KEY_WRITE| DELETE | STANDARD_RIGHTS_ALL,
			&hSubKey );

		lResult = RegReadValue(
			hSubKey,
			NULL,
			&dwType,
			spDataBuffer,
			dwDataSize );

		if ( ERROR_SUCCESS == lResult)
		{

			if ( !CheckPaths( spDataBuffer.get() ) )
			{
				//if ( C_FAILED == CLSIDCheck)
				//{
				//	RecursivelyAddKey( hOpenKey, strScanObject );
				//	RegCloseKey( hOpenKey );
				//	RegCloseKey( hSubKey );
				//	CheckingNowCLSID.pop_back();
				//	return FALSE;
				//}
				//else
				//{
				//	RecursivelyAddKey( hSubKey, strCheckedKey );
				//	RegCloseKey( hOpenKey );
				//	RegCloseKey( hSubKey );
				//	CheckingNowCLSID.pop_back();
				//	return TRUE;
				//}
				ProtocolCheck = C_TREAT;
			}
			else
			{
				//CheckingNowCLSID.pop_back();

				ProtocolCheck = C_SUCCESS;
				//return TRUE;
			}
		}
		else
		{
			//RecursivelyAddKey( hOpenKey, strScanObject );
			//RegCloseKey( hOpenKey );
			//RegCloseKey( hSubKey );
			//CheckingNowCLSID.pop_back();
			//return FALSE;

			ProtocolCheck = C_TREAT;
		}
		RegCloseKey( hSubKey );

	}
	else
	{
		ProtocolCheck = C_FAILED;
		//RegCloseKey( hOpenKey );
	}

	if ( ( ( C_SUCCESS == CurVerCheck ) && ( C_SUCCESS == CLSIDCheck ) && ( C_SUCCESS == ProtocolCheck ) ) ||
		 ( ( C_FAILED == CurVerCheck ) && ( C_FAILED == CLSIDCheck ) && ( C_FAILED == ProtocolCheck ) ) )
	{
		CheckingNowCLSID.pop_back();
		RegCloseKey( hOpenKey );
		return TRUE;
	}

	if ( !bAddKeys )
	{
		CheckingNowCLSID.pop_back();
		RegCloseKey( hOpenKey );
		return !( ( C_SUCCESS != CurVerCheck ) && ( C_SUCCESS != CLSIDCheck ) && ( C_SUCCESS != ProtocolCheck ) && 
				( ( C_TREAT == CurVerCheck ) || ( C_TREAT == CLSIDCheck ) || ( C_TREAT == ProtocolCheck ) ) );
	}

	if ( ( C_SUCCESS != CurVerCheck ) && ( C_SUCCESS != CLSIDCheck ) && ( C_SUCCESS != ProtocolCheck ) )
	{
		RecursivelyAddKey( hOpenKey, strScanObject );
		RegCloseKey( hOpenKey );
		CheckingNowCLSID.pop_back();
		TreatCLSID.push_back( strScanObject );
		return FALSE;
	}

	if ( C_TREAT == CurVerCheck )
	{
		lResult = RegOpenKeyEx( hOpenKey,
			_T( "CurVer" ),
			0,
			KEY_READ|KEY_WRITE| DELETE | STANDARD_RIGHTS_ALL,
			&hSubKey );

		if ( ERROR_SUCCESS == lResult)
		{
			ce_tstring strKeyName1( strScanObject );
			strKeyName1 +=_T( "\\CurVer" );

			RecursivelyAddKey( hSubKey, strKeyName1 );
			RegCloseKey( hSubKey );
		}
	}

	if ( C_TREAT == CLSIDCheck )
	{
		lResult = RegOpenKeyEx( hOpenKey,
			_T( "CLSID" ),
			0,
			KEY_READ|KEY_WRITE| DELETE | STANDARD_RIGHTS_ALL,
			&hSubKey );

		if ( ERROR_SUCCESS == lResult)
		{
			ce_tstring strKeyName1( strScanObject );
			//strKeyName += INTERFACE_KEY;
			//strKeyName += _T( "\\" );
			//strKeyName += szKeyName;
			strKeyName1 +=_T("\\CLSID");

			RecursivelyAddKey( hSubKey, strKeyName1 );
			RegCloseKey( hSubKey );
		}
	}

	if ( C_TREAT == ProtocolCheck )
	{
		lResult = RegOpenKeyEx( hOpenKey,
			_T("protocol\\StdFileEditing\\server"),
			0,
			KEY_READ|KEY_WRITE| DELETE | STANDARD_RIGHTS_ALL,
			&hSubKey );

		if ( ERROR_SUCCESS == lResult)
		{
			ce_tstring strKeyName1( strScanObject );
			//strKeyName += INTERFACE_KEY;
			//strKeyName += _T( "\\" );
			//strKeyName += szKeyName;
			strKeyName1 +=_T("\\protocol\\StdFileEditing\\server");

			RecursivelyAddKey( hSubKey, strKeyName1 );
			RegCloseKey( hSubKey );
		}
	}

	if ( hOpenKey )
	{
		RegCloseKey( hOpenKey );
	}

	CheckingNowCLSID.pop_back();
	return TRUE;
}

/*
*
*
*
*/

BOOL CComObjectRScanner::CheckPotentialThreat( HKEY hKey )
{
	_ASSERT( hKey );

	TCHAR szKeyName[ 16384 ];
	DWORD dwSize = sizeof( szKeyName ) / sizeof( TCHAR );
	DWORD dwIndex = 0;

	DWORD dwItemsCount = 0;

	LONG lResult;

	do
	{
		lResult = RegEnumKeyEx(
			hKey,
			dwIndex++,
			szKeyName,
			&dwSize,
			NULL,
			NULL,
			NULL,
			NULL );

		if( ERROR_SUCCESS == lResult )
			++dwItemsCount;

	} while( ERROR_SUCCESS == lResult );

	if( dwItemsCount >= 2 )
		return FALSE;

	return TRUE;
}
/*************************************************************************
* Function Name:    CTypeLibRScanner::CheckKeyForPath()
* Function Purpose: 
*
* Arguments:        hKey - Handle to a current open key.
*                   
*                   szKeyName - Pointer to a null-terminating string
*                   containing SubKey name that is need to be checked.
*                   
*                   nDepth - Recurse level.
*                   
* Return value:     NO_PATH:        SubKey doesn't contain any path.
*                   CORRECT_PATH:   SubKey contain path and path exist.
*                   INVALID_PATH:   SubKey contain path and path invalid.
*
*************************************************************************/
CHECKPATH_RESULT CComObjectRScanner::CheckKeyForPath( HKEY hKey, TCHAR * szKeyName, unsigned nDepth /* = 0  */)
{
	_ASSERT( hKey );

	if( nDepth > MaxRecurseLevel )
	{
		return NO_PATH;
	}
	LOG_TEXT3(_T("CheckKeyForPath for "),szKeyName,_T( " started"))

		HKEY hOpenKey;
	LONG lResult = RegOpenKeyEx(
		hKey,
		szKeyName,
		0,
		KEY_READ|KEY_WRITE| DELETE | STANDARD_RIGHTS_ALL,
		&hOpenKey );

	if( ERROR_SUCCESS != lResult )
	{
		LOG_TEXT3(_T("RefOpenKeyEx for "),szKeyName,_T( " was failed!"))
			return NO_PATH;
	}

	TCHAR szSubKey[ MAX_KEY_SIZE ];
	DWORD dwIndex = 0;
	DWORD dwSize;

	while( 1 )
	{
		dwSize = sizeof( szSubKey ) / sizeof( TCHAR );
		lResult = RegEnumKeyEx(
			hOpenKey,
			dwIndex++,
			szSubKey,
			&dwSize,
			NULL,
			NULL,
			NULL,
			NULL );

		if( ERROR_SUCCESS != lResult )
		{
			LOG_TEXT3(_T("Key "),szKeyName, _T( " does not contain path!"))
				break;
		}

		if( _tcsicmp( szSubKey, _T("win32") ) == 0 )
		{    
			HKEY hOpenKey2;

			if( ERROR_SUCCESS == RegOpenKeyEx(  hOpenKey,
				szSubKey,
				0,
				KEY_READ|KEY_WRITE| DELETE | STANDARD_RIGHTS_ALL,
				&hOpenKey2 ) )
			{
				DWORD dwDataSize    = 0;
				DWORD dwType        = REG_SZ; 
				countable< TCHAR > spDataBuffer;

				if( ERROR_SUCCESS == RegReadValue(
					hOpenKey2,
					NULL,
					&dwType,
					spDataBuffer,
					dwDataSize ) )
				{
					if( CheckPaths( spDataBuffer ) )
					{
						if (PathIsInReport( spDataBuffer.get() ))
						{
							LOG_TEXT3( _T("Key "),spDataBuffer.get(),_T( " is fake path!"))

								RegCloseKey( hOpenKey );
							RegCloseKey( hOpenKey2);
							return INVALID_PATH;
						}
						else
						{
							LOG_TEXT3( _T("Key "),spDataBuffer.get(),_T( " is good path!"))
								RegCloseKey( hOpenKey );
							RegCloseKey( hOpenKey2);
							return CORRECT_PATH;
						}
					}
					else
					{
						const UINT DATA_BUFFER_SIZE = 32768;
						TCHAR infoBuf[ DATA_BUFFER_SIZE ];
						DWORD bufCharCount = ExpandEnvironmentStrings( spDataBuffer.get(), infoBuf, DATA_BUFFER_SIZE ); 
						CString str = infoBuf;

						LOG_TEXT3(_T("CheckPath for "),spDataBuffer.get(),_T( " was failed!"))

							if ( str.Find( _T(":\\") )== 1 )
							{
								LOG_TEXT3( _T("Key "),spDataBuffer.get(),_T( " is bad path!"))
								RegCloseKey( hOpenKey );
								RegCloseKey( hOpenKey2);
								return INVALID_PATH;
							}
							else
							{
								LOG_TEXT3( _T("Key "),spDataBuffer.get(),_T( " is good path!"))
									RegCloseKey( hOpenKey );
								RegCloseKey( hOpenKey2);
								return CORRECT_PATH;
							}
					}
				}
				else
				{
					LOG_TEXT( _T(" win32 SubKey is empty. This is bad thing, so the pas is INVALID"))
					RegCloseKey( hOpenKey );
					RegCloseKey( hOpenKey2);
					return INVALID_PATH;
				}

				RegCloseKey( hOpenKey2 );
			}
		}

		CHECKPATH_RESULT chkResult;
		chkResult = CheckKeyForPath(
			hOpenKey,
			szSubKey,
			nDepth + 1 );

		if( ( INVALID_PATH == chkResult ) ||
			( CORRECT_PATH == chkResult ) )
		{
			RegCloseKey( hOpenKey );
			return chkResult;
		}
	}

	RegCloseKey( hOpenKey );

	LOG_TEXT3(_T("CheckKeyForPath for "),szKeyName,_T( " finished!"))

		return NO_PATH;
}

BOOL CComObjectRScanner::CLSIDIsInReport( const TCHAR * szCLSID, const TCHAR * szRoot )
{
	//countable< IReportItem > item;
	//size_t ItemCount = m_pCEngine->GetReportObject()->GetItemCount();
	//for( unsigned int i = 0; i < ItemCount ; i++)
	//{
	//	item = m_pCEngine->GetReportObject()->GetItem( i );
	//	if(	COMCLASS_SCANNER_ID == item->m_ScannerID )
	//	{
	//		SRegKReportItem* regitem = item->GetAsRegKItem();

	//		CString strCurrentCLSID;
	//		GetCLSIDFromPath(CString(szCLSID), strCurrentCLSID);

	//		CString strFoundCLSID;
	//		GetCLSIDFromPath(CString(regitem->m_strKeyPath), strFoundCLSID);

	//		if ( strFoundCLSID.CompareNoCase(strCurrentCLSID) == 0 )
	//		{
	//			LOG_TEXT3(_T("IsFakeCLSID for "),szCLSID,_T( " was failed!"));
	//			return TRUE;
	//		}

	//	}
	//}

	CString strCurrentCLSID(szRoot);
	strCurrentCLSID += CLSID_KEY;
	strCurrentCLSID += _T( "\\" );
	strCurrentCLSID += szCLSID;
	//GetCLSIDFromPath(CString(szCLSID), strCurrentCLSID);

	std::vector< CString >::iterator iter;
	iter = find( TreatCLSID.begin(), TreatCLSID.end(), strCurrentCLSID );
	if (TreatCLSID.end() != iter)
	{
		return TRUE;
	}

	return FALSE;
}

/*
*
*
*
*/
BOOL CComObjectRScanner::PathIsInReport( const TCHAR * szPath )
{
	countable< IReportItem > item;
	size_t ItemCount = m_pEngine->GetReportObject()->GetItemCount();
	for( unsigned int i = 0; i < ItemCount ; i++)
	{
		item = m_pEngine->GetReportObject()->GetItem( i );
		if(	DOWNLOAD_SCANNER_ID == item->m_ScannerID )
		{
			SFileReportItem * pItem = item->GetAsFileItem();

			CPath CurrentPath(szPath);
			CPath FoundPath( pItem->m_strDirectory );
			FoundPath.Append(pItem->m_strFile);

			CString strCurrentPath, strFoundPath;

			GetShortPath(CurrentPath,strCurrentPath);
			GetShortPath(FoundPath,strFoundPath);


			if ( strFoundPath == strCurrentPath)
			{
				LOG_TEXT3( _T("IsFakePath for "),szPath,_T( " was failed!"))
					return TRUE;
			}

		}
	}
	return FALSE;
}

/*
*
*
*
*/
BOOL CComObjectRScanner::TypeLibIsInReport( const TCHAR * szTypeLib )
{
	//countable< IReportItem > item;
	//size_t ItemCount = m_pEngine->GetReportObject()->GetItemCount();
	//for( unsigned int i = 0; i < ItemCount ; i++)
	//{
	//	item = m_pEngine->GetReportObject()->GetItem( i );

	//	if ( COMTYPELIB_SCANNER_ID == item->m_ScannerID )
	//	{
	//		SRegKReportItem* regitem = item->GetAsRegKItem();
	//		CString strCurrentItem, strFoundItem;

	//		GetCLSIDFromPath(CString(szTypeLib),strCurrentItem);
	//		GetCLSIDFromPath(CString(regitem->m_strKeyPath),strFoundItem);

	//		if ( strCurrentItem.CompareNoCase(strFoundItem) == 0 )
	//		{
	//			LOG_TEXT3(_T("IsFakeTypeLib for "),szTypeLib,_T( " was failed"))

	//				return TRUE;
	//		}
	//	}
	//}

	CString strCurrentTypeLib( szTypeLib );
	//GetCLSIDFromPath(CString(szTypeLib), strCurrentTypeLib);

	std::vector< CString >::iterator iter;
	iter = find( TreatTypeLib.begin(), TreatTypeLib.end(), strCurrentTypeLib );
	if (TreatTypeLib.end() != iter)
	{
		return TRUE;
	}

	return FALSE;
}

/*
*
*
*
*/

BOOL CComObjectRScanner::UninstallIsInReport( const TCHAR * szUninstall)
{
	countable< IReportItem > item;
	size_t ItemCount = m_pEngine->GetReportObject()->GetItemCount();
	for( unsigned int i = 0; i < ItemCount ; i++)
	{
		item = m_pEngine->GetReportObject()->GetItem( i );
		if(	UNINSTALL_SCANNER_ID == item->m_ScannerID )
		{
			SRegKReportItem* regitem = item->GetAsRegKItem();
			CString str( szUninstall );

			if ( 0 == str.CompareNoCase( regitem->m_strKeyPath ) )
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL CComObjectRScanner::CLSIDIsCheckingNow( const TCHAR * szCLSID )
{
	std::vector< CString >::iterator iter;
	CString strCLSID( szCLSID );

	iter = find( CheckingNowCLSID.begin(), CheckingNowCLSID.end(), strCLSID );

	if ( CheckingNowCLSID.end() != iter )
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CComObjectRScanner::ProgIDIsInReport( const TCHAR * szProgID, const TCHAR * szRoot )
{
	CString strCurrentCLSID(szRoot);
	strCurrentCLSID += szProgID;
	//GetCLSIDFromPath(CString(szCLSID), strCurrentCLSID);

	std::vector< CString >::iterator iter;
	iter = find( TreatCLSID.begin(), TreatCLSID.end(), strCurrentCLSID );
	if (TreatCLSID.end() != iter)
	{
		return TRUE;
	}

	return FALSE;
}
