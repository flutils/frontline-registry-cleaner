#include "StdAfx.h"
#include "IEScanner.h"
#include "CleanEngine.h"
#include "scopeguard.h"
#include "IndexDatFile.h"

#include <LMCons.h>
#include <comdef.h>
#include <comdefsp.h>

const TCHAR g_tszTypedURLs[] = _T("Software\\Microsoft\\Internet Explorer\\TypedURLs");
const TCHAR g_tszPrivacyHistory[] = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\P3P\\History\\");

/*************************************************************************
* Function Name:          CIEScanner::CIEScanner()
* Function Description:   Constructor
*************************************************************************/
CIEScanner::CIEScanner( CCleanEngine * pEngine, INotifier * pNotifier /* = NULL */, IScanProfiler * pProfiler /* = NULL */ ) :
    CCleanScanner( pEngine, pNotifier, pProfiler ),
	CFileScanner( pEngine, pNotifier, pProfiler ),
	CRegistryScanner( pEngine, pNotifier, pProfiler ),
    m_pfnAlgoSelector( NULL )
{
}

/*************************************************************************
* Function Name:          CIEScanner::~CIEScanner()
* Function Description:   Destructor
*************************************************************************/
CIEScanner::~CIEScanner()
{
}

/*************************************************************************
* Function Name:        CIEScanner::ReinitAlgorithm()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CIEScanner::ReinitAlgorithm()
{
    m_pfnAlgoSelector = &CIEScanner::algoStep1;
}

/*************************************************************************
* Function Name:        CIEScanner::algorithm()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CIEScanner::algorithm()
{
    ( this->*m_pfnAlgoSelector )();
}

/*************************************************************************
* Function Name:        CIEScanner::algoStep1()
* Function Purpose:     
*                       
*                       
*
* Arguments:            None
* Return value:         None
*************************************************************************/
void CIEScanner::algoStep1()
{
	CString szPath;
	if( GetWindowsDirectory( szPath.GetBuffer( MAX_PATH  ), MAX_PATH ) )
	{
		szPath.ReleaseBuffer();
		szPath += _T("\\Downloaded Installations\\");

		RecursivelyAddDirectory( szPath, false );
	}

    m_pfnAlgoSelector = &CIEScanner::algoStep2;
}

/*************************************************************************
* Function Name:        CIEScanner::algoStep2()
* Function Purpose:     
*                       
*                       
*
* Arguments:            None
* Return value:         None
*************************************************************************/
void CIEScanner::algoStep2()
{
	HKEY hKey;
	if( ERROR_SUCCESS == RegOpenKeyEx(
										HKEY_CURRENT_USER,
										g_tszTypedURLs,
										0,
										KEY_READ|KEY_WRITE| DELETE,
										&hKey ) )
	{	
		CString strKeyName( _T("HKEY_CURRENT_USER\\") );
		strKeyName += g_tszTypedURLs;

		EnumValues( hKey, strKeyName );

		RegCloseKey( hKey );
	}

	m_pfnAlgoSelector = &CIEScanner::algoStep3;
}

/*************************************************************************
* Function Name:        CIEScanner::algoStep3()
* Function Purpose:     
*                       
*                       
*
* Arguments:            None
* Return value:         None
*************************************************************************/
void CIEScanner::algoStep3()
{
	HKEY hKey;
	if( ERROR_SUCCESS == RegOpenKeyEx(
										HKEY_CURRENT_USER,
										g_tszPrivacyHistory,
										0,
										KEY_READ|KEY_WRITE| DELETE,
										&hKey ) )
	{	
		CString strKeyName( _T("HKEY_CURRENT_USER\\") );
		strKeyName += g_tszPrivacyHistory;

		EnumValues( hKey, strKeyName );

		RegCloseKey( hKey );
	}

	m_pfnAlgoSelector = &CIEScanner::algoStep4;
}

/*************************************************************************
* Function Name:        CIEScanner::algoStep4()
* Function Purpose:     
*                       
*                       
*
* Arguments:            None
* Return value:         None
*************************************************************************/
void CIEScanner::algoStep4()
{
	HRESULT hr = ::CoInitialize(NULL);
	if( FAILED( hr ) )
	{
		StopJustMe();
	}

	ON_BLOCK_EXIT( &::CoUninitialize );

	IUrlHistoryStgPtr spHistory;
	hr = spHistory.CreateInstance( CLSID_CUrlHistory );
	if( FAILED( hr ) )
		StopJustMe();

	IEnumSTATURLPtr spEnum;
	hr = spHistory->EnumUrls( &spEnum );
	if( FAILED( hr ) )
		StopJustMe();

	spEnum->SetFilter( NULL, STATURL_QUERYFLAG_NOTITLE );

	while( TRUE )
	{
		if( ABANDONNED == InterrogateMngr() )
			break;

		ULONG ulFetched = 0;
		STATURL statUrl = { sizeof( statUrl ) };

		hr = SafeEnumNext( spEnum, 1, &statUrl, &ulFetched );

		if ( FAILED( hr ) || ( 1 != ulFetched ) )
			break;

		if ( NULL == statUrl.pwcsUrl )
			continue;

		ON_BLOCK_EXIT( &::CoTaskMemFree, statUrl.pwcsUrl );

		static CString strUserName;
		if ( strUserName.IsEmpty() )
		{
			DWORD dwLen = UNLEN + 1;
			::GetUserName( strUserName.GetBuffer(dwLen), &dwLen );
			strUserName.ReleaseBuffer();
		}

		CString szLocation( statUrl.pwcsUrl );

		m_pEngine->AddItem(
			countable<IReportItem>(
				new SCustomReportItem(
					fn::bind1st1( fn::mem_fun( &CIEScanner::CleanupObject ), this ),
					ScannerId(),
					IT_OTHER,
					SEVERITY_MEDIUM,
					STATUS_FOUND,
					szLocation ) ) );

		//CString szName;
		//szName.Format(_T("Visited: %s@%s"),
		//	strUserName.GetString(), szLocation.GetString());

		//#ifdef USE_KEYWORDS_FOR_VISITED_IE_URL_HISTORY
		////we have to exclude all elements which includes protected keywords
		//CProtectedKeywords &keywords = CProtectedKeywords::GetInstance();
		//if( !keywords.IsEntranceExist( szName ) )
		//#endif
		//	sThreatObject.m_arrItems.Add( dct::SThreatObjectItem( dct::titIEURLHistory55, szName, szLocation, 0 ) );
	}

	StopJustMe();
}

