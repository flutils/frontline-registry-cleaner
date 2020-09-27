#include "StdAfx.h"
#include "DownloadFScanner.h"
#include "CleanEngine.h"

/*************************************************************************
* Function Name:        CDownloadFScanner::CDownloadFScanner()
* Function Purpose:     Constructor
*************************************************************************/
CDownloadFScanner::CDownloadFScanner( CCleanEngine * pEngine, INotifier * pNotifier /* = NULL */, IScanProfiler * pProfiler /* = NULL */ ) :
    CFileScanner( pEngine, pNotifier, pProfiler ),
	CCleanScanner( pEngine, pNotifier, pProfiler ),
    m_pfnAlgoSelector( NULL ),
    m_hDownloadSearch( NULL ), 
    m_strDownloadDir(_T( "" ))
{
}

/*************************************************************************
* Function Name:        CDownloadFScanner::CDownloadFScanner()
* Function Purpose:     Destructor
*************************************************************************/
CDownloadFScanner::~CDownloadFScanner()
{
}

/*************************************************************************
* Function Name:        CDownloadFScanner::ReinitAlgorithm()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CDownloadFScanner::ReinitAlgorithm()
{
    m_pfnAlgoSelector = &CDownloadFScanner::algoStep1;
}

/*************************************************************************
* Function Name:        CDownloadFScanner::postop()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CDownloadFScanner::postop()
{
    CFileScanner::postop();
	m_strDownloadDir.Empty();

    if( m_hDownloadSearch && (INVALID_HANDLE_VALUE != m_hDownloadSearch ))
    {
        FindClose( m_hDownloadSearch );
        m_hDownloadSearch = NULL;
    }
}

/*************************************************************************
* Function Name:        CDownloadFScanner::algorithm()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CDownloadFScanner::algorithm()
{
    ( this->*m_pfnAlgoSelector )();
}

/*************************************************************************
* Function Name:        CDownloadFScanner::algoStep1()
* Function Purpose:     Start enumerate files in 
*                       %WinDir%\Downloaded Installations\*.*
*
* Arguments:            None
* Return value:         None
*************************************************************************/
void CDownloadFScanner::algoStep1()
{
    TCHAR szDownloadDir[ MAX_PATH + 1024 ];
    TCHAR szDownloadPath[ MAX_PATH + 1024 ];

	if( !SHGetSpecialFolderPath(
                            NULL, 
                            szDownloadDir, 
                            CSIDL_WINDOWS, 
                            false ))
	{
        m_pfnAlgoSelector = &CDownloadFScanner::algoStep3;
		return;
	}
    
    _tcscat( szDownloadDir, _T( "\\Downloaded Installations" ));
    szDownloadDir[ sizeof( szDownloadDir ) - 1 ] = _T( '\0' );
    m_strDownloadDir = szDownloadDir;

	_tcsncpy( szDownloadPath, szDownloadDir, sizeof( szDownloadDir ));
    szDownloadPath[ sizeof( szDownloadPath ) - 1 ] = _T( '\0' );

	_tcscat( szDownloadPath, _T( "\\*.*" ));
    szDownloadPath[ sizeof( szDownloadPath ) - 1 ] = _T( '\0' );
    
	m_hDownloadSearch = FindFirstFile( szDownloadPath, &m_sFD );

	if( INVALID_HANDLE_VALUE == m_hDownloadSearch )
	{
        m_pfnAlgoSelector = &CDownloadFScanner::algoStep3;
		return;
	}

    // Notify user about current scanning object.
    //
    if( m_pNotifier )
    {
        ce_tstring strScanObject( m_strDownloadDir );
        strScanObject += _T( "\\" );
        strScanObject += m_sFD.cFileName;
        
		m_pNotifier->CallOnScan( ScannerId(), strScanObject.GetString() );
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
					m_strDownloadDir ) ) );
    }
    
    m_pfnAlgoSelector = &CDownloadFScanner::algoStep2;
}

