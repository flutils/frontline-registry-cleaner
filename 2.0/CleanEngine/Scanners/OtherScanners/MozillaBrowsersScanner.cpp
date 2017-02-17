#include "StdAfx.h"
#include "MozillaBrowsersScanner.h"
#include "CleanEngine.h"

/*************************************************************************
* Function Name:          CMozillaBrowsersScanner::CMozillaBrowsersScanner()
* Function Description:   Constructor
*************************************************************************/
CMozillaBrowsersScanner::CMozillaBrowsersScanner( CCleanEngine * pEngine, INotifier * pNotifier /* = NULL */, IScanProfiler * pProfiler /* = NULL */ ) :
	CFileScanner( pEngine, pNotifier, pProfiler ),
    CCleanScanner( pEngine, pNotifier, pProfiler ),
	m_bFlushed( TRUE ),
    m_pfnAlgoSelector( NULL )
{
	m_MozillasCookieFilesArray[0] = NULL;
	m_MozillasCookieFilesArray[1] = NULL;
	m_MozillasCookieFilesArray[2] = NULL;
}

/*************************************************************************
* Function Name:          CMozillaBrowsersScanner::~CMozillaBrowsersScanner()
* Function Description:   Destructor
*************************************************************************/
CMozillaBrowsersScanner::~CMozillaBrowsersScanner()
{
	FlushCookiesFile();
}

/*************************************************************************
* Function Name:          CMozillaBrowsersScanner::prerun()
* Function Description:   
*************************************************************************/
void CMozillaBrowsersScanner::prerun()
{
	FlushCookiesFile();

	m_MozillasCookieFilesArray[0] = new CFFCookiesFile( FIREFOX );
	m_MozillasCookieFilesArray[1] = new CFFCookiesFile( NETSCAPE8 );
	m_MozillasCookieFilesArray[2] = new CFFCookiesFile( MOZILLA );

	m_MozillasCookieFilesArray[0]->LoadFile();
	m_MozillasCookieFilesArray[1]->LoadFile();
	m_MozillasCookieFilesArray[2]->LoadFile();

	CCleanScanner::prerun();
}

/*************************************************************************
* Function Name:        CMozillaBrowsersScanner::ReinitAlgorithm()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CMozillaBrowsersScanner::ReinitAlgorithm()
{
    m_pfnAlgoSelector = &CMozillaBrowsersScanner::algoStep1;
}

/*************************************************************************
* Function Name:        CMozillaBrowsersScanner::algorithm()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CMozillaBrowsersScanner::algorithm()
{
    ( this->*m_pfnAlgoSelector )();
}

/*************************************************************************
* Function Name:        CMozillaBrowsersScanner::algoStep1()
* Function Purpose:     Search Mozilla's history files
*                       
*                       
*
* Arguments:            None
* Return value:         None
*************************************************************************/
void CMozillaBrowsersScanner::algoStep1()
{
	CAtlArray<CString> arrDirs;

	FF_TYPE ffTypes[] = { FIREFOX, NETSCAPE8, MOZILLA };

	// Not implemented
	CString strProfile;

	for( int i = 0; i < __countof(ffTypes); ++i )
	{
		arrDirs.RemoveAll();
		if( GetMozillaHistoryDirs( ffTypes[ i ], arrDirs, strProfile ) )
		{
			RemoveDublicates( arrDirs );
			CheckDirs( arrDirs );
		}

		arrDirs.RemoveAll();
		if( GetMozillaCachePath( ffTypes[ i ], arrDirs, strProfile ) )
		{
			RemoveDublicates( arrDirs );
			CheckDirs( arrDirs );
		}

		arrDirs.RemoveAll();
		if( GetPasswordsFilesPath( ffTypes[ i ], arrDirs, strProfile ) )
		{
			RemoveDublicates( arrDirs );
			CheckDirs( arrDirs );
		}
	}

	arrDirs.RemoveAll();
	if( GetTypedURLsFilePath( arrDirs, strProfile ) )
	{
		RemoveDublicates( arrDirs );
		CheckDirs( arrDirs );
	}

    m_pfnAlgoSelector = &CMozillaBrowsersScanner::algoStep2;
}