/*************************************************************************
* Function Name:        CIEScanner::EnumValues()
* Function Purpose:     
*                       
*                       
*
* Arguments:            None
* Return value:         None
*************************************************************************/
void CIEScanner::EnumValues( HKEY hKey, const CString & strKeyName )
{
	TCHAR szSubkeyName[ MAX_KEY_SIZE ];

	DWORD dwIndex = 0;
	DWORD dwType;

	do
	{
		DWORD dwSize = MAX_KEY_SIZE;

		LONG lResult = RegEnumValue(
								hKey,
								dwIndex++,
								szSubkeyName,
								&dwSize,
								NULL,
								&dwType,
								NULL,
								NULL );

		if( ERROR_SUCCESS != lResult )
		{
			break;
		}

		if( ABANDONNED == InterrogateMngr() )
		{
			break;
		}

		if( m_pNotifier )
		{
			m_pNotifier->CallOnScan( ScannerId(), strKeyName, szSubkeyName );
		}

		m_pEngine->AddItem(
			countable<IReportItem>(
				CREATE_REG_ITEM(
					CRegistryScanner::CleanupObject,
					ScannerId(),
					SEVERITY_MEDIUM,
					strKeyName,
					szSubkeyName,
					dwType ) ) );

	} while( true );
}

/*************************************************************************
* Function Name:        CIEScanner::SafeEnumNext()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
HRESULT CIEScanner::SafeEnumNext( IEnumSTATURL * pEnum, ULONG celt, LPSTATURL rgelt, ULONG *pceltFetched )
{
	HRESULT hr = E_FAIL;

	__try
	{
		hr = pEnum->Next(celt, rgelt, pceltFetched);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		hr = E_FAIL;
	}

	return hr;
}

/*************************************************************************
* Function Name:        CIEScanner::CleanupObject()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CIEScanner::CleanupObject( countable< IReportItem > item )
{
	if( item->m_Type == IT_FILE || item->m_Type == IT_DIRECTORY )
		CFileScanner::CleanupObject( item );
	else if( item->m_Type == IT_REG_KEY || item->m_Type == IT_REG_VALUE )
		CRegistryScanner::CleanupObject( item );
	else if( item->m_Type == IT_OTHER )
	{
		SET_ITEM_STATUS( item, STATUS_CLEANING );

		if( FAILED( ::CoInitialize(NULL) ) )
		{
			SET_ITEM_STATUS( item, STATUS_CLEAN_FAILED );
			return;
		}

		IUrlHistoryStgPtr spHistory;
		HRESULT hRes = spHistory.CreateInstance(CLSID_CUrlHistory);
		if ( FAILED( hRes ) )
		{
			SET_ITEM_STATUS( item, STATUS_CLEAN_FAILED );
			return;
		}

#ifndef UNICODE
		hRes = spHistory->DeleteUrl( CT2W( item->GetAsCustomItem()->m_strCustomDescription ), 0 );
#else
		hRes = spHistory->DeleteUrl( item->GetAsCustomItem()->m_strCustomDescription , 0 );
#endif

		if( ( ERROR_FILE_NOT_FOUND == HRESULT_CODE( hRes ) ) || ( ERROR_PATH_NOT_FOUND == HRESULT_CODE( hRes ) ) )
			hRes = S_OK;

		if( SUCCEEDED( hRes ) )
		{
			SET_ITEM_STATUS( item, STATUS_CLEANED );

			std::set<CString> arrIndexDat;
			GetAllIEHistoryIndexDat( arrIndexDat );
			for ( std::set<CString>::iterator it = arrIndexDat.begin(),
				end = arrIndexDat.end(); it != end; ++it )
			{
				CIndexDatFile file;
				HRESULT hr = file.Load(*it);
				if (FAILED(hr))
					continue;

				file.DeleteUrl( item->GetAsCustomItem()->m_strCustomDescription );
				file.RemoveEmptyRootItems( *it );
			}
		}
		else
		{
			SET_ITEM_STATUS( item, STATUS_CLEAN_FAILED );
		}
	}
	else
		ATLASSERT( FALSE );
}
