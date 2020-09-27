#include "StdAfx.h"
#include "IEAutoFillScanner.h"
#include "msiehost.h"
#include "FileTools.h"
#include "CleanEngine.h"
#include "scopeguard.h"

#include <comdef.h>

CIEAutoFillScanner::CIEAutoFillScanner( CCleanEngine * pEngine, INotifier * pNotifier /* = NULL */, IScanProfiler * pProfiler /* = NULL */ ) :
	CCleanScanner( pEngine, pNotifier, pProfiler ),
    m_pfnAlgoSelector( NULL ),
	m_hPStoreLib( NULL )
{
}

CIEAutoFillScanner::~CIEAutoFillScanner()
{
}

void CIEAutoFillScanner::ReinitAlgorithm()
{
    m_pfnAlgoSelector = &CIEAutoFillScanner::algoStep1;
}

void CIEAutoFillScanner::algorithm()
{
    ( this->*m_pfnAlgoSelector )();
}

void CIEAutoFillScanner::algoStep1()
{
	CMStore				*pCMStore	= NULL;		// Интерфейс CMStore
	IEnumPStoreItems	*pItems		= NULL;		
	IEnumPStoreTypes	*pTypes		= NULL;
	IEnumPStoreTypes	*pSubTypes	= NULL;
    HRESULT				hRes		= S_OK;		// статус выполнения
	DWORD				dwFlag		= 0;		
	DWORD				pceltFetched= 0;		
	LPWSTR				pszItemName;			// Item Name 
	GUID				guidType;				// Item Type
	GUID				guidSubType;			// Item SubType
	//LPBYTE				pbData;				// Item Data
	//LPSTR				szData;					// Item Data(в текстовом виде)
	PST_TYPEINFO		*pTypeInfo	= NULL;		// Type info struct
	PST_TYPEINFO		*pSubTInfo	= NULL;		// SubType info struct
	DWORD				dwData		= 200;		// Длинна Item Data
	PST_KEY				TypeKey		= PST_KEY_CURRENT_USER;
	PST_KEY				SubTypeKey	= PST_KEY_CURRENT_USER;
	PST_KEY				ItemKey		= PST_KEY_CURRENT_USER;
	PST_KEY				ReadKey		= PST_KEY_CURRENT_USER;
	DWORD				dwThreatsCount = 0;

	PST_PROMPTINFO		promptInfo	= {sizeof(promptInfo),0,NULL,NULL};

	BOOL bRes = GetCMStoreInterface( &pCMStore );
	if ( !bRes || !pCMStore ) 
		goto cleanup;

	if( m_pNotifier )
		m_pNotifier->OnScan( ScannerId(), _T("Internet Explorer PStore..."), NULL );

	//------------------------------------------------------------------------------------------
	//
	// Структура цикла получения Информации из PStore
	//	Type						// Получаем Type-элементы 
	//    |							
	//    +-> SybType				// Используя Type -> получаем SybType
	//          |					
	//          +-> Items			// Используя Type и SubType -> получаем Items
	//                |				 
	//                +-> Data		// Для доступа к данным нам надо Type, SubType и Items
	//								// Используя Type и SubType и Items -> получаем Data(данные) 
	//
	// Каждый внутренний блок("...БЛОК ПОЛУЧЕНИЯ...") - зависит от верхнего.
	//
	//------------------------------------------------------------------------------------------

	hRes = pCMStore->EnumTypes( TypeKey, dwFlag, &pTypes );
	while( SUCCEEDED( hRes ) )
	{
		hRes = pTypes->Next( 1, &guidType, &pceltFetched );
		if( SUCCEEDED( hRes ) )
		{
			hRes = pCMStore->GetTypeInfo( TypeKey, &guidType, &pTypeInfo, 0 );
			if( lstrcmpW( pTypeInfo->szDisplayName, L"Internet Explorer" ) )
				continue;

			hRes = pCMStore->EnumSubtypes( SubTypeKey, &guidType, dwFlag, &pSubTypes );
			while( SUCCEEDED( hRes) )
			{
				hRes = pSubTypes->Next( 1, &guidSubType, &pceltFetched );
				if( SUCCEEDED( hRes ) )
				{
					hRes = pCMStore->GetSubtypeInfo( SubTypeKey, &guidType, &guidSubType, &pSubTInfo, 0 );
					//printf("SubType Name: %S\n",pSubTInfo->szDisplayName);

					hRes = pCMStore->EnumItems( ItemKey, &guidType, &guidSubType, dwFlag, &pItems );
					while( SUCCEEDED( hRes ) )
					{
						hRes = pItems->Next( 1, &pszItemName, &pceltFetched );
						if( SUCCEEDED( hRes ) )
						{
							++dwThreatsCount;

							if( ABANDONNED == InterrogateMngr() )
							{
								break;
							}

							//printf("\tItem:%S\n",pszItemName);
							//hRes = pCMStore->ReadItem( ReadKey, &guidType, &guidSubType, pszItemName, &dwData, (LPBYTE*)&pbData, &promptInfo, 0 );
							//if( hRes == S_OK )
							//{
							//	szData=(LPSTR) LocalAlloc(LMEM_FIXED,dwData);
							//	memcpy(szData,pbData,dwData);
							//	printf("\tData:%S\n",szData);
							//	LocalFree(szData);
							//}
						}

						if( ( hRes != S_OK ) && (ItemKey != PST_KEY_LOCAL_MACHINE) )
						{
							ItemKey = PST_KEY_LOCAL_MACHINE;
							hRes = pCMStore->EnumItems( ItemKey, &guidType, &guidSubType, dwFlag, &pItems );
						}
					}

					hRes = S_OK;
					ItemKey = PST_KEY_CURRENT_USER;
				}

				if  (( hRes != S_OK ) && (SubTypeKey != PST_KEY_LOCAL_MACHINE))
				{
					SubTypeKey = PST_KEY_LOCAL_MACHINE;
					hRes = pCMStore->EnumSubtypes(SubTypeKey,&guidType,dwFlag,&pSubTypes);
				}
			}

			CoTaskMemFree( pTypeInfo );
			hRes		= S_OK;
			SubTypeKey	= PST_KEY_CURRENT_USER;
		}

		if ( ( hRes != S_OK ) && ( TypeKey != PST_KEY_LOCAL_MACHINE ) )
		{
			TypeKey = PST_KEY_LOCAL_MACHINE;
			hRes = pCMStore->EnumTypes( TypeKey, dwFlag, &pTypes );
		}
	}

cleanup:

	if ( hRes == -2147024894 )	// Если элементов нет
		hRes = S_OK;

	if ( pItems )		
		pItems->Release();
	if ( pTypes )		
		pTypes->Release();
	if ( pSubTypes )	
		pSubTypes->Release();
	if ( pCMStore )	
		pCMStore->Release();
	//if ( pTypeInfo )	
	//	CoTaskMemFree( pTypeInfo );

	FreeLibrary( m_hPStoreLib );

	if( dwThreatsCount )
	{
		CString strCount;
		strCount.Format( _T("%d"), dwThreatsCount );

		countable<IReportItem> cItem(
			new SCustomReportItem(
				fn::bind1st1( fn::mem_fun( &CIEAutoFillScanner::CleanupObject ), this ),
				ScannerId(),
				IT_OTHER,
				SEVERITY_MEDIUM,
				STATUS_FOUND,
				strCount ) );

		m_pEngine->AddItem( cItem );
	}

	StopJustMe();
}

