#include "StdAfx.h"
#include "IECookiesFScanner.h"
#include "CleanEngine.h"

typedef std::basic_ifstream< TCHAR > tifstream;

/*************************************************************************
* Function Name:        CIECookiesFScanner::CIECookiesFScanner()
* Function Purpose:     Constructor
*************************************************************************/
CIECookiesFScanner::CIECookiesFScanner( CCleanEngine * pEngine, INotifier * pNotifier /* = NULL */, IScanProfiler * pProfiler /* = NULL */ ) :
    CFileScanner( pEngine, pNotifier, pProfiler ),
	CCleanScanner( pEngine, pNotifier, pProfiler ),
    m_pfnAlgoSelector( NULL ),
    m_hCookieSearch( NULL ),
    m_strCookieDir(_T( "" ))
{
//    AddExcluded(_T( "aol.com" ));
//    AddExcluded(_T( "msn.com" ));
//    AddExcluded(_T( "hotmail.com" ));
//    AddExcluded(_T( "microsoft.com" ));
//    AddExcluded(_T( "google.com" ));
//    AddExcluded(_T( "yahoo.com" ));
//    AddExcluded(_T( "verizon.net" ));
//    AddExcluded(_T( "attglobal.net" ));
//    AddExcluded(_T( "comcast.net" ));
//    AddExcluded(_T( "blueyonder.co.uk" ));
//    AddExcluded(_T( "btinternet.com" ));
////  Add from GUI dynamically, own cookie for concrete product
//    AddExcluded(EasyCript(_T("xjogjyfs/dpn" )));
//    AddExcluded(_T( "sympatico.ca" ));
//    AddExcluded(_T( "yahoo.co.uk" ));
//    AddExcluded(_T( "charter.net" ));
//    AddExcluded(_T( "netzero.net" ));
//    AddExcluded(_T( "earthlink.net" ));
//    AddExcluded(_T( "cox.net" ));
//    AddExcluded(_T( "wanadoo.co.uk" ));
//    AddExcluded(_T( "bellsouth.net" ));
//    AddExcluded(_T( "bigpond.net.au" ));
//    AddExcluded(_T( "rushmore.com" ));
//    AddExcluded(_T( "homecall.co.uk" ));
//    AddExcluded(_T( "sbcglobal.net" ));
//    AddExcluded(_T( "tds.net" ));
//    AddExcluded(_T( "telus.net" ));
//    AddExcluded(_T( "usa.com" ));
//    AddExcluded(_T( "dial.pipex.com" ));
//    AddExcluded(_T( "caves.net" ));
//    AddExcluded(_T( "academicplanet.com" ));
//    AddExcluded(_T( "dccnet.com" ));
//    AddExcluded(_T( "gmail.com" ));
//    AddExcluded(_T( "gci.net" ));
//    AddExcluded(_T( "netscape.com" ));

//	AddExcluded(EasyCript(_T("bdujwbuf/tztufnepdups/dpn")));
//	AddExcluded(EasyCript(_T("tfdvsf/tztufnepdups/dpn" )));
//	AddExcluded(EasyCript(_T("tztufnepdups/dpn" )));
//	AddExcluded(EasyCript(_T("bdujwbujpo/tztufnepdups/dpn" )));

//	AddExcluded(EasyCript(_T( "bdujwbuf/fsspsqspufdups/dpn" )));
//	AddExcluded(EasyCript(_T( "tfdvsf/fsspsqspufdups/dpn" )));
//	AddExcluded(EasyCript(_T( "fsspsqspufdups/dpn" )));
//	AddExcluded(EasyCript(_T( "bdujwbujpo/fsspsqspufdups/dpn" )));
}

/*************************************************************************
* Function Name:        CIECookiesFScanner::~CIECookiesFScanner()
* Function Purpose:     Destructor
*************************************************************************/
CIECookiesFScanner::~CIECookiesFScanner()
{
}

/*************************************************************************
* Function Name:        CIECookiesFScanner::ReinitAlgorithm()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CIECookiesFScanner::ReinitAlgorithm()
{
    m_pfnAlgoSelector = &CIECookiesFScanner::algoStep1;
}

/*************************************************************************
* Function Name:        CIECookiesFScanner::algorithm()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CIECookiesFScanner::algorithm()
{
    ( this->*m_pfnAlgoSelector )();
}

/*************************************************************************
* Function Name:        CIECookiesFScanner::postop()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CIECookiesFScanner::postop()
{
    CFileScanner::postop();
    m_strCookieDir.Empty();

    if( m_hCookieSearch && ( INVALID_HANDLE_VALUE != m_hCookieSearch ))
    {
        FindClose( m_hCookieSearch );
        m_hCookieSearch = NULL;
    }
}

/*************************************************************************
* Function Name:        CIECookiesFScanner::algoStep1()
* Function Purpose:     Start enumerate cookie files in IE Cookie folder.
*
* Arguments:            None
* Return value:         None
*************************************************************************/
void CIECookiesFScanner::algoStep1()
{
	TCHAR szCookieDir[ MAX_PATH + 1024 ];
    TCHAR szCookiePath[ MAX_PATH + 1024 ];

	if( !SHGetSpecialFolderPath(
                            NULL, 
                            szCookieDir, 
                            CSIDL_COOKIES, 
                            false ))
	{
        StopJustMe();
		return;
	}

	_tcsncpy( szCookiePath, szCookieDir, sizeof( szCookiePath ));
    szCookiePath[ sizeof( szCookiePath ) - 1 ] = _T( '\0' );

    _tcscat( szCookiePath, _T( "\\*.txt" ));
    szCookiePath[ sizeof( szCookiePath ) - 1 ] = _T( '\0' );

	m_strCookieDir = szCookieDir;
	m_hCookieSearch = FindFirstFile( szCookiePath, &m_sFD );

	if( INVALID_HANDLE_VALUE == m_hCookieSearch )
	{
        StopJustMe();
		return;
	}

    // Notify user about current scanning object.
    //
    ce_tstring strScanObject( m_strCookieDir );
    strScanObject += _T( "\\" );
    strScanObject += m_sFD.cFileName;

    if( m_pNotifier )
    {   
		m_pNotifier->CallOnScan( ScannerId(), strScanObject.GetString() );
    }

    if( !( m_sFD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ))
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
					m_strCookieDir ) ) );
    }

    m_pfnAlgoSelector = &CIECookiesFScanner::algoStep2;
}

/*************************************************************************
* Function Name:        CIECookiesFScanner::algoStep2()
* Function Purpose:     Continue enumerate cookie files in cookie folder.
*
* Arguments:            None
* Return value:         None
*************************************************************************/
void CIECookiesFScanner::algoStep2()
{
    if( FindNextFile( m_hCookieSearch, &m_sFD ))
    {
        // Notify user about current scanning object.
        //
        if( m_pNotifier )
        {
            m_pNotifier->CallOnScan( ScannerId(), m_strCookieDir.GetString(), m_sFD.cFileName );
        }

        if( !( m_sFD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ))
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
						m_strCookieDir ) ) );
        }
        
        return ;
    }

    StopJustMe();
}