void CMozillaBrowsersScanner::algoStep2()
{
	FF_TYPE ffTypes[] = { FIREFOX, NETSCAPE8, MOZILLA };

	// Not implemented
	CString strProfile;

	for( int i = 0; i < 3 /*__countof(ffTypes)*/; ++i )
	{
		if( ABANDONNED == InterrogateMngr() )
		{
			return;
		}

		if( m_pNotifier )
			m_pNotifier->OnScan( ScannerId(), _T("Mozilla cookies"), NULL );

		if( ABANDONNED == InterrogateMngr() )
		{
			return;
		}

		CAtlArray<SRecord> arrRecords;
		std::set<CString> setURLs;

		arrRecords.Append( m_MozillasCookieFilesArray[i]->m_arrRecords );

		for(size_t j = 0; j < arrRecords.GetCount(); ++j )
		{
			setURLs.insert( arrRecords[j].m_strDomain );
		}

		for( set<CString>::iterator it = setURLs.begin(); it != setURLs.end(); ++it )
		{
			CString strDescr = *it;
			strDescr.TrimLeft( _T(".") );

			int iOldLen = strDescr.GetLength();

			strDescr.GetBufferSetLength( iOldLen + 2 );
			strDescr.SetAt( iOldLen, _T('\0') );

			if( 0 == i )
				strDescr.SetAt( iOldLen + 1, _T('F') );
			else if( 1 == i )
				strDescr.SetAt( iOldLen + 1, _T('N') );
			else if( 2 == i )
				strDescr.SetAt( iOldLen + 1, _T('M') );

			m_pEngine->AddItem(
				countable<IReportItem>(
					new SCustomReportItem(
						fn::bind1st1( fn::mem_fun( &CMozillaBrowsersScanner::CleanupObject ), this ),
						ScannerId(),
						IT_OTHER,
						SEVERITY_MEDIUM,
						STATUS_FOUND,
						strDescr ) ) );
		}
	}

	StopJustMe();
}

bool CMozillaBrowsersScanner::GetMozillaHistoryDirs( FF_TYPE ffType, CAtlArray<CString> & arrDirs, CString strProfile )
{
	bool bResult = false;

	if( ffType == FIREFOX )
	{
		if( GetFirefoxDirs( arrDirs, strProfile ) )
		{
			for(size_t i=0; i<arrDirs.GetCount(); i++)
				arrDirs[i] += ( _T("\\history.dat") );
		}
	}

	if( ffType == NETSCAPE8 )
	{
		if( GetNetscapeDirs( arrDirs, strProfile ) )
		{
			for(size_t i=0; i<arrDirs.GetCount(); i++)
				arrDirs[i] += ( _T("\\history.dat") );
		}
	}

	if( ffType == MOZILLA )
	{		
		if( GetMozillaDirs( arrDirs, strProfile ) )
		{
			for(size_t i=0; i<arrDirs.GetCount(); i++)
				arrDirs[i] += ( _T("\\history.dat") );
		}
	}		

	bResult = !arrDirs.IsEmpty();

	return bResult;
}

bool CMozillaBrowsersScanner::GetMozillaDirs(  CAtlArray<CString> & arrDirs, CString strProfile )
{
	bool bResult = false;

	CPath path;
	HRESULT hr = GetAppDataPath(path);
	if (FAILED(hr))
		return false;

	CPath pathAppData = path;
	pathAppData.Append(_T("Mozilla"));

	CPath pathRegistryDat = pathAppData;
	pathRegistryDat.Append(_T("registry.dat"));

	CAtlArray<PROFILE_ITEM> arrProfiles;
	CMozillaProfile profiles( pathRegistryDat.m_strPath );
	profiles.GetProfiles( arrProfiles );

	arrDirs.RemoveAll();
	for( size_t i=0; i<arrProfiles.GetCount(); i++ )
	{
		if( strProfile.IsEmpty() || 
			arrProfiles.GetAt(i).strName.CompareNoCase(strProfile) == 0  )
		{
			arrDirs.Add( arrProfiles.GetAt(i).strPath );
		}
	}
	bResult = !arrDirs.IsEmpty();

	return bResult;
}

