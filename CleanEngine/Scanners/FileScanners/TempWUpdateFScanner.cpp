#include "StdAfx.h"
#include "TempWUpdateFScanner.h"
#include "CleanEngine.h"

/*************************************************************************
* Function Name:        CTempWUpdateFScanner::CTempWUpdateFScanner()
* Function Purpose:     Constructor
*************************************************************************/
CTempWUpdateFScanner::CTempWUpdateFScanner( CCleanEngine * pEngine, INotifier * pNotifier /* = NULL */, IScanProfiler * pProfiler /* = NULL */ ) :
	CFileScanner( pEngine, pNotifier, pProfiler ),
	CCleanScanner( pEngine, pNotifier, pProfiler ),
	m_pfnAlgoSelector( NULL ),
	m_hUpdateSearch( NULL ),
	m_strUpdateDir(_T( "" ))
{
}

/*************************************************************************
* Function Name:        CTempWUpdateFScanner::~CTempWUpdateFScanner()
* Function Purpose:     Destructor
*************************************************************************/
CTempWUpdateFScanner::~CTempWUpdateFScanner()
{
}

/*************************************************************************
* Function Name:        CTempWUpdateFScanner::ReinitAlgorithm()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CTempWUpdateFScanner::ReinitAlgorithm()
{
    m_pfnAlgoSelector = &CTempWUpdateFScanner::algoStep1;
}

/*************************************************************************
* Function Name:        CTempWUpdateFScanner::postop()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CTempWUpdateFScanner::postop()
{
    CFileScanner::postop();
    m_strUpdateDir.Empty();

    if( m_hUpdateSearch && (INVALID_HANDLE_VALUE != m_hUpdateSearch ))
    {
        FindClose( m_hUpdateSearch );
        m_hUpdateSearch = NULL;
    }
}

/*************************************************************************
* Function Name:        CTempWUpdateFScanner::algorithm()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CTempWUpdateFScanner::algorithm()
{
    ( this->*m_pfnAlgoSelector )();
}

/*************************************************************************
* Function Name:        CTempWUpdateFScanner::algoStep1()
* Function Purpose:     Start enumerate files in 
*                       %ProgramFiles%\WindowsUpdate\*.*
*
* Arguments:            None
* Return value:         None
*************************************************************************/
void CTempWUpdateFScanner::algoStep1()
{
    TCHAR szUpdateDir[ MAX_PATH + 1024 ];
    TCHAR szUpdatePath[ MAX_PATH + 1024 ];

	if( !SHGetSpecialFolderPath(
                            NULL, 
                            szUpdateDir, 
                            CSIDL_PROGRAM_FILES, 
                            false ))
	{
        m_pfnAlgoSelector = &CTempWUpdateFScanner::algoStep2;
		return;
	}
    
    _tcscat( szUpdateDir, _T("\\WindowsUpdate") );
    szUpdateDir[ sizeof( szUpdatePath ) - 1 ] = _T( '\0' );
    m_strUpdateDir = szUpdateDir;

	_tcsncpy( szUpdatePath, szUpdateDir, sizeof( szUpdateDir ));
    szUpdatePath[ sizeof( szUpdatePath ) - 1 ] = _T( '\0' );

	_tcscat( szUpdatePath, _T( "\\*.*" ));
    szUpdatePath[ sizeof( szUpdatePath ) - 1 ] = _T( '\0' );
    
	m_hUpdateSearch = FindFirstFile( szUpdatePath, &m_sFD );

	if( INVALID_HANDLE_VALUE == m_hUpdateSearch )
	{
        StopJustMe();
		return;
	}

    // Notify user about current scanning object.
    //
    if( m_pNotifier )
    {
		m_pNotifier->CallOnScan( ScannerId(), m_strUpdateDir.GetString(), m_sFD.cFileName );
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
					m_strUpdateDir ) ) );
    }
    
    m_pfnAlgoSelector = &CTempWUpdateFScanner::algoStep2;
}

/*************************************************************************
* Function Name:        CTempWUpdateFScanner::algoStep2()
* Function Purpose:     Enumerate files in 
*                       %ProgramFiles%\WindowsUpdate\*.*
*
* Arguments:            None
* Return value:         None
*************************************************************************/
void CTempWUpdateFScanner::algoStep2()
{
    _ASSERT(( ( m_hUpdateSearch != NULL ) && 
              ( m_hUpdateSearch != INVALID_HANDLE_VALUE ) ));

    if( FindNextFile( m_hUpdateSearch, &m_sFD ))
    {
        // Notify user about current scanning object.
        //
        if( m_pNotifier )
        {
			m_pNotifier->CallOnScan( ScannerId(), m_strUpdateDir.GetString(), m_sFD.cFileName );
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
						m_strUpdateDir ) ) );
		}
        
        return;
    }

    StopJustMe();
}
