#include "StdAfx.h"
#include "MessengerScanner.h"
#include "CleanEngine.h"

/*************************************************************************
* Function Name:          CMessengerScanner::CMessengerScanner()
* Function Description:   Constructor
*************************************************************************/
CMessengerScanner::CMessengerScanner( CCleanEngine * pEngine, INotifier * pNotifier /* = NULL */, IScanProfiler * pProfiler /* = NULL */ ) :
    CCleanScanner( pEngine, pNotifier, pProfiler ),
	CFileScanner( pEngine, pNotifier, pProfiler ),
    m_pfnAlgoSelector( NULL )
{
}

/*************************************************************************
* Function Name:          CMessengerScanner::~CMessengerScanner()
* Function Description:   Destructor
*************************************************************************/
CMessengerScanner::~CMessengerScanner()
{
}

/*************************************************************************
* Function Name:        CMessengerScanner::ReinitAlgorithm()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CMessengerScanner::ReinitAlgorithm()
{
    m_pfnAlgoSelector = &CMessengerScanner::algoStep1;
}

/*************************************************************************
* Function Name:        CMessengerScanner::algorithm()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CMessengerScanner::algorithm()
{
    ( this->*m_pfnAlgoSelector )();
}

/*************************************************************************
* Function Name:        CMessengerScanner::algoStep1()
* Function Purpose:     
*                       
*                       
*
* Arguments:            None
* Return value:         None
*************************************************************************/
void CMessengerScanner::algoStep1()
{
	TCHAR tszAppData[MAX_PATH];
		
	if( !SHGetSpecialFolderPath( 0, tszAppData, CSIDL_APPDATA, FALSE ) )
		tszAppData[0] = 0;

	if( tszAppData[0] )
	{
		CString szFolder;
		size_t t;

		szFolder = tszAppData;
		szFolder += _T("\\ICQ\\ICQPro\\Temp\\" );

		RecursivelyAddDirectory( szFolder, false );

		//Lite
		CAtlArray< CString >  aUINs;
		GetICQUINs( aUINs );
		t = aUINs.GetCount();

		std::list<std::string> lstExtMask;
		lstExtMask.push_back( _T("*tmp") );

		while( t )
		{
			szFolder = tszAppData;
			szFolder += _T("\\ICQLite\\Bartcache\\" );
			szFolder += aUINs[--t];
			szFolder += _T("\\Temp\\");

			RecursivelyScanByMask( std::string( szFolder.GetString() ), lstExtMask );
		}
	}

    m_pfnAlgoSelector = &CMessengerScanner::algoStep2;
}

/*************************************************************************
* Function Name:        CMessengerScanner::algoStep2()
* Function Purpose:     
*                       
*                       
*						
* Arguments:            None
* Return value:         None
*************************************************************************/
void CMessengerScanner::algoStep2()
{
	HANDLE hFind;
	TCHAR tszICQInstallPath[MAX_PATH];
	GetICQInstallPath( tszICQInstallPath );

	CString szFolder, szSubF;
	size_t t;

	std::list<std::string> lstExtMask;
	lstExtMask.push_back( _T("*xml") );

	CAtlArray< CString >  arrUINList;
	GetICQUINs( arrUINList );

	TCHAR tszAppData[MAX_PATH];
		
	if( !SHGetSpecialFolderPath( 0, tszAppData, CSIDL_APPDATA, FALSE ) )
		tszAppData[0] = 0;

	for( t = 0; t < arrUINList.GetCount(); ++t )
	{
		//ICQ Lite(version 9)
		szFolder = tszAppData;
		szFolder += _T("\\ICQLite\\HistoryDB\\");
		szFolder += arrUINList[t];
		szFolder += _T("\\");
		szSubF = szFolder;
		szSubF += _T("*.");
		WIN32_FIND_DATA wfd;
		hFind = FindFirstFile( szSubF.GetString(), &wfd);
		if ( hFind != INVALID_HANDLE_VALUE) 
		{
			do
				if( ( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) &&
					( isdigit( wfd.cFileName[0] ) )  )

				{
					szSubF = szFolder;
					szSubF += wfd.cFileName;
					szSubF += _T("\\");

					RecursivelyScanByMask( std::string( szFolder.GetString() ), lstExtMask );
				}
			while( FindNextFile( hFind, &wfd ) );
			FindClose( hFind );
		}
		//ICQ 2003 Pro
		if( tszICQInstallPath[0] )
		{
			szSubF = tszICQInstallPath;
			//there can be 2000a, 2000b, 2001a, 2002a, 2003a also
			szSubF += _T("\\2003b\\");
			szSubF += arrUINList[t];
			//szSubF += _T("\\Messages");
			//szSubF += arrUINList[t];
			//szSubF += _T(".fpt");

			CString szName( _T("Messages") );
			szName += arrUINList[t];
			szName += _T(".fpt");

			WIN32_FILE_ATTRIBUTE_DATA wfad;
			if( ( GetFileAttributesEx( szSubF + _T("\\") + szName, GetFileExInfoStandard, (LPVOID)&wfad ) ) &&
				( !( wfad.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) ) &&
				( wfad.dwFileAttributes != INVALID_FILE_ATTRIBUTES ) )
			{
				m_pEngine->AddItem(
					countable<IReportItem>(
						new SFileReportItem(
							fn::bind1st1( fn::mem_fun( &CFileScanner::CleanupObject ), this ),
							ScannerId(),
							IT_FILE,
							SEVERITY_MEDIUM,
							STATUS_FOUND,
							szName,
							szSubF ) ) );
			}
		}
	}

    m_pfnAlgoSelector = &CMessengerScanner::algoStep3;
}