bool CMozillaBrowsersScanner::GetNetscapeDirs( CAtlArray<CString> & arrDirs, CString strProfile )
{
	bool bResult = false;

	CPath path;
	HRESULT hr = GetAppDataPath(path);
	if (FAILED(hr))
		return false;

	LPCTSTR lpDirs[] = {
							_T("Netscape\\NSB"),
							_T("Netscape\\Navigator")
						};

	for (size_t i = 0; i < __countof(lpDirs); ++i)
	{
		CPath pathAppData = path;
		pathAppData.Append(lpDirs[i]);

		CPath pathProfilesIni = pathAppData;
		pathProfilesIni.Append(_T("profiles.ini"));

		CString strSections = ReadSectionNamesFromIni(pathProfilesIni);
		strSections.Replace(_T('\0'), _T(';'));

		int nPos = 0;
		LPCTSTR lpDelims = _T(";");
		CString strSection = strSections.Tokenize(lpDelims, nPos);

		while (!strSection.IsEmpty())
		{
			CString strProfilePath = ReadStringFromIni(
				pathProfilesIni, strSection, _T("Path"));

			if (!strProfilePath.IsEmpty())
			{
				CString strProfileName = ReadStringFromIni(	pathProfilesIni, strSection, _T("Name"));

				if( strProfile.IsEmpty() || strProfileName.CompareNoCase(strProfile) == 0 )
				{
					strProfilePath.Replace(_T('/'), _T('\\'));

					CPath pathTmp = pathAppData;
					pathTmp.Append(strProfilePath);

					arrDirs.Add(pathTmp);
				}
			}

			strSection = strSections.Tokenize(lpDelims, nPos);
		}
	}
	bResult = !arrDirs.IsEmpty();

	return bResult;
}

bool CMozillaBrowsersScanner::GetFirefoxDirs( CAtlArray<CString> & arrDirs, CString strProfile )
{
	bool bResult = false;

	CPath path;
	HRESULT hr = GetAppDataPath(path);
	if (FAILED(hr))
		return false;

	LPCTSTR lpDirs[] = { _T("Mozilla\\FireFox") };

	for (size_t i = 0; i < __countof(lpDirs); ++i)
	{
		CPath pathAppData = path;
		pathAppData.Append(lpDirs[i]);

		CPath pathProfilesIni = pathAppData;
		pathProfilesIni.Append(_T("profiles.ini"));

		CString strSections = ReadSectionNamesFromIni(pathProfilesIni);
		strSections.Replace(_T('\0'), _T(';'));

		int nPos = 0;
		LPCTSTR lpDelims = _T(";");
		CString strSection = strSections.Tokenize(lpDelims, nPos);

		while (!strSection.IsEmpty())
		{
			CString strProfilePath = ReadStringFromIni(
				pathProfilesIni, strSection, _T("Path"));

			if (!strProfilePath.IsEmpty())
			{
				CString strProfileName = ReadStringFromIni(	pathProfilesIni, strSection, _T("Name"));

				if( strProfile.IsEmpty() || strProfileName.CompareNoCase(strProfile) == 0 )
				{
					strProfilePath.Replace(_T('/'), _T('\\'));

					CPath pathTmp = pathAppData;
					pathTmp.Append(strProfilePath);

					arrDirs.Add(pathTmp);
				}
			}

			strSection = strSections.Tokenize(lpDelims, nPos);
		}
	}
	bResult = !arrDirs.IsEmpty();

	return bResult;
}

