#include "StdAfx.h"
#include "PrefetchStuffFScanner.h"
#include "CleanEngine.h"

/*************************************************************************
* Function Name:        CPrefetchStuffFScanner::CPrefetchStuffFScanner()
* Function Purpose:     Constructor
*************************************************************************/
CPrefetchStuffFScanner::CPrefetchStuffFScanner( CCleanEngine * pEngine, INotifier * pNotifier /* = NULL */, IScanProfiler * pProfiler /* = NULL */ ) :
    CFileScanner( pEngine, pNotifier, pProfiler ),
	CCleanScanner( pEngine, pNotifier, pProfiler ),
    m_pfnAlgoSelector( NULL )
{
}

/*************************************************************************
* Function Name:        CPrefetchStuffFScanner::~CPrefetchStuffFScanner()
* Function Purpose:     Destructor
*************************************************************************/
CPrefetchStuffFScanner::~CPrefetchStuffFScanner()
{
}

/*************************************************************************
* Function Name:        CPrefetchStuffFScanner::ReinitAlgorithm()
* Function Purpose:       
* Function Description: 
*************************************************************************/
void CPrefetchStuffFScanner::ReinitAlgorithm()
{
    m_pfnAlgoSelector = &CPrefetchStuffFScanner::algoStep1;
}

/*************************************************************************
* Function Name:        CPrefetchStuffFScanner::algorithm()
* Function Purpose:       
* Function Description: 
*************************************************************************/
void CPrefetchStuffFScanner::algorithm()
{
    ( this->*m_pfnAlgoSelector )();
}

/*************************************************************************
* Function Name:        CPrefetchStuffFScanner::algoStep1()
* Function Purpose:     Enumerate prefetch stuffs in 
*                       %WinDir%\prefetch folder. For WinNT systems only.
*
* Arguments:            None
* Return value:         None
*************************************************************************/
void CPrefetchStuffFScanner::algoStep1()
{
    TCHAR szWindowsDir[ MAX_PATH ];

	if( !SHGetSpecialFolderPath(
                            NULL,
                            szWindowsDir,
                            CSIDL_WINDOWS,
                            false ) )
	{
        StopJustMe();
		return;
	}
    
    WIN32_FIND_DATA sFD;
    HANDLE hPrefetchSearch;

    ce_tstring strPrefetchPath( szWindowsDir );
    strPrefetchPath += _T( "\\Prefetch" );
    int size = strPrefetchPath.GetLength();
	strPrefetchPath += _T( "\\*.*" );
    
    hPrefetchSearch = FindFirstFile( strPrefetchPath.GetString(), &sFD );

	if( INVALID_HANDLE_VALUE == hPrefetchSearch )
	{
        StopJustMe();
		return;
	}
    
	strPrefetchPath.GetBufferSetLength( size );

    // Notify user about current scanning object.
    //
    if( m_pNotifier )
    {
        strPrefetchPath.GetBufferSetLength( size );

		m_pNotifier->CallOnScan( ScannerId(), strPrefetchPath.GetString() );
    }

    do
    {
		if( ABANDONNED == InterrogateMngr() )
		{
			break;
		}

		if( m_pNotifier )
		{
			m_pNotifier->CallOnScan( ScannerId(), strPrefetchPath.GetString(), sFD.cFileName );
		}

		// TODO: Implement FileScanner configuration that 
        // it can be abble to skip separate files or file types 
        // ( such as desktop.ini etc. ) Does this need :-?
        if( !( sFD.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN ) && 
            !( sFD.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM ) && 
            !( sFD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ))
        {
			m_pEngine->AddItem(
				countable<IReportItem>(
					new SFileReportItem(
						fn::bind1st1( fn::mem_fun( &CFileScanner::CleanupObject ), this ),
						ScannerId(),
						IT_FILE,
						SEVERITY_MEDIUM,
						STATUS_FOUND,
						sFD.cFileName,
						strPrefetchPath ) ) );
        }
    }
    while( FindNextFile( hPrefetchSearch, &sFD ));

    FindClose( hPrefetchSearch );
    StopJustMe();
}
