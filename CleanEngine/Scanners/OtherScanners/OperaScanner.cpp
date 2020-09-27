#include "StdAfx.h"
#include "OperaScanner.h"
#include "CleanEngine.h"

/*************************************************************************
* Function Name:          COperaScanner::COperaScanner()
* Function Description:   Constructor
*************************************************************************/
COperaScanner::COperaScanner( CCleanEngine * pEngine, INotifier * pNotifier /* = NULL */, IScanProfiler * pProfiler /* = NULL */ ) :
	CFileScanner( pEngine, pNotifier, pProfiler ),
    CCleanScanner( pEngine, pNotifier, pProfiler ),
	m_bFlushed( TRUE ),
    m_pfnAlgoSelector( NULL )
{
}

/*************************************************************************
* Function Name:          COperaScanner::~COperaScanner()
* Function Description:   Destructor
*************************************************************************/
COperaScanner::~COperaScanner()
{
	FlushCookiesFile();
}

/*************************************************************************
* Function Name:          COperaScanner::prerun()
* Function Description:   
*************************************************************************/
void COperaScanner::prerun()
{
	FlushCookiesFile();

	m_OperaCookieFilesArray.RemoveAll();

	CAtlArray<CString> arrDirs;
	if( GetOperaCookiesPath( arrDirs ) ) 
	{
		RemoveDublicates( arrDirs );

		for( size_t i = 0; i < arrDirs.GetCount(); ++i )
			m_OperaCookieFilesArray[ m_OperaCookieFilesArray.Add() ].LoadFile( arrDirs[i] );
	}

	m_bFlushed = FALSE;

	CCleanScanner::prerun();
}

/*************************************************************************
* Function Name:        COperaScanner::ReinitAlgorithm()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void COperaScanner::ReinitAlgorithm()
{
    m_pfnAlgoSelector = &COperaScanner::algoStep1;
}

/*************************************************************************
* Function Name:        COperaScanner::algorithm()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void COperaScanner::algorithm()
{
    ( this->*m_pfnAlgoSelector )();
}

/*************************************************************************
* Function Name:        COperaScanner::algoStep1()
* Function Purpose:     Search Mozilla's history files
*                       
*                       
*
* Arguments:            None
* Return value:         None
*************************************************************************/
void COperaScanner::algoStep1()
{
	CAtlArray<CString> arrDirs;
	if( GetOperaHistoryPath( arrDirs ) )
	{
		RemoveDublicates( arrDirs );
		CheckDirs( arrDirs );
	}


	m_pfnAlgoSelector = &COperaScanner::algoStep2;
}

void COperaScanner::algoStep2()
{
	CAtlArray<CString> arrDirs;
	if( GetOperaVLinksPath( arrDirs ) )
	{
		RemoveDublicates( arrDirs );
		CheckDirs( arrDirs );
	}

	m_pfnAlgoSelector = &COperaScanner::algoStep3;
}

void COperaScanner::algoStep3()
{
	CAtlArray<CString> arrDirs;
	if( GetDownloadInfoPath( arrDirs ) )
	{
		RemoveDublicates( arrDirs );
		CheckDirs( arrDirs );
	}

	m_pfnAlgoSelector = &COperaScanner::algoStep4;
}

void COperaScanner::algoStep4()
{
	CAtlArray<CString> arrDirs;
	if( GetCacheFilesPathes( arrDirs ) )
	{
		CheckDirs( arrDirs );
	}

	m_pfnAlgoSelector = &COperaScanner::algoStep5;
}

void COperaScanner::algoStep5()
{
	CAtlArray<CString> arrDirs;
	if( GetMRULIstsPath( arrDirs ) )
	{
		RemoveDublicates( arrDirs );
		CheckDirs( arrDirs );
	}

	m_pfnAlgoSelector = &COperaScanner::algoStep6;
}

