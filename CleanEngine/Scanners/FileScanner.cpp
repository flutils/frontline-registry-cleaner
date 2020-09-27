#include "StdAfx.h"
#include "FileScanner.h"
#include "CleanEngine.h"
#include "IndexDatFile.h"
#include "IniFile.h"
#include <AtlPath.h>
#include "AtlFileFind.h"
#include <time.h>

CFileScanner::CFileScanner( CCleanEngine * pEngine, INotifier * pNotifier /* = NULL */, IScanProfiler * pProfiler /* = NULL */ ) :
	CCleanScanner( pEngine, pNotifier, pProfiler )
{
}

/*************************************************************************
* Function Name:          CFileScanner::CleanupObject()
* Function Purpose:       Erase item and all sub-items by it Id.
*
* Arguments:              itemId - Identifier of item that is need to be
*                         erased.
*
* Return value:           None.
*************************************************************************/
void CFileScanner::CleanupObject( countable< IReportItem > item )
{
	SFileReportItem * pItem;
	pItem = item->GetAsFileItem();

	item->m_Status = STATUS_CLEANING;

	if( m_pNotifier )
		m_pNotifier->OnItemFix( item );

	BOOL bUseSD = FALSE;
	if( m_pEngine )
		bUseSD = InterlockedCompareExchange( &m_pEngine->m_bUsePostponedClean, FALSE, FALSE );

	DWORD dwRetries;

	if( m_pProfiler )
		m_pProfiler->GetDWORD( ID_SECURE_LEVEL, dwRetries );

	switch( item->m_Type )
	{
	case IT_FILE:
		{
			CPath fileFullPath( pItem->m_strDirectory );
			fileFullPath.Append( pItem->m_strFile );

			if( S_OK != IsPathExist( fileFullPath.m_strPath ) )
			{
				item->m_Status = STATUS_CLEANED;
			}
			else
			{
				if( bUseSD )
				{
					item->m_Status = ( S_OK == SecureDeleteFile( fileFullPath.m_strPath, static_cast<UINT>( dwRetries ) ) ) ?
						STATUS_CLEANED : STATUS_CLEAN_FAILED;
				}
				else
				{
					DeleteFile( fileFullPath.m_strPath ) ?
						item->m_Status = STATUS_CLEANED:
						item->m_Status = STATUS_CLEAN_FAILED;
				}
			}
		} break;

	case IT_DIRECTORY:
		{
			if( S_OK != IsPathExist( pItem->m_strDirectory ) )
			{
				item->m_Status = STATUS_CLEANED;
			}
			else
			{
				if( bUseSD )
				{
					item->m_Status = ( S_OK == SecureDeleteFolder( pItem->m_strDirectory, static_cast<UINT>( dwRetries ) ) ) ?
						STATUS_CLEANED : STATUS_CLEAN_FAILED;
				}
				else
				{
					S_OK == DeleteFolder( pItem->m_strDirectory ) ?
						item->m_Status = STATUS_CLEANED:
						item->m_Status = STATUS_CLEAN_FAILED;
				}
			}
		} break;

	default:

		// Not FileScanner threat
		ATLASSERT( FALSE );
	}

	if( m_pNotifier )
		m_pNotifier->OnItemFix( item );
}

/*************************************************************************
* Function Name:    CFileScanner::RecursivelyAddDirectory()
*
* Function Description:   
*                   Recursively add directory and all it 
*                   files and subdirectoryies to scan result list. 
*
* Arguments:        rstrDirName - Full path to directory is added.
*                   bAddDirs - If true directories also will be added.
*                   nDepth - Recurse level.
*
* Return value:     None
*
*************************************************************************/
void CFileScanner::RecursivelyAddDirectory(
	ce_tstring&         rstrDirName,
	const bool          bAddDirs,
	const unsigned      nDepth)
{
    if( nDepth > MaxRecurseLevel )
    {
        return;
    }

    if( m_pNotifier )
    {
        m_pNotifier->CallOnScan( ScannerId(), rstrDirName );
    }

    // Add directory to list
    //
    if( bAddDirs )
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
					rstrDirName ) ) );
    }

    // Enumerate files in directory.
    //
	int size = rstrDirName.GetLength();
    rstrDirName += _T( "\\*.*" );
    
    WIN32_FIND_DATA sFD;
	HANDLE hSearch = FindFirstFile( rstrDirName.GetString(), &sFD );
	rstrDirName.GetBufferSetLength( size );

	if( INVALID_HANDLE_VALUE == hSearch )
	{
		return;
	}
    
    do
    {
		if( ABANDONNED == InterrogateMngr() )
		{
			break;
		}

        if( sFD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
        {
            if( ( _tcsicmp( _T( "."  ), sFD.cFileName ) != 0 ) && 
                ( _tcsicmp( _T( ".." ), sFD.cFileName ) != 0 ) )
            {
                rstrDirName += _T( "\\" );
                rstrDirName += sFD.cFileName;

                RecursivelyAddDirectory( rstrDirName,
                                         bAddDirs,
                                         nDepth + 1 );

				rstrDirName.GetBufferSetLength( size );
            }
        }
        else
        {

			if( m_pNotifier )
			{            
				m_pNotifier->CallOnScan( ScannerId(), rstrDirName, sFD.cFileName );
			}

			m_pEngine->AddItem(
				countable<IReportItem>(
					new SFileReportItem(
						fn::bind1st1( fn::mem_fun( &CFileScanner::CleanupObject ), this ),
						ScannerId(),
						IT_FILE,
						SEVERITY_MEDIUM,
						STATUS_FOUND,
						sFD.cFileName,
						rstrDirName ) ) );
        }
    }
    while( FindNextFile( hSearch, &sFD ));

    FindClose( hSearch );
}