const TCHAR g_tsz_logs_[] = _T("\\logs\\");

/*************************************************************************
* Function Name:        CMessengerScanner::algoStep3()
* Function Purpose:     
*                       
*                       
*						
* Arguments:            None
* Return value:         None
*************************************************************************/
void CMessengerScanner::algoStep3()
{
	CString tszInstallPath;
	GetTrillianInstallPath( tszInstallPath );
	if( tszInstallPath.IsEmpty() )
	{
		StopJustMe();
		return;
	}

	size_t t = 0;

	CString szLogFolder;

	std::list<std::string> lstExtMask;
	lstExtMask.push_back( _T("*xml") );
	lstExtMask.push_back( _T("*log") );

	//spy have trillian 3.0 with one profile with log folder in folder "default"
	szLogFolder = tszInstallPath;
	szLogFolder += _T("\\users\\default\\logs\\");
	if( S_OK == IsPathExist( szLogFolder ) )
	{
		RecursivelyScanByMask( std::string( szLogFolder.GetString() ), lstExtMask );
	}

	TCHAR tszPath[MAX_PATH];
	SHGetSpecialFolderPath( 0, tszPath, CSIDL_PROFILE, FALSE );
	lstrcat( tszPath, _T("\\Trillian\\User Settings\\") );
	tszInstallPath += _T("\\users\\");

	CAtlArray<CString> arrProfiles;
	GetTrillianProfiles( arrProfiles );

	for( t = 0; t < arrProfiles.GetCount(); ++t )
	{
		szLogFolder = tszPath;
		szLogFolder += arrProfiles[t];
		szLogFolder += g_tsz_logs_;

		RecursivelyScanByMask( std::string( szLogFolder.GetString() ), lstExtMask );

		//check Trillian Install folder
		szLogFolder = tszInstallPath;
		szLogFolder += arrProfiles[t];
		szLogFolder += g_tsz_logs_;

		RecursivelyScanByMask( std::string( szLogFolder.GetString() ), lstExtMask );
	}

	StopJustMe();
}

/*************************************************************************
* Function Name:        CMessengerScanner::GetICQUINs()
* Function Purpose:     
*                       
*                       
*						
* Arguments:            None
* Return value:         None
*************************************************************************/
void CMessengerScanner::GetICQUINs( CAtlArray< CString > & arrUINList )
{
	//ICQ Lite (2005, version 9)
	//ICQ2003Pro (verstion 10)
	HKEY hKey;
	if( ERROR_SUCCESS == RegOpenKeyEx( HKEY_CURRENT_USER, 
		_T("Software\\Mirabilis\\ICQ\\NewOwners"), 0, KEY_READ|KEY_WRITE| DELETE, &hKey ) )
	{
		DWORD dwIndex, dwSize;
		TCHAR tszBuffer[MAX_PATH];
		dwIndex = 0;
		dwSize = sizeof( tszBuffer );
		FILETIME ft;
		while( ERROR_SUCCESS == RegEnumKeyEx( hKey, dwIndex, tszBuffer, &dwSize, 0, 0, 0, &ft ) )
		{
			arrUINList.Add( CString( tszBuffer ) );
			dwIndex++;
			dwSize = sizeof( tszBuffer );
		}
		RegCloseKey( hKey );
	}
}

