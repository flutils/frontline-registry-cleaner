#include "StdAfx.h"
#include "BrokenShCutsFScanner.h"
#include "CleanEngine.h"

CBrokenShCutsFScanner::CBrokenShCutsFScanner( CCleanEngine * pEngine, INotifier * pNotifier /* = NULL */, IScanProfiler * pProfiler /* = NULL */ ) :
    CFileScanner( pEngine, pNotifier, pProfiler ),
	CCleanScanner( pEngine, pNotifier, pProfiler ),
    m_pfnAlgoSelector( NULL )
{
}

CBrokenShCutsFScanner::~CBrokenShCutsFScanner()
{
}

void CBrokenShCutsFScanner::ReinitAlgorithm()
{
    m_pfnAlgoSelector = &CBrokenShCutsFScanner::algoStep1;
}

void CBrokenShCutsFScanner::algorithm()
{
    ( this->*m_pfnAlgoSelector )();
}

void CBrokenShCutsFScanner::algoStep1()
{
#ifdef _DEBUG
    //MessageBox( NULL, "CBrokenShCutsFScanner::algoStep1()", NULL, MB_OK );
#endif
	DWORD dwType = 0;
	if( m_pProfiler )
	{
		m_pProfiler->GetDWORD( ID_LINK_SCANNER_TYPE, dwType );
	}

	if(dwType)
	{
		m_pfnAlgoSelector = &CBrokenShCutsFScanner::algoStep3;
	}
	else
	{
		m_pfnAlgoSelector = &CBrokenShCutsFScanner::algoStep2;
	}
}

void CBrokenShCutsFScanner::algoStep2()
{
#ifdef _DEBUG
    //MessageBox( NULL, "CBrokenShCutsFScanner::algoStep2()", NULL, MB_OK );
#endif
	TCHAR FolderPath[MAX_PATH];
	//if (SHGetSpecialFolderPath(NULL, FolderPath, CSIDL_PROGRAMS, FALSE))
	//{
	//	std::list< std::string > ext;
	//	std::string str = FolderPath;
	//	ext.push_back( _T("*lnk") );
	//	RecursivelyScanByMask( str, ext );
	//}

	if (SHGetSpecialFolderPath(NULL, FolderPath, CSIDL_STARTMENU , FALSE))
	{
		std::list< std::string > ext;
		std::string str = FolderPath;
		ext.push_back( _T("*lnk") );
		RecursivelyScanByMask( str, ext );
	}
	StopJustMe();
    //m_pfnAlgoSelector = &CBrokenShCutsFScanner::algoStep3;
}

void CBrokenShCutsFScanner::algoStep3()
{
#ifdef _DEBUG
    //MessageBox( NULL, "CBrokenShCutsFScanner::algoStep3() : STOP", NULL, MB_OK );
#endif
	std::list< std::string > FixedDrives,ext;
	GetDrivesArray( 0xFFFFFFFF, FixedDrives );
	ext.push_back( _T("*lnk") );
	for (std::list<std::string>::iterator i = FixedDrives.begin(); i != FixedDrives.end(); i++)
	{
		RecursivelyScanByMask( (*i), ext );
	}
    StopJustMe();
}

void CBrokenShCutsFScanner::OnScanConditionDone(LPCTSTR strCurrentDir, LPCTSTR strFileName)
{
	IPersistFile* ppf;
	IShellLink* pshl;
	WIN32_FIND_DATA wfd;
	CString str;
	HRESULT hres;

	CoInitialize(NULL);

	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink/*IID_IPersistFile/**/, reinterpret_cast<LPVOID*>(&pshl));
	if( SUCCEEDED( hres ) )
	{

		hres = pshl->QueryInterface(IID_IPersistFile,reinterpret_cast<void**>(&ppf));
		if ( SUCCEEDED( hres ) )
		{
			WCHAR wsz[MAX_PATH];
			
			str = strCurrentDir;
			str += _T("\\");
			str += strFileName;
			
			MultiByteToWideChar(CP_ACP, 0, str.GetString(), -1, wsz, MAX_PATH); 
			hres = ppf->Load(wsz, STGM_READWRITE);
			//ppf->QueryInterface(IID_IShellLink, reinterpret_cast<void**>(&pshl));
			if( SUCCEEDED( hres ) )
			{
				LPTSTR pszTarget = new TCHAR[MAX_PATH];
				pshl->GetPath(pszTarget, MAX_PATH, &wfd, 0);
				if( lstrcmpi(pszTarget,"") )
				{

					hres = pshl->Resolve(NULL, SLR_NOUPDATE | SLR_NOSEARCH | SLR_NO_UI );
					if( NO_ERROR != hres )
					{
						//CString str1;	
						//int i = 0;
						//LPTSTR pszTarget = new TCHAR[MAX_PATH];
						//pshl->GetPath(pszTarget, MAX_PATH, &wfd, 0);
						//str = pszTarget;
						//i = str.ReverseFind('\\');
						//str1 = strFileName;
						//str = strG;

						m_pEngine->AddItem(
							countable<IReportItem>(
								new SFileReportItem(
									fn::bind1st1( fn::mem_fun( &CFileScanner::CleanupObject ), this ),
									ScannerId(),
									IT_FILE,
									SEVERITY_MEDIUM,
									STATUS_FOUND,
									strFileName /*str1.GetString()*/,
									strCurrentDir /*str.GetString()*/ ) ) );
					}
				}
				delete[] pszTarget;
			}
			ppf->Release();
		}
		pshl->Release();
	}
	
	CoUninitialize();
}