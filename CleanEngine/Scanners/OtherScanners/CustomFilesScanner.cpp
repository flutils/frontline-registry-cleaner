#include "StdAfx.h"
#include "CustomFilesScanner.h"
#include "CleanEngine.h"

/*************************************************************************
* Function Name:          CCustomFilesScanner::CCustomFilesScanner()
* Function Description:   Constructor
*************************************************************************/
CCustomFilesScanner::CCustomFilesScanner( CCleanEngine * pEngine, INotifier * pNotifier /* = NULL */, IScanProfiler * pProfiler /* = NULL */ ) :
    CCleanScanner( pEngine, pNotifier, pProfiler ),
    CFileScanner( pEngine, pNotifier, pProfiler ),
    m_pfnAlgoSelector( NULL )
{
}

/*************************************************************************
* Function Name:          CCustomFilesScanner::~CCustomFilesScanner()
* Function Description:   Destructor
*************************************************************************/
CCustomFilesScanner::~CCustomFilesScanner()
{
}

/*************************************************************************
* Function Name:        CCustomFilesScanner::ReinitAlgorithm()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CCustomFilesScanner::ReinitAlgorithm()
{
    m_pfnAlgoSelector = &CCustomFilesScanner::algoStep1;
}

/*************************************************************************
* Function Name:        CCustomFilesScanner::algorithm()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CCustomFilesScanner::algorithm()
{
    ( this->*m_pfnAlgoSelector )();
}

/*************************************************************************
* Function Name:        CCustomFilesScanner::algoStep1()
* Function Purpose:     
*                       
*                       
*
* Arguments:            None
* Return value:         None
*************************************************************************/
void CCustomFilesScanner::algoStep1()
{
	std::list< std::string > listCustomItems;

	if( m_pProfiler )
		m_pProfiler->GetStringList( CUSTOM_LIST, listCustomItems );

	std::list< std::string >::const_iterator it = listCustomItems.begin();
	std::list< std::string >::const_iterator itLast = listCustomItems.end();

	while( it != itLast )
	{
		if( ABANDONNED == InterrogateMngr() )
		{
			break;
		}

		if( m_pNotifier )
		{
			m_pNotifier->CallOnScan( ScannerId(), it->c_str() );
		}

		DWORD dwAttr = GetFileAttributes( it->c_str() );
		if( dwAttr == INVALID_FILE_ATTRIBUTES )
		{
			++it;
			continue;
		}

		if( dwAttr == FILE_ATTRIBUTE_DIRECTORY )
		{
			m_pEngine->AddItem(
				countable<IReportItem>(
					new SFileReportItem(
						fn::bind1st1( fn::mem_fun( &CFileScanner::CleanupObject ), this ),
						ScannerId(),
						IT_DIRECTORY,
						SEVERITY_MEDIUM,
						STATUS_FOUND,
						_T(""),
						it->c_str() ) ) );
		}
		else
		{
			CPath path( it->c_str() );

			int iPos = path.FindFileName();
			CString strDir = path.m_strPath.Left( iPos );
			CString strFile = path.m_strPath.Right( path.m_strPath.GetLength() - iPos );

			m_pEngine->AddItem(
				countable<IReportItem>(
					new SFileReportItem(
						fn::bind1st1( fn::mem_fun( &CFileScanner::CleanupObject ), this ),
						ScannerId(),
						IT_FILE,
						SEVERITY_MEDIUM,
						STATUS_FOUND,
						strFile,
						strDir ) ) );
		}

		++it;
	}

	StopJustMe();
}