/*************************************************************************
* Function Name:        CMessengerScanner::GetICQInstallPath()
* Function Purpose:     
*                       
*                       
*						
* Arguments:            None
* Return value:         None
*************************************************************************/
void CMessengerScanner::GetICQInstallPath( LPTSTR szShortPath )
{
	HKEY hKey;
	TCHAR szPath[MAX_PATH];
	szPath[0] = 0;
	szShortPath[0] = 0;
	DWORD dw = sizeof( szPath );
	if( ERROR_SUCCESS == RegOpenKeyEx( HKEY_LOCAL_MACHINE, 
		_T("SOFTWARE\\Mirabilis\\icq\\ICQPro\\Install\\General"), 
		0, KEY_READ|KEY_WRITE| DELETE, &hKey ) )
	{
		if( ERROR_SUCCESS != RegQueryValueEx( hKey, _T("InstalledDir"), 0, 0, 
			(LPBYTE)szPath, &dw ) )
			szPath[0] = 0;
		RegCloseKey( hKey );
	}
	//reserve variant
	if( !szPath[0] )
		if( ERROR_SUCCESS == RegOpenKeyEx( HKEY_LOCAL_MACHINE, 
			_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\ICQ"), 
			0, KEY_READ|KEY_WRITE| DELETE, &hKey ) )
		{
			if( ERROR_SUCCESS == RegQueryValueEx( hKey, _T("UninstallString"), 0, 0, 
				(LPBYTE)szPath, &dw ) )
			{
				TCHAR *pL = strrchr( szPath, _T('\\') );
				if( pL )
					*pL = 0;
				else
					szPath[0] = 0;
			} else
				szPath[0] = 0;
			RegCloseKey( hKey );
		}
	//delete the quotation marks
	if( szPath[0] == _T('"') )
	{
		TCHAR *pL = strrchr( szPath + 1, _T('"') );
		if( pL )
			*pL = 0;
		lstrcpy( szShortPath, szPath + 1 );
	} else
		lstrcpy( szShortPath, szPath );
}

/*************************************************************************
* Function Name:        CMessengerScanner::GetTrillianProfiles()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CMessengerScanner::GetTrillianInstallPath( CString & szShortPath )
{
	// try to take path by some registry key
	HKEY hKey;
	TCHAR szPath[MAX_PATH];
	szPath[0] = 0;
	szShortPath = _T("");
	DWORD dw = sizeof( szPath );
	if( ERROR_SUCCESS == RegOpenKeyEx( HKEY_LOCAL_MACHINE, 
		_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Trillian"), 
		0, KEY_READ|KEY_WRITE| DELETE, &hKey ) )
	{
		if( ERROR_SUCCESS == RegQueryValueEx( hKey, _T("UninstallString"), 0, 0, 
			(LPBYTE)szPath, &dw ) )
		{
			TCHAR *pL = strrchr( szPath, _T('\\') );
			if( pL )
				*pL = 0;
			else
				szPath[0] = 0;
		} else
			szPath[0] = 0;
		RegCloseKey( hKey );
	}

	// the key above may not exist( depending on install settings )
	// thus make another try....
	if( 0 == szPath[0] )
	{
		if( ERROR_SUCCESS == RegOpenKeyEx( HKEY_LOCAL_MACHINE, 
			_T("SOFTWARE\\Classes\\Trillian.SkinZip\\shell\\Install\\command"), 
			0, KEY_READ|KEY_WRITE| DELETE, &hKey ) )
		{
			if( ERROR_SUCCESS == RegQueryValueEx( hKey, NULL, 0, 0, 
				(LPBYTE)szPath, &dw ) )
			{
				TCHAR *pL = strrchr( szPath, _T('\\') );
				if( pL )
					*pL = 0;
				else
					szPath[0] = 0;
			} else
				szPath[0] = 0;
			RegCloseKey( hKey );
		}

	}

	//delete the quotation marks
	if( szPath[0] == _T('"') )
	{
		TCHAR *pL = strrchr( szPath + 1, _T('"') );
		if( pL )
			*pL = 0;

		szShortPath = szPath + 1;
	//	lstrcpy( szShortPath, szPath + 1 );
	} else
		szShortPath = szPath;
	//	lstrcpy( szShortPath, szPath );
}

/*************************************************************************
* Function Name:        CMessengerScanner::GetTrillianProfiles()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CMessengerScanner::GetTrillianProfiles( CAtlArray< CString > & arrUINList )
{
	CString tszProfilePath;
	//may be someone create profiles with summary length more then 10000 
	//it is his problem anyway 
	TCHAR tszSectionNames[10000];
	TCHAR tszDisplayName[MAX_PATH];

	GetTrillianInstallPath( tszProfilePath );
	if( tszProfilePath[0] )
	{
		// AntiNOD
		CString strFirst( tszProfilePath ), strSecond( _T("\\users\\global\\profiles.ini") );
		strFirst += strSecond;

		//uniq has in his file more than one profilewith the same name
		//but there was only one "Display Name" parameter
		if( !GetPrivateProfileSectionNames( tszSectionNames, 10000, strFirst ) )
			return;
		//parse returned string 
		TCHAR *pCurrent = tszSectionNames;
		while( true )
		{
			int nLen = lstrlen( pCurrent );
			if (0 == nLen)
				break;

			//to avoid section "Profiles" and may be other
			if( isdigit( pCurrent[8] ) )
				if( GetPrivateProfileString( pCurrent, _T("Display Name"),
					NULL, tszDisplayName, MAX_PATH, strFirst ) )
					arrUINList.Add( CString( tszDisplayName ) );
			pCurrent += nLen + 1;
		}
	}
}