void COperaScanner::algoStep6()
{
	if( m_pNotifier )
	{
		m_pNotifier->CallOnScan( ScannerId(), _T("Opera cookies"), NULL );
	}

	set <CString> setURLs;

	for( size_t i = 0, i_max = m_OperaCookieFilesArray.GetCount(); i < i_max; ++i )
		m_OperaCookieFilesArray[ i ].GetCookiesSet( setURLs );

	for( set<CString>::iterator it = setURLs.begin(); it != setURLs.end(); ++it )
	{
		m_pEngine->AddItem(
			countable<IReportItem>(
				new SCustomReportItem(
					fn::bind1st1( fn::mem_fun( &COperaScanner::CleanupObject ), this ),
					ScannerId(),
					IT_OTHER,
					SEVERITY_MEDIUM,
					STATUS_FOUND,
					_T("http://") + *it ) ) );
	}

	StopJustMe();
}

// Opera(4, 7, 8) Directory
bool COperaScanner::GetOperaDir(  CAtlArray<CString> & arrDir )
{
	arrDir.RemoveAll();

	GetOperaDirByRegKey( _T("Last Directory3"),     arrDir );
	GetOperaDirByRegKey( _T("Last Beta Directory"), arrDir );

	return (arrDir.GetCount() != 0);
}

void COperaScanner::GetOperaDirByRegKey( CString strKeyValue, CAtlArray<CString> & arrDir )
{
	BOOL bMultiUser = TRUE;

	CPath strOperaDir;
	CPath strUserDir;
	CString strOperaProductName( _T("Opera") );

	CRegKey regKey;
	LONG lRes = regKey.Open(HKEY_CURRENT_USER, _T("Software\\Opera Software"), KEY_READ);
	if (ERROR_SUCCESS == lRes)
	{
		CPath path;
		ULONG nSize = MAX_PATH;
		lRes = regKey.QueryStringValue( strKeyValue,
			path.m_strPath.GetBuffer(nSize), &nSize);

		path.m_strPath.ReleaseBuffer();
		if (ERROR_SUCCESS == lRes)
		{
			strOperaDir = path;

			CPath pathIni(path);
			pathIni.Append(_T("operadef6.ini"));

			bMultiUser = ::GetPrivateProfileInt(_T("System"),
				_T("Multi User"), bMultiUser, pathIni);

			int nPos = strOperaDir.m_strPath.ReverseFind( '\\' );
			if( (-1 != nPos) && (nPos != strOperaDir.m_strPath.GetLength()-1 ) )//найдено, и это не последний символ
			{
				strOperaProductName = strOperaDir.m_strPath.Mid( nPos );
				strOperaProductName.Trim( _T("\\") );// на вс€кий случай
			}
		}

	}
	else return;

	if( bMultiUser )
	{
		HRESULT hr = GetAppDataPath(strUserDir);
		if (SUCCEEDED(hr))
		{
			CString strOperaDirTemplatePath;
			strOperaDirTemplatePath.Format( _T("\\Opera\\%s\\Profile"), strOperaProductName );
			strUserDir.Append( strOperaDirTemplatePath );
			arrDir.Add(strUserDir);
		}
	}
	else
	{
		strOperaDir.Append(_T("\\Profile"));
		arrDir.Add(strOperaDir);
	}
}

bool COperaScanner::GetOperaHistoryPath( CAtlArray<CString> &arrPathes )
{
	bool bResult = false;

	//  Gettting Dirs for Opera 
	if( GetOperaDir( arrPathes ) )
	{
		for( size_t i = 0; i < arrPathes.GetCount(); ++i )
			arrPathes[i] += _T("\\global.dat");
	}

	bResult = !arrPathes.IsEmpty();

	return bResult;
}