void CIEAutoFillScanner::CleanupObject( countable< IReportItem > item )
{
	if( item->m_Type != IT_OTHER )
	{
		ATLASSERT( FALSE );

		SET_ITEM_STATUS( item, STATUS_CLEAN_FAILED );
		return;
	}

	SET_ITEM_STATUS( item, STATUS_CLEANING );

	HRESULT hr = ::CoInitialize(NULL);
	if ( FAILED( hr ) )
	{
		SET_ITEM_STATUS( item, STATUS_CLEAN_FAILED );
		return;
	}

	ON_BLOCK_EXIT( &::CoUninitialize );
	
	IOleCommandTargetPtr p;
	hr = p.CreateInstance(__uuidof(InternetExplorer));

	if( FAILED( hr ) )
	{
		SET_ITEM_STATUS( item, STATUS_CLEAN_FAILED );
		return;
	}

	VARIANT v;
	v.vt = VT_I4;
	v.lVal = IECMDID_ARG_CLEAR_FORMS_ALL;

	hr = p->Exec(&CGID_InternetExplorer,
		IECMDID_CLEAR_AUTOCOMPLETE_FOR_FORMS,
		OLECMDEXECOPT_DONTPROMPTUSER, &v, NULL);

	if ( FAILED( hr ) )
	{
		do
		{
			HMODULE hDll = LoadLibrary( _T("shdocvw.dll") );
			if (NULL == hDll)
			{
				hr = AtlHresultFromLastError();
				item->m_Status = STATUS_CLEAN_FAILED;
				break;
			}

			HRESULT (__stdcall * ClearAutoSuggestForForms_)(DWORD) = NULL;

			reinterpret_cast<FARPROC &>(ClearAutoSuggestForForms_) =
				::GetProcAddress(hDll, reinterpret_cast<LPCSTR>(211));

			if (NULL == ClearAutoSuggestForForms_)
			{
				hr = AtlHresultFromLastError();
				item->m_Status = STATUS_CLEAN_FAILED;
				FreeLibrary( hDll );
				break;
			}

			hr = (*ClearAutoSuggestForForms_)( IECMDID_ARG_CLEAR_FORMS_ALL );
			item->m_Status = FAILED( hr ) ? STATUS_CLEAN_FAILED : STATUS_CLEANED;

			FreeLibrary( hDll );
		}
		while (false);
	}
	else
		item->m_Status = STATUS_CLEANED;

	ON_FIX( item );
}

//****************************************************************************
// Функция получения указателя на интерфейс CMStore (IPStore+ICMStore)
// из pstorec.dll(наша)
//****************************************************************************
BOOL CIEAutoFillScanner::GetCMStoreInterface(CMStore **ppStore)
{
	m_hPStoreLib = LoadLibrary( _T("pstorec.dll") );
	if ( !m_hPStoreLib )
		return FALSE;

	typedef HRESULT (__stdcall * PFPSTORECREATEINSTANCE)( CMStore** ppProvider, PST_PROVIDERID* pProviderID, void* pReserved, DWORD dwFlags );
	PFPSTORECREATEINSTANCE pfPStoreCreateInstance = (PFPSTORECREATEINSTANCE)GetProcAddress( m_hPStoreLib, _T("PStoreCreateInstance") );
	if ( !pfPStoreCreateInstance )
		return FALSE;

	if ( S_OK != pfPStoreCreateInstance( ppStore, 0, 0, 0 ) )
		return FALSE;

	return TRUE;
}
