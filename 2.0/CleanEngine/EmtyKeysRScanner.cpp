#include "StdAfx.h"
#include "EmtyKeysRScanner.h"
#include "CleanEngine.h"

CEmtyKeysRScanner::CEmtyKeysRScanner(CCleanEngine * pEngine, INotifier * pNotifier /* = NULL */, IScanProfiler * pProfiler /* = NULL */ ) :
CRegistryScanner( pEngine, pNotifier, pProfiler ),
CCleanScanner( pEngine, pNotifier, pProfiler ),
m_pfnAlgoSelector( NULL ),
m_dwIndex( 0 )
{
}

CEmtyKeysRScanner::~CEmtyKeysRScanner(void)
{
}

void CEmtyKeysRScanner::ReinitAlgorithm()
{
	m_pfnAlgoSelector = &CEmtyKeysRScanner::algoStep1;
	m_dwIndex = 0;
}

void CEmtyKeysRScanner::algorithm()
{
	( this->*m_pfnAlgoSelector )();
}

void CEmtyKeysRScanner::postop( void )
{
	CRegistryScanner::postop();
}

void CEmtyKeysRScanner::algoStep1()
{
	countable< IReportItem > item;
	size_t ItemCount = m_pEngine->GetReportObject()->GetItemCount();
	for( unsigned int i = 0; i < ItemCount ; i++)
	{
		item = m_pEngine->GetReportObject()->GetItem( i );
		
		
		if ( EMPTY_KEYS_SCANNER_ID == item->m_ScannerID )
		{
			
			SRegKReportItem* regitem = item->GetAsRegKItem();
			if( m_pNotifier )
			{
				//::MessageBox(0,0,_T("errs"),0);
				//CString strDesc = _T(""); 
				//regitem->GenerateDescription(strDesc);
				//RecursivelyAddKey(regitem->)
				//m_pNotifier->CallOnScan( EMPTY_KEYS_SCANNER_ID, _T(""));
				m_pNotifier->OnItemFind(item);
			}
			
		}
	}
	StopJustMe();
}

