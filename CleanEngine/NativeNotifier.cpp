#include "StdAfx.h"
#include "NativeNotifier.h"

CNativeNotifier::CNativeNotifier(void):Handler(0)
{
}

CNativeNotifier::~CNativeNotifier(void)
{
}

void CNativeNotifier::OnScan( const scanner_id scannerId, const TCHAR * szObjectName, const TCHAR * szValueName )
{

}

void CNativeNotifier::OnItemFind( countable< IReportItem > item )
{
	CString strDescr;
	//::MessageBox(0,0,0,0);
	switch( item->m_Type )
	{
	case IT_REG_KEY:
		if( SRegKReportItem * pRegKReportItem = item.get()->GetAsRegKItem() )
		{
			strDescr = _T("Key (<K>)");
			pRegKReportItem->GenerateDescription( strDescr );
		}
		break;

	case IT_REG_VALUE:
		if( SRegVReportItem * pRegVReportItem = item.get()->GetAsRegVItem() )
		{
			strDescr = _T("Subkey (<S>) at key (<K>)");
			pRegVReportItem->GenerateDescription( strDescr );
		}
		break;

	case IT_FILE:
		if( SFileReportItem * pFileReportItem = item.get()->GetAsFileItem() )
		{
			strDescr = _T("File (<F>) at directory (<D>)");
			pFileReportItem->GenerateDescription( strDescr );
		}
		break;

	case IT_DIRECTORY:
		if( SFileReportItem * pFileReportItem = item.get()->GetAsFileItem() )
		{
			strDescr = _T("Directory (<D>)");
			pFileReportItem->GenerateDescription( strDescr );
		}
		break;

	case IT_OTHER:
		if( SCustomReportItem * pCustomReportItem = item.get()->GetAsCustomItem() )
		{
			pCustomReportItem->GenerateDescription( strDescr );
		}
		break;

	default:
		ATLASSERT( FALSE );
	}

	static int index = 0;

	CString strItemID;
	CString strScannerID;

	strItemID.Format( _T("%d"), item.get()->m_ItemID );
	strScannerID.Format( _T("%d"), item.get()->m_ScannerID );

	DWORD dwID = static_cast<DWORD>( item.get()->m_ItemID );

	
	Handler(strDescr, item);
//	::MessageBox(0, strDescr, strScannerID,0);
// 	index = m_pScanList->InsertItem( index + 1, strItemID );
// 	m_pScanList->SetItemText( index, 1, strScannerID );
// 	m_pScanList->SetItemText( index, 2, strDescr );
// 	m_pScanList->SetItemText( index, 3, GetDescrFromStatus( item->m_Status ) );
// 	m_pScanList->SetItemData( index, dwID );
}

void CNativeNotifier::OnItemFix( countable< IReportItem > item )
{

}

void CNativeNotifier::OnScannerStarted( const scanner_id scannerId )
{
	ScanStart(scannerId);
}

void CNativeNotifier::OnScannerFinished( const scanner_id scannerId )
{
	ScanFinish(scannerId);
}