bool CMozillaBrowsersScanner::GetMozillaCachePath( FF_TYPE ffType, CAtlArray<CString> &arrPathes, CString strProfile )
{
	bool bResult = false;
	CAtlArray<CString> arrDirs;

	if( ffType == NETSCAPE8 )
	{
		if( GetNetscapeDirs( arrDirs, strProfile ) )
		{
			for( size_t i = 0; i < arrDirs.GetCount(); ++i )
			{
				arrDirs[i] += _T("\\Cache");
				CPath path( arrDirs[i] );

				// get files in this directory ====
				if( path.IsDirectory() )
					GetFilesFromDirectory(path, arrPathes );
				// ================================
			}
		}		
	}

	if( ffType == FIREFOX )
	{
		if( GetFirefoxDirs( arrDirs, strProfile ) )
		{
			for(size_t i=0; i<arrDirs.GetCount(); i++)
			{
				arrDirs[i] += _T("\\Cache");
				CPath path( arrDirs[i] );

				// get files in this directory ====
				if( path.IsDirectory() )
					GetFilesFromDirectory(path, arrPathes );
				// ================================
			}
		}		
	}

	//  Gettting Dirs for MOZILLA 1.5
	if( ffType == MOZILLA )
	{		
		if( GetMozillaDirs( arrDirs, strProfile ) )
		{
			for(size_t i=0; i<arrDirs.GetCount(); i++)
			{
				arrDirs[i] += _T("\\Cache");
				CPath path( arrDirs[i] );

				// get files in this directory ====
				if( path.IsDirectory() )
					GetFilesFromDirectory(path, arrPathes );
				// ================================
			}
		}	
	}		

	bResult = !arrDirs.IsEmpty();

	return bResult;
}

bool CMozillaBrowsersScanner::GetTypedURLsFilePath( CAtlArray<CString> &arrPathes, CString strProfile )
{
	bool bResult = false;

	//  Gettting Dirs for MOZILLA 1.5
	if( GetMozillaDirs( arrPathes, strProfile ) )
	{
		for(size_t i=0; i<arrPathes.GetCount(); i++)
			arrPathes[i] += _T("\\localstore.rdf");
	}	

	bResult = !arrPathes.IsEmpty();

	return bResult;
}

bool CMozillaBrowsersScanner::GetPasswordsFilesPath( FF_TYPE ffType, CAtlArray<CString> &arrPathes, CString strProfile )
{
	bool bResult = false;
	CAtlArray<CString> arrDirs;

	if( ffType == FIREFOX )
	if( GetFirefoxDirs( arrDirs, strProfile ) )
	{
		for(size_t i=0; i<arrDirs.GetCount(); i++)
		{
			CPath path( arrDirs[i] );
			
			if( path.IsDirectory() )
				arrPathes.Add( path.m_strPath + _T("\\signons.txt") );
			//===========================================================
		}
	}

	//  Gettting Dirs for MOZILLA 1.5
	if( ffType == MOZILLA )
	if( GetMozillaDirs( arrDirs, strProfile ) )
	{
		for(size_t i=0; i<arrDirs.GetCount(); i++)
		{
			CPath path( arrDirs[i] );

			// try to find files with "*.s" mask in directory ===========
			if( path.IsDirectory() )
				FindNeededFiles( path, arrPathes ); 
			//===========================================================
		}
	}	

	bResult = !arrPathes.IsEmpty();

	return bResult;
}

void CMozillaBrowsersScanner::FindNeededFiles( CPath pathTmp, CAtlArray<CString> &arrPathes )
{
	WIN32_FIND_DATA _lpfd;
	CString strPath = pathTmp.m_strPath;
	if (strPath[strPath.GetLength()-1] != '\\')
		strPath = strPath + '\\';
	HANDLE hFileSearch = FindFirstFile(strPath+"*.*", &_lpfd);
	if (hFileSearch != INVALID_HANDLE_VALUE)
	{
		do 
		{
			CString strCurrentFile = (strPath + _lpfd.cFileName).GetBuffer();
			if (((CString)_lpfd.cFileName).Trim()!="."&& (((CString)_lpfd.cFileName).Trim()!="..")) // if file
			{
				if( ((CString)_lpfd.cFileName).Right(2) == _T(".s") )
					arrPathes.Add( strCurrentFile );
			}
		} 
		while(FindNextFile(hFileSearch, &_lpfd)!=0);

		FindClose(hFileSearch);
	}
}

