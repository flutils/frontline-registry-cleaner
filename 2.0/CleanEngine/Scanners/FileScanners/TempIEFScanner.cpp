#include "StdAfx.h"
#include "TempIEFScanner.h"
#include "IndexDatFile.h"
#include "CleanEngine.h"

/*************************************************************************
* Function Name:        CTempIEFScanner::CTempIEFFScanner()
* Function Purpose:     Constructor
*************************************************************************/
CTempIEFScanner::CTempIEFScanner( CCleanEngine * pEngine, INotifier * pNotifier /* = NULL */, IScanProfiler * pProfiler /* = NULL */ ) :
    CFileScanner( pEngine, pNotifier, pProfiler ),
	CCleanScanner( pEngine, pNotifier, pProfiler ),
    m_pfnAlgoSelector( NULL ),
    m_hTempIeSearch( NULL ), 
    m_strTempIeDir(_T( "" ))
{
    //AddExcluded( ce_tstring(_T( "desktop.ini" )));
    //AddExcluded( ce_tstring(_T( "index.dat" )));
}

/*************************************************************************
* Function Name:        CTempIEFScanner::~CTempIEFFScanner()
* Function Purpose:     Destructor
*************************************************************************/
CTempIEFScanner::~CTempIEFScanner()
{
}

/*************************************************************************
* Function Name:        CTempIEFScanner::ReinitAlgorithm()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CTempIEFScanner::ReinitAlgorithm()
{
    m_pfnAlgoSelector = &CTempIEFScanner::algoStep1;
}

/*************************************************************************
* Function Name:        CTempIEFScanner::algorithm()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CTempIEFScanner::algorithm()
{
    ( this->*m_pfnAlgoSelector )();
}

/*************************************************************************
* Function Name:        CTempIEFScanner::postop()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CTempIEFScanner::postop()
{
    CFileScanner::postop();
	m_strTempIeDir.Empty();

    if( m_hTempIeSearch && ( INVALID_HANDLE_VALUE != m_hTempIeSearch ))
    {
        FindClose( m_hTempIeSearch );
        m_hTempIeSearch = NULL;
    }
}

/*************************************************************************
* Function Name:        CTempIEFScanner::algoStep1()
* Function Purpose:     
*						
* Arguments:            None
* Return value:         None
*************************************************************************/
void CTempIEFScanner::algoStep1()
{
	LPINTERNET_CACHE_ENTRY_INFO pInfo = NULL;

	DWORD dwSize = 1000;
	DWORD dw = dwSize;

	pInfo = (LPINTERNET_CACHE_ENTRY_INFO) malloc ( dwSize );

	HANDLE hFind = SafeFindFirstUrlCacheEntryEx( 0, 0,
		NORMAL_CACHE_ENTRY | STICKY_CACHE_ENTRY,
		0, pInfo, &dw, 0, 0, 0 );

	if( dw > 20000 )
	{
		if( dwSize && pInfo )
			free( pInfo );

	    StopJustMe();
		return;
	}

	if( !hFind && ( dw > dwSize ) )
	{
		dwSize = dw;
		free( pInfo );

		pInfo = (LPINTERNET_CACHE_ENTRY_INFO)malloc( dwSize );
		pInfo->dwStructSize = dwSize;
		hFind = SafeFindFirstUrlCacheEntryEx( 0, 0,
			NORMAL_CACHE_ENTRY | STICKY_CACHE_ENTRY,
			0, pInfo, &dw, 0, 0, 0 );
	}

	CString szName, szLocation;
	if( hFind )
	{
		dw = dwSize;
		while( true )
		{
			szName = pInfo->lpszSourceUrlName;
			szLocation = pInfo->lpszLocalFileName;

			if( ABANDONNED == InterrogateMngr() )
			{
				break;
			}

			if( m_pNotifier )
				m_pNotifier->OnScan( ScannerId(), szName, NULL );

			countable<IReportItem> cItem(
				new SCustomReportItem(
					fn::bind1st1( fn::mem_fun( &CTempIEFScanner::CleanupObject ), this ),
					ScannerId(),
					IT_OTHER,
					SEVERITY_MEDIUM,
					STATUS_FOUND,
					szName ) );

			cItem->m_strReserved = szLocation;

			m_pEngine->AddItem( cItem );

			if( !SafeFindNextUrlCacheEntry( hFind, pInfo, &dw ) )
				if( ( ERROR_NO_MORE_FILES == GetLastError() ) || ( dw <= dwSize ) )
					break;
				else
				{
					if( dwSize && pInfo )
						free( pInfo );

					dwSize = dw;
					pInfo = (LPINTERNET_CACHE_ENTRY_INFO) malloc ( dwSize );
					pInfo->dwStructSize = dwSize;

					if( !SafeFindNextUrlCacheEntry( hFind, pInfo, &dw ) )
						break;
				}

				pInfo->dwStructSize = dwSize;
				dw = dwSize;
		}

		FindCloseUrlCache( hFind );
	}

	if( dwSize && pInfo )
		free( pInfo );

	StopJustMe();
}