/*************************************************************************
* Function Name:    CFileScanner::GetFilesFromDirectory()
*
*************************************************************************/
void CFileScanner::GetFilesFromDirectory( CPath pathTmp, CAtlArray<CString> &arrPathes )
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
			if (((CString)_lpfd.cFileName).Trim()!="."&& (((CString)_lpfd.cFileName).Trim()!="..")) 
			{
				if( ABANDONNED == InterrogateMngr() )
				{
					break;
				}

				if( m_pNotifier )
				{
					m_pNotifier->CallOnScan( ScannerId(), strCurrentFile, NULL );
				}

				arrPathes.Add( strCurrentFile );
			}
		} 
		while( FindNextFile( hFileSearch, &_lpfd ) != 0 );

		FindClose(hFileSearch);
	}
}

/*************************************************************************
* Function Name:    CFileScanner::CheckDirs()
*
*************************************************************************/
void CFileScanner::CheckDirs( CAtlArray<CString> & arrDirs )
{
	for( size_t i = 0, max_i = arrDirs.GetCount(); i < max_i; ++i )
	{
		CString strDir = arrDirs[i];

		if( ABANDONNED == InterrogateMngr() )
		{
			return;
		}

		if( m_pNotifier )
			m_pNotifier->OnScan( ScannerId(), strDir, NULL );

		if ( !::PathFileExists( strDir ) )
			continue;

		CPath path( strDir );

		int iPos = path.FindFileName();
		strDir = path.m_strPath.Left( iPos );
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
}

/*************************************************************************
* Function Name:    CFileScanner::RemoveDublicates()
*
*************************************************************************/
void CFileScanner::RemoveDublicates( CAtlArray<CString> & arrDirs )
{
	for( size_t i = 0; i < arrDirs.GetCount(); ++i )
	{
		CString strDir = arrDirs[i];
		for( size_t j = i + 1; j < arrDirs.GetCount(); )
		{
			if( strDir == arrDirs[j] )
				arrDirs.RemoveAt( j );
			else ++j;
		}
	}
}

/*************************************************************************
* Function Name:    CFileScanner::RecursivelyScanByMask()
*
*************************************************************************/
void CFileScanner::RecursivelyScanByMask(	std::string & strCurrentDir,
							const std::list<std::string> & lstExtMask,
							const unsigned nDepth /* = 0 */ )
{
    if( nDepth > MaxRecurseLevel )
    {
        return;
    }

    if( m_pNotifier )
    {
		m_pNotifier->CallOnScan( ScannerId(), strCurrentDir.c_str() );
    }

    // Enumerate files in directory.
    //
	size_t size = strCurrentDir.size();
    strCurrentDir += _T( "\\*.*" );
    
    WIN32_FIND_DATA sFD;
	HANDLE hSearch = FindFirstFile( strCurrentDir.c_str(), &sFD );
	strCurrentDir.resize( size );

	if( INVALID_HANDLE_VALUE == hSearch )
	{
		return;
	}
    
    do
    {
		if( ABANDONNED == InterrogateMngr() )
		{
			break;
		}

        if( sFD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
        {
            if( ( _tcsicmp( _T( "."  ), sFD.cFileName ) != 0 ) && 
                ( _tcsicmp( _T( ".." ), sFD.cFileName ) != 0 ) )
            {
                strCurrentDir += _T( "\\" );
                strCurrentDir += sFD.cFileName;

                RecursivelyScanByMask(	strCurrentDir,
										lstExtMask,
										nDepth + 1 );

				strCurrentDir.resize( size );
            }
        }
        else
        {

			if( m_pNotifier )
			{
				m_pNotifier->CallOnScan( ScannerId(), strCurrentDir.c_str(), sFD.cFileName );
			}

			for( std::list<std::string>::const_iterator it = lstExtMask.begin(), itEnd = lstExtMask.end(); it != itEnd; ++it )
			{
				if( CPath( sFD.cFileName ).MatchSpec( it->c_str() ) )
				{
					//m_pEngine->AddItem(
					//	countable<IReportItem>(
					//		new SFileReportItem(
					//			fn::bind1st1( fn::mem_fun( &CFileScanner::CleanupObject ), this ),
					//			ScannerId(),
					//			IT_FILE,
					//			SEVERITY_MEDIUM,
					//			STATUS_FOUND,
					//			sFD.cFileName,
					//			strCurrentDir.c_str() ) ) );
					OnScanConditionDone(strCurrentDir.c_str(),sFD.cFileName);

					break;
				}
			}
        }
    }
    while( FindNextFile( hSearch, &sFD ));

    FindClose( hSearch );
}

HRESULT CFileScanner::_SecureDeleteFile(LPCSTR lpFileName, UINT uiRetries)
{
	HRESULT hr = S_OK;

	do
	{
		CPath path(lpFileName);
		path.StripPath();
		if (0 == path.m_strPath.CompareNoCase(_T("desktop.ini")))
		{
			// we won't delete this file
			break;
		}
		else if (0 == path.m_strPath.CompareNoCase(_T("index.dat")))
		{
			CIndexDatFile file;
			hr = file.Load(lpFileName);
			if (SUCCEEDED(hr))
				hr = file.CleanHistory();

			break;
		}

		if (!::SetFileAttributes(lpFileName, FILE_ATTRIBUTE_NORMAL))
		{
			hr = AtlHresultFromLastError();
			//break;
		}

		if (0 < uiRetries)
		{
			CAtlFile file;
			hr = file.Create(lpFileName, GENERIC_WRITE, FILE_SHARE_READ |
				FILE_SHARE_WRITE /*| FILE_SHARE_DELETE*/, OPEN_EXISTING);
			if (FAILED(hr))
				break;

			srand(static_cast<unsigned int>(time(NULL)));

			UINT const CHUNK_SIZE = 4 * 1024;

			static bool bInited = false;
			static BYTE arrZeros[CHUNK_SIZE] = { 0 };		
			static BYTE arrFfs[CHUNK_SIZE];
			static BYTE arrRnd[CHUNK_SIZE];

			if (!bInited)
			{
				for (size_t i = 0; i < __countof(arrFfs); ++i)
					arrFfs[i] = 0xFF;

				bInited = true;
			}

			ULONGLONG ullFileSize = 0;
			hr = file.GetSize(ullFileSize);

			for (UINT i = 0; SUCCEEDED(hr) && (i < uiRetries); ++i)
			{
				hr = file.Seek(0, FILE_BEGIN);

				for (ULONGLONG ullOffset = 0; SUCCEEDED(hr) &&
					(ullOffset < ullFileSize); ullOffset += sizeof(arrZeros))
				{
					hr = file.Write(arrZeros, sizeof(arrZeros));
				}

				if (FAILED(hr))
					break;

				hr = file.Flush();
				if (FAILED(hr))
					break;

				hr = file.Seek(0, FILE_BEGIN);

				for (ULONGLONG ullOffset = 0; SUCCEEDED(hr) &&
					(ullOffset < ullFileSize); ullOffset += sizeof(arrFfs))
				{
					hr = file.Write(arrFfs, sizeof(arrFfs));
				}

				if (FAILED(hr))
					break;

				hr = file.Flush();
				if (FAILED(hr))
					break;

				hr = file.Seek(0, FILE_BEGIN);

				for (ULONGLONG ullOffset = 0; SUCCEEDED(hr) &&
					(ullOffset < ullFileSize); ullOffset += sizeof(arrFfs))
				{
					for (size_t i = 0; i < __countof(arrRnd); ++i)
						arrRnd[i] = static_cast<BYTE>((static_cast<__int64>(rand()) * 0xFF) / RAND_MAX);

					hr = file.Write(arrRnd, sizeof(arrRnd));
				}

				if (FAILED(hr))
					break;

				hr = file.Flush();
			}
		}

		//::DeleteFile(lpFileName);
		//hr = AtlHresultFromLastError();
	}
	while (false);

	if (::DeleteFile(lpFileName))
		hr = S_OK;
	else
		hr = AtlHresultFromLastError();

	DWORD dwErr = HRESULT_CODE(hr);
	if ((ERROR_FILE_NOT_FOUND == dwErr) ||
		(ERROR_PATH_NOT_FOUND == dwErr))
	{
		hr = S_FALSE;
	}

	return hr;
}

HRESULT CFileScanner::SecureDeleteFile(LPCSTR lpFileName, UINT uiRetries)
{
	HRESULT hr;
	if( FAILED( hr = _SecureDeleteFile( lpFileName, uiRetries) ) )
	{
		
		DWORD dwAttr = GetFileAttributes( lpFileName );
		if( /*!( dwAttr & FILE_ATTRIBUTE_DIRECTORY) &&*/
			( INVALID_FILE_ATTRIBUTES != dwAttr ) )
		{
			OSVERSIONINFO	ovi;
			ovi.dwOSVersionInfoSize = sizeof( ovi );
			GetVersionEx( &ovi );
			if( ovi.dwMajorVersion > 4 )
			{
				CStringW wsz( lpFileName );
				g_csStera.Delete_File( wsz.GetBuffer(), uiRetries );
				wsz.ReleaseBuffer();
			} else {
				TCHAR szShortName[MAX_PATH];
				DWORD dwLen = ::GetShortPathName(lpFileName, szShortName, __countof(szShortName));
				if (0 == dwLen) {
					return E_FAIL;
				}
				_ASSERTE(dwLen <= __countof(szShortName));
				TCHAR const lpIniFileName[] = _T("\\WinInit.ini");
				TCHAR szFilePath[MAX_PATH];
				UINT nLen = ::GetWindowsDirectory(szFilePath, __countof(szFilePath));
				_ASSERTE((nLen + __countof(lpIniFileName)) <= __countof(szFilePath));
				StringCbCopy(szFilePath + nLen, MAX_PATH - nLen, lpIniFileName);
				CIniFile inifile(szFilePath);
				LPCTSTR lpSectionName = _T("rename");
				CString strSection = inifile.ReadSection(lpSectionName);
				BOOL bEmpty = strSection.IsEmpty();
				strSection.Insert(0, _T('\0'));
				strSection.Insert(0, szShortName);
				strSection.Insert(0, _T("NUL="));
				if (bEmpty) strSection.AppendChar(_T('\0'));
				inifile.WriteSection(lpSectionName, strSection);
			}
		}
	}
	return hr;
}

HRESULT CFileScanner::SecureDeleteFolder(LPCTSTR lpFolder, UINT uiRetries)
{
	HRESULT hr = E_FAIL;

	SecureDeleteFolderItems(lpFolder, uiRetries);

	if (::RemoveDirectory(lpFolder))
		hr = S_OK;
	else
	{
		CStringW wsz( lpFolder );
		g_csStera.Delete_File( wsz.GetBuffer(), uiRetries );
		wsz.ReleaseBuffer();

/*		DWORD dwErr = ::GetLastError();
		if ((ERROR_FILE_NOT_FOUND == dwErr) ||
			(ERROR_PATH_NOT_FOUND == dwErr))
		{
			hr = S_FALSE;
		}*/
	}

	return hr;
}

void CFileScanner::SecureDeleteFolderItems(LPCTSTR lpFolder, UINT uiRetries)
{
	do
	{
		CPath szPath;
		szPath.Combine(lpFolder, _T("*.*"));

		CAtlFileFind finder;

		HRESULT hr = finder.FindFirst(szPath);

		while (S_OK == hr)
		{
			if (!finder.IsDots())
			{
				szPath.Combine(lpFolder, finder.GetFileName());

				if ( finder.IsDirectory() )
				{
					HRESULT hRes = SecureDeleteFolder(szPath, uiRetries);
				}
				else
				{
					HRESULT hRes = SecureDeleteFile(szPath, uiRetries);
				}
			}

			hr = finder.FindNext();
		}
	}

	while (false);
}

void CFileScanner::OnScanConditionDone(LPCTSTR strCurrentDir, LPCTSTR  strFileName)
{
	
	m_pEngine->AddItem(
		countable<IReportItem>(
		new SFileReportItem(
		fn::bind1st1( fn::mem_fun( &CFileScanner::CleanupObject ), this ),
		ScannerId(),
		IT_FILE,
		SEVERITY_MEDIUM,
		STATUS_FOUND,
		strFileName,
		strCurrentDir) ) );

}