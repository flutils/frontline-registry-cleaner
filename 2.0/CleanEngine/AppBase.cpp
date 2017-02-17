#include "StdAfx.h"
#include "AppBase.h"
#include "profile.h"
#include <ShlObj.h>
#include <atlpath.h>

struct HiveAbbr 
{
	TCHAR	szAbbr[5];
	HKEY	hHive;
} g_HiveAbbr[] = {
	{ _T("HKCR"), HKEY_CLASSES_ROOT },
	{ _T("HKCU"), HKEY_CURRENT_USER },
	{ _T("HKLM"), HKEY_LOCAL_MACHINE }
};

BOOL CALLBACK XmlsEnumProc(HMODULE hModule, LPTSTR lpType, LPTSTR lpName, LONG_PTR lParam);

SAppBase::SAppBase(void)
{
}

SAppBase::~SAppBase(void)
{
}

void SAppBase::Init( LPCTSTR lpDbFolder, BASE_SOURCE uiLoadFrom, HINSTANCE hResInstance )
{
	m_strDbFolder = lpDbFolder;
	switch( uiLoadFrom )
	{
	case LOAD_FROM_DAT:
		ReadBaseFiles( LOAD_FROM_DAT );
	break;

	case LOAD_FROM_XML:
		ReadBaseFiles( LOAD_FROM_XML );
	break;

	case LOAD_FROM_RES:
		EnumResourceNames( hResInstance, RT_RCDATA, (ENUMRESNAMEPROC)(XmlsEnumProc), reinterpret_cast<LONG_PTR>( this ) );
	break;

	default:
		// Not implemented yet
		ATLASSERT(FALSE);
	}
}