/*************************************************************************
* Function Name:        CTempIEFScanner::SafeFindFirstUrlCacheEntryEx()
* Function Purpose:     
*						
* Arguments:            None
* Return value:         None
*************************************************************************/
HANDLE CTempIEFScanner::SafeFindFirstUrlCacheEntryEx(LPCTSTR lpszUrlSearchPattern,
	DWORD dwFlags, DWORD dwFilter, GROUPID GroupId,
	LPINTERNET_CACHE_ENTRY_INFO lpFirstCacheEntryInfo,
	LPDWORD lpdwFirstCacheEntryInfoBufferSize,
	LPVOID lpReserved, LPDWORD pcbReserved2, LPVOID lpReserved3)
{
	HANDLE hResult = NULL;

	__try
	{
		hResult = ::FindFirstUrlCacheEntryEx(lpszUrlSearchPattern,
			dwFlags, dwFilter, GroupId, lpFirstCacheEntryInfo,
			lpdwFirstCacheEntryInfoBufferSize, lpReserved,
			pcbReserved2, lpReserved3);
	}
	__except ( EXCEPTION_EXECUTE_HANDLER )
	{
		hResult = NULL;
	}

	return hResult;
}

/*************************************************************************
* Function Name:        CTempIEFScanner::SafeFindNextUrlCacheEntry()
* Function Purpose:     
*						
* Arguments:            None
* Return value:         None
*************************************************************************/
BOOL CTempIEFScanner::SafeFindNextUrlCacheEntry(HANDLE hEnumHandle,
	LPINTERNET_CACHE_ENTRY_INFO lpNextCacheEntryInfo,
	LPDWORD lpdwNextCacheEntryInfoBufferSize)
{
	BOOL bRes = FALSE;

	__try
	{
		bRes = ::FindNextUrlCacheEntry(hEnumHandle, lpNextCacheEntryInfo,
			lpdwNextCacheEntryInfoBufferSize);
	}
	__except ( EXCEPTION_EXECUTE_HANDLER )
	{
		bRes = FALSE;
	}

	return bRes;
}

/*************************************************************************
* Function Name:        CTempIEFScanner::CleanupObject()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CTempIEFScanner::CleanupObject( countable< IReportItem > item )
{
	item->m_Status = STATUS_CLEANING;

	ON_FIX( item );

	SCustomReportItem * pItem = item->GetAsCustomItem();
	if( !pItem )
	{
		item->m_Status = STATUS_CLEAN_FAILED;
		ON_FIX( item );
		return;
	}

	BOOL bRes;
	if( S_OK != IsPathExist( pItem->m_strReserved ) )
		bRes = TRUE;
	else
	{
		BOOL bUseSD = FALSE;
		if( m_pEngine )
			bUseSD = InterlockedCompareExchange( &m_pEngine->m_bUsePostponedClean, FALSE, FALSE );

		DWORD dwRetries;

		if( m_pProfiler )
			m_pProfiler->GetDWORD( ID_SECURE_LEVEL, dwRetries );

		if( bUseSD )
		{
			bRes = ( S_OK == SecureDeleteFile( pItem->m_strReserved, static_cast<UINT>( dwRetries ) ) );
		}
		else
		{
			bRes = DeleteFile( pItem->m_strReserved );
		}
	}

	do
	{
		if( !DeleteUrlCacheEntry( pItem->m_strCustomDescription ) )
		{
			DWORD dwErr = ::GetLastError();
			if ( (ERROR_FILE_NOT_FOUND != dwErr) && (ERROR_PATH_NOT_FOUND != dwErr) )
			{
				item->m_Status = STATUS_CLEAN_FAILED;
				break;
			}
		}

		item->m_Status = bRes ? STATUS_CLEANED : STATUS_CLEAN_FAILED;

	} while( FALSE );

	if( item->m_Status == STATUS_CLEANED )
	{
		CString strUrl;

		std::set<CString> arrIndexDat;
		GetAllIEHistoryIndexDat( arrIndexDat );
		for ( std::set<CString>::iterator it = arrIndexDat.begin(),
			end = arrIndexDat.end(); it != end; ++it )
		{
			CIndexDatFile file;
			HRESULT hr = file.Load(*it);
			if (FAILED(hr))
				continue;

			strUrl = pItem->m_strCustomDescription;
			int nPos = strUrl.Find( _T('@') );
			if ( 0 <= nPos )
				strUrl.Delete( 0, nPos + 1 );

			file.DeleteUrl( strUrl );
			file.RemoveEmptyRootItems( *it );
		}
	}

	ON_FIX( item );
}
