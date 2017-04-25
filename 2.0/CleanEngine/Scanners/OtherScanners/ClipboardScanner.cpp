#include "StdAfx.h"
#include "ClipboardScanner.h"
#include "CleanEngine.h"

/*************************************************************************
* Function Name:          CClipboardScanner::CClipboardScanner()
* Function Description:   Constructor
*************************************************************************/
CClipboardScanner::CClipboardScanner( CCleanEngine * pEngine, INotifier * pNotifier /* = NULL */, IScanProfiler * pProfiler /* = NULL */ ) :
    CCleanScanner( pEngine, pNotifier, pProfiler ),
    m_pfnAlgoSelector( NULL )
{
}

/*************************************************************************
* Function Name:          CClipboardScanner::~CClipboardScanner()
* Function Description:   Destructor
*************************************************************************/
CClipboardScanner::~CClipboardScanner()
{
}

/*************************************************************************
* Function Name:        CClipboardScanner::ReinitAlgorithm()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CClipboardScanner::ReinitAlgorithm()
{
    m_pfnAlgoSelector = &CClipboardScanner::algoStep1;
}

/*************************************************************************
* Function Name:        CClipboardScanner::algorithm()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CClipboardScanner::algorithm()
{
    ( this->*m_pfnAlgoSelector )();
}

/*************************************************************************
* Function Name:        CClipboardScanner::algoStep1()
* Function Purpose:     
*                       
*                       
*
* Arguments:            None
* Return value:         None
*************************************************************************/
void CClipboardScanner::algoStep1()
{
	if( m_pNotifier )
		m_pNotifier->OnScan( ScannerId(), _T("Clipboard"), NULL );

	if ( !::OpenClipboard( ::GetDesktopWindow() ) )
	{
        StopJustMe();
        return;
	}

	int iCount = ::CountClipboardFormats();
	if (0 == iCount)
		iCount = 1;

	UINT uiFormat = 0;
	TCHAR szBuf[16];

	while (true)
	{
		uiFormat = ::EnumClipboardFormats( uiFormat );
		if ( 0 == uiFormat )
		{
			DWORD dwErr = ::GetLastError();
			ATLASSERT( ERROR_SUCCESS == dwErr );

			break;
		}

		_ultoa( uiFormat, szBuf, 10 );

		m_pEngine->AddItem(
			countable<IReportItem>(
				new SCustomReportItem(
					fn::bind1st1( fn::mem_fun( &CClipboardScanner::CleanupObject ), this ),
					ScannerId(),
					IT_OTHER,
					SEVERITY_MEDIUM,
					STATUS_FOUND,
					szBuf ) ) );
	}

	CloseClipboard();

	StopJustMe();
}

/*************************************************************************
* Function Name:        CClipboardScanner::CleanupObject()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CClipboardScanner::CleanupObject( countable< IReportItem > item )
{
	item->m_Status = STATUS_CLEANING;

	ON_FIX( item );

	SCustomReportItem * pItem = item.get()->GetAsCustomItem();

	do
	{
		if ( !::OpenClipboard( ::GetDesktopWindow() ) )
		{
			item->m_Status = STATUS_CLEAN_FAILED;
			break;
		}

		::EmptyClipboard() ? 
			item->m_Status = STATUS_CLEANED:
			item->m_Status = STATUS_CLEAN_FAILED;

		::CloseClipboard();
	} while (false);

	ON_FIX( item );
}