void SAppBase::ReadBaseFiles( UINT uiFrom )
{
	CPath szFile;

	LOAD_FROM_DAT == uiFrom ?
		szFile.Combine(m_strDbFolder, _T("*.dat")):
		szFile.Combine(m_strDbFolder, _T("*.xml"));

	WIN32_FIND_DATA wfd;
	HANDLE hFind = FindFirstFile(szFile, &wfd);
	if (hFind != INVALID_HANDLE_VALUE) 
	{
		do 
			if( !(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
			{
				szFile.Combine(m_strDbFolder, wfd.cFileName);
				LOAD_FROM_DAT == uiFrom ?
					GetApplications( szFile ):
					ReadXml( szFile, FALSE );
			} 
		while( FindNextFile( hFind, &wfd ) );
		FindClose( hFind );
	}	
}

/*****************************************************************/
bool IsVariable(const CString& szData)
{
	if (szData.FindOneOf(_T("%")) == -1)
		return false;

	LPCTSTR lpsz = static_cast<LPCTSTR>(szData);
	
	size_t i = 0;

	bool bVar = false;
	CString szRawChars;

	bool bVarExists = false;

	while (lpsz[i] != '\0')
	{
		if (lpsz[i] == '%' && !bVar) // assume that this is the begini
		{
			bVar = true;
		}
		i++;
		if (lpsz[i] != '%' && bVar)
		{
			szRawChars.AppendChar(lpsz[i]);
		}
		else
		{
			bVar = false;
			szRawChars.Empty();
			if (!bVarExists)
				bVarExists = true;
			i++;
		}
	}

	return bVarExists;
}

CString GetVariablesData(const CString& szVarScriptData, CUserProfile * pprofile, XMLfilePart * pxmlProfile/* = NULL*/);

// gets the variables data
CString GetVariableData(const CString& szVarScript, CUserProfile * pprofile, XMLfilePart * pxmlProfile)
{
	CString szRetVal;
	wchar_t szPath[MAX_PATH] = {0};

	if (szVarScript.Find(_T("CSIDL_")) != -1) // csidl values
	{
		int nCsild = 0;

		if (szVarScript == _T("CSIDL_APPDATA"))
			nCsild = CSIDL_APPDATA;
		else if (szVarScript == _T("CSIDL_COMMOM_APPDATA"))
			nCsild = CSIDL_COMMON_APPDATA;
		else if (szVarScript == _T("CSIDL_LOCAL_APPDATA"))
			nCsild = CSIDL_COMMON_APPDATA;
		else if (szVarScript == _T("CSIDL_COMMON_PROGRAMS"))
			nCsild = CSIDL_COMMON_PROGRAMS;
		else if (szVarScript == _T("CSIDL_PROGRAMS"))
			nCsild = CSIDL_PROGRAMS;
		else if (szVarScript == _T("CSIDL_DESKTOPDIRECTORY"))
			nCsild = CSIDL_DESKTOPDIRECTORY;
		else if (szVarScript == _T("CSIDL_COMMON_DESKTOPDIRECTORY"))
			nCsild = CSIDL_COMMON_DESKTOPDIRECTORY;		
		else if (szVarScript == _T("CSIDL_WINDOWS"))
			nCsild = CSIDL_WINDOWS;
		else
			return szRetVal;

		SHGetSpecialFolderPathW(NULL, szPath, nCsild, FALSE);

		if (szPath[wcslen(szPath)] != '\\')
			szPath[wcslen(szPath)] = '\\';

		szRetVal = szPath;
	}
	else
	{
		CString szRet;

		if( pprofile )
		{
			szRet = pprofile->GetString(pprofile->GetLastSection(), szVarScript, _T(""));
		}

		if( pxmlProfile )
		{
			XMLfilePart_iterator it = pxmlProfile->find( szVarScript );
			if( it != pxmlProfile->end() )
				szRet = it->second;
		}

		if (szRet.GetLength() != 0)
		{
			//Sad makes someshit here: in WDC wasn't any way to parse
			//"AppDir=HKCU:Software\Radiate\Go!Zilla\fm path"
			//to get real folder so still nobody know how it should be interpreted
			//Farid would tell us later what to do
			if (IsVariable(szRet))
				szRetVal = GetVariablesData(szRet, pprofile, pxmlProfile);
			else
				szRetVal = szRet;
		}
	}
	return szRetVal;
}

// parses the value of a variable 
// determening the path.
// as an ex. variableA=%CSIDL_APPDATA%

CString GetVariablesData(const CString& szVarScriptData, CUserProfile * pprofile, XMLfilePart * pxmlProfile = NULL)
{
	CString szRetVal;
	LPCTSTR lpsz = static_cast<LPCTSTR>(szVarScriptData);
	size_t nPos  = 0;
	CString szRawChars;
	bool bVar = false;

	while (lpsz[nPos] != '\000')
	{
		if (lpsz[nPos] == '%' && !bVar) // assume that this is the begini
		{
			bVar = true;
			nPos++;
			continue;
		}
			
		if (lpsz[nPos] != '%' && bVar)
		{
			szRawChars.AppendChar(lpsz[nPos]);
		}
		else if (bVar)
		{
			bVar = false; // a new variable !
			szRetVal += GetVariableData(szRawChars, pprofile, pxmlProfile);
			szRawChars.Empty();
			nPos++;
		}
		else if (!bVar)
		{
			szRetVal.AppendChar(lpsz[nPos]);
			nPos++;
		}

		if (bVar)
			nPos++;
	}
	return szRetVal;
}

/*****************************************************************/

BOOL CALLBACK XmlsEnumProc( HMODULE hModule, LPTSTR /*lpType*/, LPTSTR lpName, LONG_PTR lParam )
{
	SAppBase* pAppScannerObj = reinterpret_cast<SAppBase*>( lParam );

	HRSRC hr = FindResource( hModule, lpName, RT_RCDATA );

	if( !hr )
		return TRUE;

	HGLOBAL hRes = LoadResource( hModule, hr );
	if( !hRes )
		return TRUE;

	LPTSTR pXmlBuf = (LPTSTR) LockResource( hRes );

	if( pXmlBuf )
	{
		pAppScannerObj->ReadXml( CString( pXmlBuf ), TRUE );

		UnlockResource( hRes );
	}

	return TRUE;
}

void SAppBase::ReadXml( CString &szParam, BOOL bFromRes )
{
	CUserXMLParser prof;

	if( bFromRes )
	{ // Load bases from resources
		prof.InitFromBuffer( szParam );
	}
	else
	{ // Load bases from dir//*.xml
		prof.InitFromFile( szParam );
	}

	size_t size = prof.GetFilesCount();

	for ( size_t s = 0; s < size; ++s )
	{

		SAction sAction;
		CStringW wszGUID;
		GUID guid = GUID_NULL;

		XMLfilePart_iterator it = prof.m_arrXMLFiles[s].begin();

		for( ; it != prof.m_arrXMLFiles[s].end(); ++it )
		{
			if( 0 == lstrcmpi( it->first, _T("IsInstallHDD") ) )
			{
				sAction.strIsInstallHDD = it->second;
			}
			else if( 0 == lstrcmpi( it->first, _T("IsInstallReg") ) )
			{
				sAction.strIsInstallReg = it->second;
			}
			else if( 0 == lstrcmpi( it->first, _T("Id") ) )
			{
				wszGUID = it->second;
				if( FAILED( CLSIDFromString( wszGUID.GetBuffer(), &guid ) ) )
					break;
			}
			else if( 0 == lstrcmpi( it->first, _T("Name") ) )
			{
				sAction.strName = it->second;
			}
			else if( 0 == lstrcmpi( it->first, _T("DelDir") ) )
			{
				if( it->second.GetLength() )
				{
					CString strLocation;
					if( IsVariable( it->second ) )
						strLocation = GetVariablesData( it->second, NULL, &(prof.m_arrXMLFiles[s]) );
					else
						strLocation = it->second;

					sAction.vThreatItems.push_back( std::make_pair( strLocation, IT_DIRECTORY ) );
				}
			}
			else if( 0 == lstrcmpi( it->first, _T("DelFile") ) )
			{
				if( it->second.GetLength() )
				{
					CString strLocation;
					if( IsVariable( it->second ) )
						strLocation = GetVariablesData( it->second, NULL, &(prof.m_arrXMLFiles[s]) );
					else
						strLocation = it->second;

					sAction.vThreatItems.push_back( std::make_pair( strLocation, IT_FILE ) );
				}
			}
			else if( 0 == lstrcmpi( it->first, _T("DelRegKey") ) )
			{
				if( it->second.GetLength() )
				{
					sAction.vThreatItems.push_back( std::make_pair( it->second, IT_REG_KEY ) );
				}
			}
			else if( 0 == lstrcmpi( it->first, _T("DelRegValue") ) )
			{
				if( it->second.GetLength() )
				{
					sAction.vThreatItems.push_back( std::make_pair( it->second, IT_REG_VALUE ) );
				}
			}

		} // for( ; it != prof.m_arrXMLFiles[s].end(); ++it )

		if( guid != GUID_NULL )
			m_mapAction.SetAt( guid, sAction );

	} // for (INT_PTR i = 0; i < SectionsArr.GetCount(); i++)
}

void SAppBase::GetApplications( CString &szFile )
{
	CUserProfile prof(szFile, false, CUserProfile::CRegistryRootKey::hkeyCurrentUser);
	CAtlArray<CString> SectionsArr;

	prof.GetSections(&SectionsArr);

	CString szSection;
	CString szSectItem;
	CAtlArray<CString> SectionItemsArr;

	INT_PTR iCount = static_cast<INT_PTR>( SectionsArr.GetCount() );

	for ( INT_PTR i = 0; i < iCount; ++i )
	{
		szSection = SectionsArr[i];

		if ( szSection.Find( _T("action") ) != -1 )
		{
			SAction sAction;
			CStringW wszGUID;
			sAction.strIsInstallHDD	= prof.GetString(szSection, _T("IsInstallHDD"), _T(""));
			if( sAction.strIsInstallHDD.GetLength()
				&& IsVariable( sAction.strIsInstallHDD ) )
				sAction.strIsInstallHDD = GetVariablesData( sAction.strIsInstallHDD, &prof );
			
			sAction.strIsInstallReg = prof.GetString(szSection, _T("IsInstallReg"), _T(""));
			//Nobody know for what fuck it is here
			//wszGUID = prof.GetString(szSection, _T("IsInstallGUID"), "");
			

			GUID guid = GUID_NULL;

			prof.GetSectionItems(szSection, &SectionItemsArr);
			size_t nCount = SectionItemsArr.GetCount();

			for ( size_t j = 0; j < nCount; ++j )
			{
				szSectItem = SectionItemsArr[j];
				if ( szSectItem.Find( _T("Id") ) != -1 )
				{
					wszGUID = prof.GetString(szSection, szSectItem, _T(""));
					if( FAILED( CLSIDFromString( wszGUID.GetBuffer(), &guid ) ) )
						break;					
				}
				else if ( szSectItem.Find(_T("Name")) != -1 )
				{
					sAction.strName = prof.GetString( szSection, szSectItem, _T("") );
				}
				else if ( szSectItem.Find(_T("DelDir")) != -1 )
				{
					CString strLocation = prof.GetString( szSection, szSectItem, _T("") );
					if( strLocation.GetLength() )
					{
						if( IsVariable( strLocation ) )
							strLocation = GetVariablesData( strLocation, &prof );

						sAction.vThreatItems.push_back( std::make_pair( strLocation, IT_DIRECTORY ) );
					}
				}				
				else if ( szSectItem.Find(_T("DelFile")) != -1 )
				{
					CString strLocation = prof.GetString( szSection, szSectItem, _T("") );
					if( strLocation.GetLength() )
					{
						if( IsVariable( strLocation ) )
							strLocation = GetVariablesData( strLocation, &prof );

						sAction.vThreatItems.push_back( std::make_pair( strLocation, IT_FILE ) );
					}
				}
				else if ( szSectItem.Find(_T("DelRegKey")) != -1 )
				{
					CString strLocation = prof.GetString(szSection, szSectItem, _T("")); 

					if( strLocation.GetLength() )
					{
						sAction.vThreatItems.push_back( std::make_pair( strLocation, IT_REG_KEY ) );
					}
				}
				else if ( szSectItem.Find(_T("DelRegValue")) != -1 )
				{
					CString strLocation = prof.GetString(szSection, szSectItem, _T("")); 

					if( strLocation.GetLength() )
					{
						sAction.vThreatItems.push_back( std::make_pair( strLocation, IT_REG_VALUE ) );
					}
				}
			}
			if( guid != GUID_NULL )
				m_mapAction.SetAt( guid, sAction );
		}
	}
}

void SAppBase::GetAppList( CAtlArray<SAppInfo> & arrAppInfoList ) const
{
	// farid's definitions make IntelliSense unworkable with dct-namespace :(
	POSITION pos = m_mapAction.GetStartPosition();
	BOOL b, bIsEmpty = TRUE;
	while( pos )
	{
		b = FALSE;
		SAction const & rAction = m_mapAction.GetValueAt( pos );

		if( rAction.strIsInstallReg.GetLength() )
		{
			bIsEmpty = FALSE;
			LPCTSTR lpKey;
			HKEY hKey, hHive;
			if( GetKeyFromSadsDataFormat( rAction.strIsInstallReg.GetString(), hHive, lpKey ) &&
				( ERROR_SUCCESS == RegOpenKey( hHive, lpKey, &hKey ) ) )
			{
				RegCloseKey( hKey );
				b = TRUE;
			}
		}
		if( !b && rAction.strIsInstallHDD.GetLength() )
		{
			bIsEmpty = FALSE;
			b = INVALID_FILE_ATTRIBUTES != GetFileAttributes( rAction.strIsInstallHDD.GetString() );
		}

		if( (!b && bIsEmpty) || b )
			arrAppInfoList.Add( SAppInfo( rAction.strName, m_mapAction.GetKeyAt( pos ) ) );

		m_mapAction.GetNext( pos );
	}
}

BOOL SAppBase::GetKeyFromSadsDataFormat( LPCTSTR lpSource, HKEY & hHive, LPCTSTR & lpKey ) const
{
	//Sad made it like
	//pRawProf = new CUserProfile(szclearRegPath, true, (pszReg[2] == 'C' ? CUserProfile::CRegistryRootKey::hkeyCurrentUser : CUserProfile::CRegistryRootKey::hkeyLocalMachine));
	//so there will be only litle additional check
	int n = sizeof( g_HiveAbbr ) / sizeof( g_HiveAbbr[0] ),
		nLen;
	TCHAR *pL = strchr( const_cast<TCHAR *>(lpSource), _T(':') );
	if( pL )
		nLen = static_cast<int>( pL - lpSource );
	else
		nLen = 4; //char capability of g_HiveAbbr[0].szAbbr

	while( n )
		if( !strncmp( lpSource, g_HiveAbbr[--n].szAbbr, nLen ) )
		{
			hHive = g_HiveAbbr[n].hHive;
			lpKey = lpSource + nLen + 1;
			return TRUE;
		}
	ATLASSERT( !"Incorrect of DelRegKey" );
	return FALSE;
}

BOOL SAppBase::GetRegValueFromSadsDataFormat( LPCTSTR lpSource, HKEY & hHive, CString & szKey, CString & szValue )
{
	int n = sizeof( g_HiveAbbr ) / sizeof( g_HiveAbbr[0] ),
		nLen;
	TCHAR *pL = strchr( const_cast<TCHAR *>(lpSource), _T(':') );
	if( pL )
		nLen = static_cast<int>( pL - lpSource );
	else
		nLen = 4; //char capability of g_HiveAbbr[0].szAbbr

	while( n )
		if( !strncmp( lpSource, g_HiveAbbr[--n].szAbbr, nLen ) )
		{
			//include ':' in the cut off part
			nLen++;
			hHive = g_HiveAbbr[n].hHive;
			TCHAR *pL = strrchr( const_cast<char *>(lpSource), _T(':') );
			if( pL >= lpSource + nLen )
			{
				szKey.SetString( lpSource + nLen, static_cast<int>( pL - lpSource - nLen ) );
				szValue = pL + 1;
				return TRUE;
			}
			break;
		}
	//ATLASSERT( !"Incorrect format of DelRegValue" );
	return FALSE;
}

SAppBase * SAppBase::Instance()
{
	static SAppBase baseObj;
	return &baseObj;
}