/*************************************************************************
* Function Name:        CDownloadFScanner::algoStep2()
* Function Purpose:     Enumerate files in 
*                       %WinDir%\Downloaded Installations\*.*
*
* Arguments:            None
* Return value:         None
*************************************************************************/
void CDownloadFScanner::algoStep2()
{
    if( AddDownloadFile() )
    {
        return;
    }
    
    FindClose( m_hDownloadSearch );
    m_hDownloadSearch = NULL;

    m_pfnAlgoSelector = &CDownloadFScanner::algoStep3;
}

/*************************************************************************
* Function Name:        CDownloadFScanner::algoStep1()
* Function Purpose:     Start enumerate files in 
*                       %WinDir%\Downloaded Program Files\*.*
*
* Arguments:            None
* Return value:         None
*************************************************************************/
void CDownloadFScanner::algoStep3()
{
    TCHAR szDownloadDir[ MAX_PATH + 1024 ];
    TCHAR szDownloadPath[ MAX_PATH + 1024 ];

	if( !SHGetSpecialFolderPath(
                            NULL, 
                            szDownloadDir, 
                            CSIDL_WINDOWS, 
                            false ))
	{
        StopJustMe();
		return;
	}
    
    _tcscat( szDownloadDir, _T( "\\Downloaded Program Files" ));
    szDownloadDir[ sizeof( szDownloadDir ) - 1 ] = _T( '\0' );
    m_strDownloadDir = szDownloadDir;

	_tcsncpy( szDownloadPath, szDownloadDir, sizeof( szDownloadDir ));
    szDownloadPath[ sizeof( szDownloadPath ) - 1 ] = _T( '\0' );

	_tcscat( szDownloadPath, _T( "\\*.*" ));
    szDownloadPath[ sizeof( szDownloadPath ) - 1 ] = _T( '\0' );
    
	m_hDownloadSearch = FindFirstFile( szDownloadPath, &m_sFD );

	if( INVALID_HANDLE_VALUE == m_hDownloadSearch )
	{
        StopJustMe();
		return;
	}

    //
    // Notify user about current scanning object.
    //
    if( m_pNotifier )
    {
        ce_tstring strScanObject( m_strDownloadDir );
        strScanObject += _T( "\\" );
        strScanObject += m_sFD.cFileName;
        
        m_pNotifier->CallOnScan( ScannerId(), strScanObject.GetString() );
    }
    
    //
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
					m_strDownloadDir ) ) );
    }
    
    m_pfnAlgoSelector = &CDownloadFScanner::algoStep4;
}

/*************************************************************************
* Function Name:        CDownloadFScanner::algoStep4()
* Function Purpose:     Enumerate files in 
*                       %WinDir%\Downloaded Program Files\*.*
*
* Arguments:            None
* Return value:         None
*************************************************************************/
void CDownloadFScanner::algoStep4()
{
    if( AddDownloadFile() )
    {
        return;
    }

    StopJustMe();
}

/*************************************************************************
* Function Name:        CDownloadFScanner::AddDownloadFile()
* Function Purpose:     Add next downloaded file in current scanning dir.
*
* Arguments:            None
* Return value:         True if next file was added, false if there are 
*                       no files left.
*************************************************************************/
bool CDownloadFScanner::AddDownloadFile()
{
    _ASSERT(( ( m_hDownloadSearch != NULL ) && 
              ( m_hDownloadSearch != INVALID_HANDLE_VALUE ) ));

    if( FindNextFile( m_hDownloadSearch, &m_sFD ))
    {
        //
        // Notify user about current scanning object.
        //
        if( m_pNotifier )
        {
            ce_tstring strScanObject( m_strDownloadDir );
            strScanObject += _T( "\\" );
            strScanObject += m_sFD.cFileName;
            
            m_pNotifier->CallOnScan( ScannerId(), strScanObject.GetString() );
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
						m_strDownloadDir ) ) );
        }
        
        return true;
    }

    return false;
}