void CMozillaBrowsersScanner::GetCookiesSet( FF_TYPE ffType, std::set<CString> & setDeleteCookies, CString strProfile ) const
{
	setDeleteCookies.clear();

	CFFCookiesFile ffCookies( ffType );	
	HRESULT hr = ffCookies.LoadFile( strProfile );	
	if( FAILED(hr) )
		return ;

	CAtlArray<SRecord> arrRecords;
	set <CString> setURLs;

	for(size_t i=0; i<ffCookies.m_arrRecords.GetCount(); i++)
	{
		setDeleteCookies.insert( ffCookies.m_arrRecords[i].m_strDomain );
	}
}

/*************************************************************************
* Function Name:        CMozillaBrowsersScanner::CleanupObject()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CMozillaBrowsersScanner::CleanupObject( countable< IReportItem > item )
{
	if( item->m_Type == IT_FILE || item->m_Type == IT_DIRECTORY )
		CFileScanner::CleanupObject( item );
	else if( item->m_Type == IT_OTHER )
	{
		item->m_Status = STATUS_CLEANING;
		ON_FIX( item );

		SCustomReportItem * pItem = item->GetAsCustomItem();

		if( !pItem )
		{
			ATLASSERT( FALSE );
			item->m_Status = STATUS_CLEAN_FAILED;
			ON_FIX( item );
			return;
		}

		int iTokenPos = pItem->m_strCustomDescription.Find( _T('\0') );

		CString strUrl = pItem->m_strCustomDescription.Left( iTokenPos );
		CString strProfile;

		TCHAR cType = pItem->m_strCustomDescription[ pItem->m_strCustomDescription.GetLength() - 1 ];

		int iIndex;

		if( _T('F') == cType )
			//ffType = FIREFOX;
			iIndex = 0;
		else if( _T('N') == cType )
			//ffType = NETSCAPE8;
			iIndex = 1;
		else if( _T('M') == cType )
			//ffType = MOZILLA;
			iIndex = 2;

//		long lMax = static_cast<long>( m_MozillasCookieFilesArray[iIndex]->m_arrRecords.GetCount() );
		for( long l = 0; l < static_cast<long>( m_MozillasCookieFilesArray[iIndex]->m_arrRecords.GetCount() ); ++l )
		{
			CString strDomain = m_MozillasCookieFilesArray[iIndex]->m_arrRecords[l].m_strDomain;

			strDomain.TrimLeft( _T(".") );
			strDomain.Trim();

			if( strDomain.GetLength() < 2 )
				continue;

			if( 0 == strUrl.CompareNoCase( strDomain ) )
			{
				m_MozillasCookieFilesArray[iIndex]->m_arrRecords.RemoveAt( l );
				--l;
				m_bFlushed = FALSE;
				continue;
			}
		}

		item->m_Status = STATUS_CLEANED;

		ON_FIX( item );
	}
	else ATLASSERT( FALSE );
}

/*************************************************************************
* Function Name:        CMozillaBrowsersScanner::FlushCookiesFile()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CMozillaBrowsersScanner::FlushCookiesFile()
{
	if( m_bFlushed )
		return;

	m_MozillasCookieFilesArray[0]->SaveFile();
	m_MozillasCookieFilesArray[1]->SaveFile();
	m_MozillasCookieFilesArray[2]->SaveFile();

	if( m_MozillasCookieFilesArray[0] )
		delete m_MozillasCookieFilesArray[0];

	if( m_MozillasCookieFilesArray[1] )
		delete m_MozillasCookieFilesArray[1];

	if( m_MozillasCookieFilesArray[2] )
		delete m_MozillasCookieFilesArray[2];

	m_bFlushed = TRUE;
}
