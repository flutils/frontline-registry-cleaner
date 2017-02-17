// Profile.cpp: implementation of the CProfile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Profile.h"
#include <shlwapi.h>
#include "AtlFileFind.h"

//#include <winnt.h>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUserProfile::CUserProfile(const CString& Name, const bool& UseRegistry,
	const CRegistryRootKey& RegistryRootKey)
{
	ATLASSERT(!Name.IsEmpty());

	m_sName = Name;
	m_bUseRegistry = UseRegistry; 
	m_eRegistryRootKey = RegistryRootKey;

	m_sLastSection = "";
	m_hKey = NULL;

	//Registry key names cannot begin with '\' but should end with '\'
	//because of the way we concatenate the section name.
	if (m_bUseRegistry)
	{
		int NameLen = m_sName.GetLength();
		if ((NameLen > 0) && (m_sName[0] == _T('\\')))
			m_sName = m_sName.Right(NameLen-1);
		NameLen = m_sName.GetLength();
		if ((NameLen > 0) && (m_sName[NameLen-1] != _T('\\')))
			m_sName += _T("\\");
	}
}

CUserProfile::~CUserProfile()
{
	CloseRegistryHKey();
}

//////////////////////////////////////////////////////////////////////
// Protected Member Functions
//////////////////////////////////////////////////////////////////////

HKEY CUserProfile::OpenRegistryHKey(const CString& Section)
{
	if ((m_hKey == NULL) || (Section.CompareNoCase(m_sLastSection) != 0))
	{
		CloseRegistryHKey();
		//Update m_sLastSection
		m_sLastSection = Section;
		//Update m_hKey;
		DWORD dw;
		HKEY hRootKey = m_eRegistryRootKey == hkeyCurrentUser ? HKEY_CURRENT_USER : HKEY_LOCAL_MACHINE;
		LONG lResult = RegCreateKeyEx(hRootKey, LPCTSTR(m_sName+Section), 0, REG_NONE,
			REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL, &m_hKey, &dw);

		if (lResult != ERROR_SUCCESS) CloseRegistryHKey();
	}
	return m_hKey;
}

