#include "StdAfx.h"
#include "IEHistoryFScanner.h"
#include <atlcomcli.h>
#include <comdef.h>
#include <mshtml.h>
#include <UrlHist.h>
#include "CleanEngine.h"


/*************************************************************************
* Function Name:        CIEHistoryFScanner::CIEHistoryFScanner()
* Function Purpose:     Constructor
*************************************************************************/
CIEHistoryFScanner::CIEHistoryFScanner( CCleanEngine * pEngine, INotifier * pNotifier /* = NULL */, IScanProfiler * pProfiler /* = NULL */ ) :
    CCleanScanner( pEngine, pNotifier, pProfiler ),
    m_pfnAlgoSelector( NULL )
{
}

/*************************************************************************
* Function Name:        CIEHistoryFScanner::~CIEHistoryFScanner()
* Function Purpose:     Destructor
*************************************************************************/
CIEHistoryFScanner::~CIEHistoryFScanner()
{   
}

/*************************************************************************
* Function Name:        CIEHistoryFScanner::ReinitAlgorithm()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CIEHistoryFScanner::ReinitAlgorithm()
{
    m_pfnAlgoSelector = &CIEHistoryFScanner::algoStep1;
}

/*************************************************************************
* Function Name:        CIEHistoryFScanner::algorithm()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CIEHistoryFScanner::algorithm()
{
    ( this->*m_pfnAlgoSelector )();
}

/*************************************************************************
* Function Name:        CIEHistoryFScanner::algoStep1()
* Function Purpose:     Create IUrlHistoryStg interface and enumerate 
*                       history visited links.
*
* Arguments:            None
* Return value:         None
*************************************************************************/
void CIEHistoryFScanner::algoStep1()
{
	m_hrInitialize = CoInitialize( NULL );

	if( m_pNotifier )
	{
		m_pNotifier->CallOnScan( ScannerId(), _T("IE History") );
	}

    while( 1 )
    {   
        CComPtr< IUrlHistoryStg2 >  history;
        CComPtr< IEnumSTATURL >     enumPtr;
        STATURL                     url;
        HRESULT                     hr;
        ULONG                       uFetched;
	    CLSID                       clsid;
        CString                     strUrl;
        
        hr = CLSIDFromString( 
            L"{3C374A40-BAE4-11CF-BF7D-00AA006946EE}", &clsid );
                
        if( NOERROR != hr )
        {
            break;
        }

        if( !SUCCEEDED( history.CoCreateInstance( clsid ) ) )
        {
            break;
        }

        if( S_OK != history->EnumUrls( &enumPtr ) )
        {
            break;
        }
        
        while( SUCCEEDED( enumPtr->Next( 1, &url, &uFetched ) ) )
        {
            if( !uFetched )
            {
                break;
            }

            strUrl = url.pwcsUrl;

			m_pEngine->AddItem(
				countable<IReportItem>(
					new SCustomReportItem(
						fn::bind1st1( fn::mem_fun( &CIEHistoryFScanner::CleanupObject ), this ),
						ScannerId(),
						IT_OTHER,
						SEVERITY_MEDIUM,
						STATUS_FOUND,
						strUrl ) ) );
        }

        break;
    }

	if( S_OK == m_hrInitialize )
	{
	    CoUninitialize();
	}

	StopJustMe();
}

/*************************************************************************
* Function Name:        CIEHistoryFScanner::CleanupObject()
* Function Purpose:     Use IUrlHistoryStg2 interface for cleaning 
*                       IE history visited links.
*
* Arguments:            None
* Return value:         None
*************************************************************************/
void CIEHistoryFScanner::CleanupObject( countable< IReportItem > item )
{
	HRESULT hr;
    CLSID clsid;

	SET_ITEM_STATUS( item, STATUS_CLEANING );

	SCustomReportItem * pItem = item.get()->GetAsCustomItem();

	if( !pItem )
	{
		SET_ITEM_STATUS( item, STATUS_CLEAN_FAILED );
		return;
	}

    hr = CLSIDFromString(
            L"{3C374A40-BAE4-11CF-BF7D-00AA006946EE}", &clsid );

    if( NULL == pItem || NOERROR != hr )
    {
		SET_ITEM_STATUS( item, STATUS_CLEAN_FAILED );
		return;
    }

	m_hrInitialize = CoInitialize( NULL );

    while( 1 )
    {
        CComPtr< IUrlHistoryStg2 > history;
        if( !SUCCEEDED( hr = history.CoCreateInstance( clsid ) ) )
        {
			item->m_Status = STATUS_CLEAN_FAILED;
            break;
        }

		CT2OLE strOleUrl( pItem->m_strCustomDescription );
        hr = history->DeleteUrl( strOleUrl, 0 );

		if( ( ERROR_FILE_NOT_FOUND == HRESULT_CODE( hr ) ) || ( ERROR_PATH_NOT_FOUND == HRESULT_CODE( hr ) ) )
			hr = S_OK;

        if( SUCCEEDED( hr ) )
        {
			item->m_Status = STATUS_CLEANED;
        }
		else
		{
			item->m_Status = STATUS_CLEAN_FAILED;
		}

		break;
    }

	if( S_OK == m_hrInitialize )
	{
	    CoUninitialize();
	}

	ON_FIX( item );
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CIEHistoryFScanner::ClearHistory( void )
{
	IUrlHistoryStg2* pUrlHistoryStg2 = NULL;

	HRESULT hr = CoCreateInstance(CLSID_CUrlHistory,
		NULL, CLSCTX_INPROC, IID_IUrlHistoryStg2,
		(void**)&pUrlHistoryStg2);

	if (SUCCEEDED(hr))
	{
		hr = pUrlHistoryStg2->ClearHistory();
		pUrlHistoryStg2->Release();
	}

}