bool COperaScanner::GetOperaVLinksPath( CAtlArray<CString> &arrPathes )
{
	bool bResult = false;

	//  Gettting Dirs for Opera 
	if( GetOperaDir( arrPathes ) )
	{
		size_t nCount = arrPathes.GetCount();
		for(size_t i=0; i<nCount; i++)
		{
			arrPathes.Add( arrPathes[i] + _T("\\sessions\\autosave.win") );
			arrPathes[i] += _T("\\vlink4.dat");
		}
	}	

	bResult = !arrPathes.IsEmpty();

	return bResult;
}

bool COperaScanner::GetDownloadInfoPath( CAtlArray<CString> &arrPathes )
{
	bool bResult = false;

	//  Gettting Dirs for Opera 
	if( GetOperaDir( arrPathes ) )
	{
		for(size_t i=0; i<arrPathes.GetCount(); i++)
			arrPathes[i] += _T("\\download.dat");
	}	

	bResult = !arrPathes.IsEmpty();

	return bResult;
}

bool COperaScanner::GetCacheFilesPathes( CAtlArray<CString> &arrPathes )
{
	bool bResult = false;
	CAtlArray<CString> arrDirs;

	//  Gettting Dirs for Opera 
	if( GetOperaDir( arrDirs ) )
	{
		RemoveDublicates( arrDirs );

		for(size_t i=0; i<arrDirs.GetCount(); i++)
		{
			arrDirs[i] += _T("\\Cache4");
			CPath path( arrDirs[i] );

			// get files in this directory ====
			if( path.IsDirectory() )
				GetFilesFromDirectory(path, arrPathes );
			// ================================
		}
	}	

	bResult = !arrPathes.IsEmpty();

	return bResult;
}

bool COperaScanner::GetMRULIstsPath( CAtlArray<CString> &arrPathes )
{
	bool bResult = false;

	//  Gettting Dirs for Opera 
	if( GetOperaDir( arrPathes ) )
	{
		for(size_t i=0; i<arrPathes.GetCount(); i++)
			arrPathes[i] += _T("\\opera.dir");
	}	

	bResult = !arrPathes.IsEmpty();

	return bResult;
}

bool COperaScanner::GetOperaCookiesPath( CAtlArray<CString> &arrPathes )
{
	bool bResult = false;

	//  Gettting Dirs for Opera 
	if( GetOperaDir( arrPathes ) )
	{
		for(size_t i=0; i<arrPathes.GetCount(); i++)
			arrPathes[i] += _T("\\cookies4.dat");
	}	

	bResult = !arrPathes.IsEmpty();

	return bResult;
}

/*************************************************************************
* Function Name:        COperaScanner::CleanupObject()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void COperaScanner::CleanupObject( countable< IReportItem > item )
{
	if( item->m_Type == IT_FILE || item->m_Type == IT_DIRECTORY )
		CFileScanner::CleanupObject( item );
	else if( item->m_Type == IT_OTHER )
	{
		ON_FIX( item );

		SCustomReportItem * pItem = item->GetAsCustomItem();

		if( !pItem )
		{
			ATLASSERT( FALSE );
			item->m_Status = STATUS_CLEAN_FAILED;
			ON_FIX( item );
			return;
		}

		for( size_t i = 0, i_max = m_OperaCookieFilesArray.GetCount(); i < i_max; ++i )
			m_OperaCookieFilesArray[ i ].DeleteCookie( pItem->m_strCustomDescription );

		item->m_Status = STATUS_CLEANED;

		ON_FIX( item );
	}
	else
		ATLASSERT( FALSE );
}

/*************************************************************************
* Function Name:        COperaScanner::FlushCookiesFile()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void COperaScanner::FlushCookiesFile()
{
	if( m_bFlushed )
		return;

	CAtlArray<CString> arrDirs;
	if( GetOperaCookiesPath( arrDirs ) ) 
	{
		RemoveDublicates( arrDirs );

		for( size_t i = 0, i_max = m_OperaCookieFilesArray.GetCount(); i < i_max; ++i )
			m_OperaCookieFilesArray[ i ].SaveFile( arrDirs[ i ] );
	}

	m_bFlushed = TRUE;
}
