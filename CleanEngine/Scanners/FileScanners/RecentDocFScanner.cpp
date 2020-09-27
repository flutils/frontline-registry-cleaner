#include "StdAfx.h"
#include "RecentDocFScanner.h"
#include "CleanEngine.h"

/*************************************************************************
* Function Name:        CRecentDocFScanner::CRecentDocFScanner()
* Function Purpose:     Constructor
*************************************************************************/
CRecentDocFScanner::CRecentDocFScanner( CCleanEngine * pEngine, INotifier * pNotifier /* = NULL */, IScanProfiler * pProfiler /* = NULL */ ) :
    CFileScanner( pEngine, pNotifier, pProfiler ),
	CCleanScanner( pEngine, pNotifier, pProfiler ),
    m_pfnAlgoSelector( NULL ),
    m_hRecentSearch( NULL ), 
    m_strRecentDir(_T( "" ))
{
}

/*************************************************************************
* Function Name:        CRecentDocFScanner::~CRecentDocFScanner()
* Function Purpose:     Destructor
*************************************************************************/
CRecentDocFScanner::~CRecentDocFScanner()
{
}

/*************************************************************************
* Function Name:        CRecentDocFScanner::ReinitAlgorithm()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CRecentDocFScanner::ReinitAlgorithm()
{
    m_pfnAlgoSelector = &CRecentDocFScanner::algoStep1;
}

/*************************************************************************
* Function Name:        CRecentDocFScanner::algorithm()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CRecentDocFScanner::algorithm()
{
    ( this->*m_pfnAlgoSelector )();
}

/*************************************************************************
* Function Name:        CRecentDocFScanner::postop()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CRecentDocFScanner::postop()
{
    CFileScanner::postop();
	m_strRecentDir.Empty();

    if( m_hRecentSearch && ( INVALID_HANDLE_VALUE != m_hRecentSearch ))
    {
        FindClose( m_hRecentSearch );
        m_hRecentSearch = NULL;
    }
}

/*************************************************************************
* Function Name:        CDownloadFScanner::algoStep1()
* Function Purpose:     Start enumerate files in Recent Documents dir.
*
* Arguments:            None
* Return value:         None
*************************************************************************/
void CRecentDocFScanner::algoStep1()
{
    TCHAR szRecentPath[ MAX_PATH + 1024 ];

	if( !SHGetSpecialFolderPath(
                            NULL, 
                            szRecentPath, 
                            CSIDL_RECENT, 
                            false ))
	{
        StopJustMe();
		return;
	}
    
    m_strRecentDir = szRecentPath;
    _tcscat( szRecentPath, _T( "\\*.*" ));
    szRecentPath[ sizeof( szRecentPath ) - 1 ] = _T( '\0' );
    
	m_hRecentSearch = FindFirstFile( szRecentPath, &m_sFD );

	if( INVALID_HANDLE_VALUE == m_hRecentSearch )
	{
        StopJustMe();
		return;
	}

    // Notify user about current scanning object.
    //
    if( m_pNotifier )
    {
        m_pNotifier->CallOnScan( ScannerId(), m_strRecentDir.GetString(), m_sFD.cFileName );
    }
    
    // TODO: File scanners can be abble to be configured to skip or
    //  not system, hidden and other files (such as desktop.ini etc)
    //
    if( !( m_sFD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) && 
        !( m_sFD.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) && 
        !( m_sFD.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM ))        
    {
		m_pEngine->AddItem(
			countable<IReportItem>(
				new SFileReportItem(
					fn::bind1st1( fn::mem_fun( &CFileScanner::CleanupObject ), this ),
					ScannerId(),
					IT_FILE,
					SEVERITY_MEDIUM,
					STATUS_FOUND,
					m_sFD.cFileName,
					m_strRecentDir ) ) );
    }
    
    m_pfnAlgoSelector = &CRecentDocFScanner::algoStep2;
}

/*************************************************************************
* Function Name:        CDownloadFScanner::algoStep1()
* Function Purpose:     Enumerate files in Recent Documents dir.
*
* Arguments:            None
* Return value:         None
*************************************************************************/
void CRecentDocFScanner::algoStep2()
{
    _ASSERT(( ( m_hRecentSearch != NULL ) && 
              ( m_hRecentSearch != INVALID_HANDLE_VALUE ) ));

    if( FindNextFile( m_hRecentSearch, &m_sFD ))
    {
		// Notify user about current scanning object.
        //
        if( m_pNotifier )
        {
	        m_pNotifier->CallOnScan( ScannerId(), m_strRecentDir.GetString(), m_sFD.cFileName );
        }

        // TODO: File scanners can be abble to be configured to skip or
        // not system, hidden and other files (such as desktop.ini etc)
        //
        if( !( m_sFD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) &&
            !( m_sFD.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) &&
            !( m_sFD.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM ))
        {
			m_pEngine->AddItem(
				countable<IReportItem>(
					new SFileReportItem(
						fn::bind1st1( fn::mem_fun( &CFileScanner::CleanupObject ), this ),
						ScannerId(),
						IT_FILE,
						SEVERITY_MEDIUM,
						STATUS_FOUND,
						m_sFD.cFileName,
						m_strRecentDir ) ) );
        }
        
        return;
    }

    StopJustMe();
}