bool CUserProfile::CloseRegistryHKey()
{
	LONG lResult = ERROR_SUCCESS;
	if (m_hKey != NULL)	lResult = RegCloseKey(m_hKey);
	m_hKey = NULL;
	m_sLastSection = "";
	return lResult == ERROR_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
// Public Member Functions
//////////////////////////////////////////////////////////////////////

CString CUserProfile::GetString(const CString& Section, const CString& Ident, const CString& Default)
{
	m_sLastSection = Section;
	if (m_bUseRegistry)
	{
		OpenRegistryHKey(Section);
		if (m_hKey == NULL) return Default;

		CString strValue;
		DWORD dwType, dwCount;
		LONG lResult = RegQueryValueEx(m_hKey, LPCTSTR(Ident), NULL, &dwType, NULL, &dwCount);
		if (lResult == ERROR_SUCCESS)
		{
			ATLASSERT(dwType == REG_SZ);
			lResult = RegQueryValueEx(m_hKey, LPCTSTR(Ident), NULL, &dwType,
				(LPBYTE)strValue.GetBuffer(dwCount/sizeof(TCHAR)), &dwCount);
			strValue.ReleaseBuffer();
		}
		if (lResult == ERROR_SUCCESS)
		{
			ATLASSERT(dwType == REG_SZ);
			return strValue;
		}
		return Default;
	}
	else
	{
		TCHAR Buffer[4096];
		DWORD dw = GetPrivateProfileString(LPCTSTR(Section), LPCTSTR(Ident), LPCTSTR(Default), Buffer, sizeof(Buffer), LPCTSTR(m_sName));
		ATLASSERT(dw < 4095);
		return CString(Buffer);
	}
	
}

bool CUserProfile::WriteString(const CString& Section, const CString& Ident, const CString& Value)
{
	if (m_bUseRegistry)
	{
		LONG lResult;
		OpenRegistryHKey(Section);
		if (m_hKey == NULL) return false;
		lResult = RegSetValueEx( m_hKey, LPCTSTR( Ident ), NULL, REG_SZ,
			(LPBYTE) LPCTSTR( Value ), static_cast<DWORD>( ( _tcslen( LPCTSTR( Value ) ) + 1 ) * sizeof(TCHAR ) ) );
		return lResult == ERROR_SUCCESS;
	}
	else
		return (WritePrivateProfileString(LPCTSTR(Section), LPCTSTR(Ident), LPCTSTR(Value), LPCTSTR(m_sName)) != FALSE);
}

int CUserProfile::GetInt(const CString& Section, const CString& Ident, const int& Default)
{
	if (m_bUseRegistry)
	{
		OpenRegistryHKey(Section);
		if (m_hKey == NULL) return Default;

		DWORD dwValue;
		DWORD dwType;
		DWORD dwCount = sizeof(DWORD);
		LONG lResult = RegQueryValueEx(m_hKey, (LPCTSTR)Ident, NULL, &dwType,
			(LPBYTE)&dwValue, &dwCount);
		if (lResult == ERROR_SUCCESS)
		{
			ATLASSERT(dwType == REG_DWORD);
			ATLASSERT(dwCount == sizeof(dwValue));
			return (UINT)dwValue;
		}
		return Default;
	}
	else
		return GetPrivateProfileInt(LPCTSTR(Section), LPCTSTR(Ident), Default, LPCTSTR(m_sName));
}

bool CUserProfile::WriteInt(const CString& Section, const CString& Ident, const int& Value)
{
	if (m_bUseRegistry)
	{
		OpenRegistryHKey(Section);
		if (m_hKey == NULL) return false;
		LONG lResult = RegSetValueEx(m_hKey, LPCTSTR(Ident), NULL, REG_DWORD,
			(LPBYTE)&Value, sizeof(Value));
		return lResult == ERROR_SUCCESS;
	}
	else
	{
		CString Buffer;
		Buffer.Format(_T("%d"), Value);
		return WriteString(Section, Ident, Buffer);	
	}
}

bool CUserProfile::GetBool(const CString& Section, const CString& Ident, const bool& Default)
{
	return GetInt(Section, Ident, Default ? 1 : 0) != FALSE;	
}

bool CUserProfile::WriteBool(const CString& Section, const CString& Ident, const bool& Value)
{
	return WriteInt(Section, Ident, Value ? 1 : 0);
}

double CUserProfile::GetDouble(const CString& Section, const CString& Ident, const double& Default)
{
	LPBYTE pData;
	UINT Bytes;
	if (GetBinary(Section, Ident, &pData, &Bytes))
	{
		double Result;
		memcpy(&Result, pData, Bytes);
		delete[] pData;
		return Result;
	}
	else
		return Default;
}

bool CUserProfile::WriteDouble(const CString& Section, const CString& Ident, const double& Value)
{
	return WriteBinary(Section, Ident, (LPBYTE)&Value, sizeof(Value));
}

bool CUserProfile::GetBinary(const CString& Section, const CString& Ident, BYTE** ppData, UINT* pBytes)
{
	ATLASSERT(ppData != NULL);
	ATLASSERT(pBytes != NULL);
	*ppData = NULL;
	*pBytes = 0;

	if (m_bUseRegistry)
	{
		OpenRegistryHKey(Section);
		if (m_hKey == NULL) return false;

		DWORD dwType, dwCount;
		LONG lResult = RegQueryValueEx(m_hKey, (LPCTSTR)Ident, NULL, &dwType, NULL, &dwCount);
		if ((lResult == ERROR_SUCCESS) && (dwType == REG_BINARY))
		{
			*pBytes = dwCount;
			*ppData = new BYTE[*pBytes];
			lResult = RegQueryValueEx(m_hKey, (LPCTSTR)Ident, NULL, &dwType, *ppData, &dwCount);
			if (lResult != ERROR_SUCCESS)
			{
				delete[] *ppData;
				*pBytes = 0;
				*ppData = NULL;
			}
			return lResult == ERROR_SUCCESS;
		}
		else
			return false;
	}
	else
	{
		CString Buffer = GetString(Section, Ident, "");
		if (Buffer.IsEmpty()) return false;

		int nLen = Buffer.GetLength();
		ATLASSERT(nLen%2 == 0);
		if (nLen%2 == 0)
		{
			*pBytes = nLen >> 1;
			*ppData = new BYTE[*pBytes];
			for (int i=0; i<nLen; i+=2)
				(*ppData)[i>>1] = (BYTE)(((Buffer[i+1] - 'A') << 4) + (Buffer[i] - 'A'));
			return true;
		}
		else
			return false;
	}
}

bool CUserProfile::WriteBinary(const CString& Section, const CString& Ident, LPBYTE pData, UINT nBytes)
{
	if (m_bUseRegistry)
	{
		OpenRegistryHKey(Section);
		if (m_hKey == NULL)	return false;
		return RegSetValueEx(m_hKey, LPCTSTR(Ident), NULL, REG_BINARY, 
			pData, nBytes) == ERROR_SUCCESS;
	}
	else
	{
		//Convert data to CString and write it out
		CString Buffer;
		LPTSTR lpsz = Buffer.GetBufferSetLength((nBytes<<1)+1);
		UINT i;
		for (i=0; i<nBytes; i++)
		{
			lpsz[i<<1] = (TCHAR)((pData[i] & 0x0F) + 'A'); //low nibble
			lpsz[(i<<1)+1] = (TCHAR)(((pData[i] >> 4) & 0x0F) + 'A'); //high nibble
		}
		lpsz[i<<1] = _T('\0');
		Buffer.ReleaseBuffer();
		return WriteString(Section, Ident, Buffer);
	}
}

bool CUserProfile::GetSectionItems(const CString& Section, CAtlArray<CString>* Items)
{
	Items->RemoveAll();

	if (m_bUseRegistry)
	{
		OpenRegistryHKey(Section);
		if (m_hKey == NULL) return false;

		//Get num values and max value name length.
		DWORD dwNumValues, dwMaxValueLen;
		if (RegQueryInfoKey(m_hKey, NULL, NULL, NULL, NULL, NULL, NULL,
			&dwNumValues, &dwMaxValueLen, NULL, NULL, NULL) != ERROR_SUCCESS)
			return false;
		dwMaxValueLen++; //Add space for NULL terminator.

		char* Buffer = new char[dwMaxValueLen];
		DWORD BufSize = dwMaxValueLen;
		LONG lResult;
		for(DWORD i=0; i<dwNumValues; i++)
		{
			lResult = RegEnumValue(m_hKey, i, Buffer, &BufSize, NULL, NULL, NULL, NULL);
			ATLASSERT(lResult == ERROR_SUCCESS);
			if (lResult != ERROR_SUCCESS) return false;
			Items->Add(Buffer);
			BufSize = dwMaxValueLen; //RegEnumValue modifies BufSize
		}
		delete[] Buffer;
		return true;
	}
	else
	{
		TCHAR Buffer[8192];
		char* P;
		//Passing NULL for Section gets all the section names in the file.
		DWORD dw = GetPrivateProfileString(Section.IsEmpty() ? NULL : LPCTSTR(Section), 
			NULL, NULL, Buffer, sizeof(Buffer), LPCTSTR(m_sName));
		if (dw != 0)
		{
			P = Buffer;
			while (*P != NULL)
			{
				Items->Add(P);
				P+=(_tcslen(P) + sizeof(TCHAR));
			}
		}
		return dw != 0;
	}
}

bool CUserProfile::GetSections(CAtlArray<CString>* const Sections)
{
	if (m_bUseRegistry)
	{
		Sections->RemoveAll();

		OpenRegistryHKey();
		if (m_hKey == NULL) return false;

		//Get num keys and max key name length.
		DWORD dwNumKeys, dwMaxKeyLen;
		if (RegQueryInfoKey(m_hKey, NULL, NULL, NULL, &dwNumKeys, &dwMaxKeyLen, 
			NULL, NULL, NULL, NULL, NULL, NULL) != ERROR_SUCCESS)
			return false;
		dwMaxKeyLen++; //Add space for NULL terminator.

		char* Buffer = new char[dwMaxKeyLen];
		DWORD BufSize = dwMaxKeyLen;
		LONG lResult;
		for(DWORD i=0; i<dwNumKeys; i++)
		{
			lResult = RegEnumKey(m_hKey, i, Buffer, BufSize);
			ATLASSERT(lResult == ERROR_SUCCESS);
			if (lResult != ERROR_SUCCESS) return false;
			Sections->Add(Buffer);
			BufSize = dwMaxKeyLen; //RegEnumKey modifies BufSize
		}
		delete[] Buffer;
		return true;
	}
	else
		return GetSectionItems(_T(""), Sections);
}

bool CUserProfile::DeleteSection(const CString& Section)
{
	if (m_bUseRegistry)
	{
		OpenRegistryHKey();
		if (m_hKey == NULL) return false;
		LONG lResult = RegDeleteKey(m_hKey, LPCTSTR(Section));
		return lResult == ERROR_SUCCESS;
	}
	else
		return (WritePrivateProfileString(LPCTSTR(Section), NULL, NULL, LPCTSTR(m_sName)) != FALSE);
}

bool CUserProfile::DeleteItem(const CString& Section, const CString& Ident)
{
	if (m_bUseRegistry)
	{
		OpenRegistryHKey(Section);
		if (m_hKey == NULL) return false;
		LONG lResult = RegDeleteValue(m_hKey, LPCTSTR(Ident));
		return lResult == ERROR_SUCCESS;
	}
	else
		return (WritePrivateProfileString(LPCTSTR(Section), LPCTSTR(Ident), NULL, LPCTSTR(m_sName)) != FALSE);
}

CString CUserProfile::GetName()
{
	return m_sName;	
}

bool CUserProfile::GetUseRegistry()
{
	return m_bUseRegistry;
}

CUserProfile::CRegistryRootKey CUserProfile::GetRegistryRootKey()
{
	return m_eRegistryRootKey;
}

bool CUserProfile::IsRegistryValueSpecifyed(const CString& szSection, const CString& /*szIdent*/)
{
	if ((m_hKey == NULL) || (szSection.CompareNoCase(m_sLastSection) != 0))
	{
		CloseRegistryHKey();
		//Update m_sLastSection
		m_sLastSection = szSection;
		//Update m_hKey;		
		HKEY hRootKey = m_eRegistryRootKey == hkeyCurrentUser ? HKEY_CURRENT_USER : HKEY_LOCAL_MACHINE;
		LONG lResult = RegOpenKeyEx(hRootKey, LPCTSTR(m_sName+szSection), 0, KEY_READ, &m_hKey);

		if (lResult != ERROR_SUCCESS) {			
			CloseRegistryHKey();
			return false;
		}
	}
	CloseRegistryHKey();
	return true;
}

bool CUserProfile::IsAppIdRegistered(const CString& /*szInstalledGUID*/)
{	
	HKEY hRootKey = HKEY_CLASSES_ROOT;
	HKEY hKey;
	LONG lResult = RegOpenKeyEx(hRootKey, "AppID", 0, KEY_READ, &hKey);

	if (lResult != ERROR_SUCCESS) 
	{			
		RegCloseKey(hKey);
		return false;
	}

	RegCloseKey(hKey);
	return true;
}

CString CUserProfile::GetExecutableModuleDir()
{
	HMODULE hm = GetModuleHandle(NULL);
	char sz[MAX_PATH];
	GetModuleFileName(hm, sz, MAX_PATH);
	CString szRetVal = sz;

	return GetExecutableDir(szRetVal);
}

CString CUserProfile::GetExecutableModulePathName()
{
	HMODULE hm = GetModuleHandle(NULL);
	char sz[MAX_PATH];
	GetModuleFileName(hm, sz, MAX_PATH);
	CString szRetVal = sz;

	return szRetVal;
}

CString CUserProfile::GetExecutableDir(const CString& szExecPath)
{
	LPCTSTR lpsz = (LPCTSTR)szExecPath;
	size_t sLen = strlen(lpsz);
	int i = static_cast<int>( sLen );

	while (lpsz[i] != '\\')
	{
		i--;
	}

	return szExecPath.Mid(0, i);
}

CString CUserProfile::CorrectSlesh(CString str)
{
	int NameLen = str.GetLength();
	if ((NameLen > 0) && (str[NameLen-1] != _T('\\')))
		str += _T("\\");
	return str;
}

BOOL CUserProfile::DeleteFileEx(const CString& szFilePath)
{
	DWORD dwAttr = ::GetFileAttributes(szFilePath);
	/*
	CFileStatus status;
	CFile::GetStatus( szFilePath, status );
	*/
	BOOL bDel = ::DeleteFile((LPCTSTR)szFilePath);

	if (!bDel)
	{
		try
		{
			/*
			BYTE newAttribute = 0x00;;
			status.m_attribute = newAttribute;
			CFile::SetStatus( szFilePath, status );
			*/
			::SetFileAttributes(szFilePath, dwAttr);
			bDel = ::DeleteFile((LPCTSTR)szFilePath);	
	}
		catch (...)
		{
			bDel = false;
			//MessageBox(NULL, "exception occured while deleting a file!! " + szFilePath, "dsds", MB_OK);
		}
		}
	return bDel;
}

BOOL CUserProfile::IsFolderEmpty(const CString& szFolderPath)
{
	CAtlFileFind fileFnd;
	
	BOOL bEmpty = true;
	CString szPath = szFolderPath;
	szPath = CUserProfile::CorrectSlesh(szFolderPath);
	if (S_OK == fileFnd.FindFirst(szPath + _T("*.*")))
	{
		BOOL bWork = true;
		while (bWork)
		{
			bWork = (S_OK == fileFnd.FindNext());
			if (fileFnd.IsDots())
			{
				continue;
			}
			else
			{
				bEmpty = false;
				break;
			}
		}
	}

	return bEmpty;
}
BOOL CUserProfile::DeleteRegistryKeyValue(const CString& szRegValue, CUserProfile::CRegistryRootKey regKey)
{
	HKEY hkey = (regKey == hkeyCurrentUser ? HKEY_CURRENT_USER : HKEY_LOCAL_MACHINE);
	
	LPCTSTR lpsz = static_cast<LPCTSTR>(szRegValue);

	CString szSection;
	CString szValue;
	int i = 0;
	
	for (i = szRegValue.GetLength(); i > 0; i--)
	{
		if (lpsz[i] == '\\')
			break;
	}
	szSection = szRegValue.Mid(0, i);
	int n = szSection.GetLength() + 1;
	szValue = szRegValue.Mid(n, szRegValue.GetLength() - 1);
	
	HKEY hVal;
	LONG retVal = RegOpenKey(hkey, static_cast<LPCTSTR>(szSection), &hVal);
	if (retVal != ERROR_SUCCESS)
		return false;
	retVal = RegDeleteValue(hVal, static_cast<LPCTSTR>(szValue));
	RegCloseKey(hVal);
	return (retVal == ERROR_SUCCESS);
}

BOOL CUserProfile::DeleteRegistryKey(const CString& szRegValue, CUserProfile::CRegistryRootKey regKey)
{
	HKEY hkey = (regKey == hkeyCurrentUser ? HKEY_CURRENT_USER : HKEY_LOCAL_MACHINE);
	LONG rslt;
    rslt = SHDeleteKey(hkey, szRegValue);
	return (rslt == ERROR_SUCCESS);
	
}


//---------------------------------------------------------------------------

struct
{
	LPCTSTR m_szParamName;
} const _XMLParams[] =	{
							{_T("<Name>")},
							{_T("<Id>")},
							{_T("<IsInstallHDD>")},
							{_T("<IsInstallReg>")},
							{_T("<DelRegKey>")}, 
							{_T("<DelRegValue>")},
							{_T("<DelDir>")},
							{_T("<DelFile>")},
							{_T("<AppDir>")}
						};

int gParamsCount = sizeof(_XMLParams)/sizeof(_XMLParams[0]);

void CUserXMLParser::InitFromFile( const CString& XMLPath )
{
	HANDLE hFile = CreateFile( XMLPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if( hFile == INVALID_HANDLE_VALUE )
	{
		ATLASSERT( FALSE );
		return;
	}

	DWORD dwSize, dwSize2;
	dwSize = GetFileSize( hFile, &dwSize2 );
	CAtlString strBuffer;

	TCHAR buf[256];
	do
	{
		BOOL bRet = ReadFile( hFile, buf, 256, &dwSize2, NULL );
		if( !bRet && !dwSize2 )
		{
			ATLASSERT( FALSE );
			return;
		}

		strBuffer.Append( buf, dwSize2 );
	} while( 256 == dwSize2 );

	strBuffer.Append( buf, dwSize2 );

	CloseHandle( hFile );

	InitFromBuffer( strBuffer );
};

void CUserXMLParser::InitFromBuffer( const CString& strData )
{
	int pos = 0, posEnd = 0, posFileBegin = 0;
	XMLfilePart file;
	strPair tmpPair;
	CString strTmp;
	CString strValName;

	pos = strData.Find( _T("<files>"), pos );
	if( -1 == pos )
		return;

	do
	{
		pos = strData.Find( _T("<file>"), pos );
		if( -1 == pos )
			break;

		posFileBegin = pos;

		//---------

		posEnd = strData.Find( _T("</file>"), pos );
		if( -1 == posEnd )
			break;

		//---------

		for ( int i = 0; i < gParamsCount ; ++i )
		{
			pos = posFileBegin;
			strTmp.Format( _T("%s"), _XMLParams[i] );
			while ( pos != -1 )
			{
				pos = strData.Find( strTmp, pos );
				if ( pos < posEnd && pos != -1 )
				{
					strValName = strTmp;
					if( strValName.GetLength() > 2 )
					{
						strValName.Delete( 0 );
						strValName.Delete( strValName.GetLength() - 1 );
					}
					file.insert( std::make_pair( strValName, GetValue( pos, strData ) ) );
				}
				else
					break;
				pos += lstrlen( strTmp );
			}
		}

		m_arrXMLFiles.Add( file );

		pos = posEnd;
		posFileBegin = pos;

	} while( pos != -1 );
};

CString CUserXMLParser::GetValue( int &pos, const CString &str )
{
	CString strRet;
	int tmpPos1 = 0, tmpPos2 = 0;

	//-----

	tmpPos1 = str.Find( _T('"'), ++pos );
	tmpPos2 = str.Find( _T('"'), ++tmpPos1 );
	strRet = str.Mid( tmpPos1, tmpPos2 - tmpPos1 );

	pos = tmpPos2;
	return strRet;
};
