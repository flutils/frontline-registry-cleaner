#ifndef INIFILE_H
#define INIFILE_H

#pragma once

#include <windows.h>
#include <crtdbg.h>
#include <strsafe.h>
#include <atlstr.h>

class CIniFile
{
public:
	CIniFile(LPCTSTR lpFileName)
	{
		_ASSERTE(NULL != lpFileName);
		HRESULT hr = ::StringCbCopy(_m_szFileName, sizeof(_m_szFileName), lpFileName);
		_ASSERTE(SUCCEEDED(hr));
	}

	INT ReadInt(LPCTSTR lpSection, LPCTSTR lpKeyName, INT nDefault = 0)
	{
		_ASSERTE(NULL != lpSection);
		_ASSERTE(NULL != lpKeyName);
		return ::GetPrivateProfileInt(lpSection, lpKeyName, nDefault, _m_szFileName);
	}

	HRESULT WriteInt(LPCTSTR lpSection, LPCTSTR lpKeyName, INT nValue)
	{
		TCHAR lpString[24];
		HRESULT hr = ::StringCbPrintf(lpString, sizeof(lpString),
			_T("%i"), static_cast<int>(nValue));
		_ASSERTE(SUCCEEDED(hr));
		return WriteString(lpSection, lpKeyName, lpString);
	}

	UINT ReadUInt(LPCTSTR lpSection, LPCTSTR lpKeyName, UINT nDefault = 0)
	{
		_ASSERTE(NULL != lpSection);
		_ASSERTE(NULL != lpKeyName);
		return ::GetPrivateProfileInt(lpSection, lpKeyName, nDefault, _m_szFileName);
	}

	HRESULT WriteUInt(LPCTSTR lpSection, LPCTSTR lpKeyName, UINT nValue)
	{
		TCHAR lpString[24];
		HRESULT hr = ::StringCbPrintf(lpString, sizeof(lpString),
			_T("%u"), static_cast<unsigned int>(nValue));
		_ASSERTE(SUCCEEDED(hr));
		return WriteString(lpSection, lpKeyName, lpString);
	}

	CString ReadString(LPCTSTR lpSection, LPCTSTR lpKeyName, LPCTSTR lpDefault = _T(""))
	{
		_ASSERTE(NULL != lpSection);
		_ASSERTE(NULL != lpKeyName);
		_ASSERTE(NULL != lpDefault);
		CString strResult;
		DWORD dwSize = 128;
		DWORD dwLen = 0;
		do {
			dwSize *= 2;
			dwLen = ::GetPrivateProfileString(lpSection, lpKeyName, lpDefault,
				strResult.GetBuffer(dwSize - 1), dwSize, _m_szFileName);
		} while (!(dwLen < (dwSize - 1 * sizeof(TCHAR))));
		strResult.ReleaseBuffer(dwLen);
		return strResult;
	}

	HRESULT WriteString(LPCTSTR lpSection, LPCTSTR lpKeyName, LPCTSTR lpString)
	{
		_ASSERTE(NULL != lpSection);
		_ASSERTE(NULL != lpKeyName);
		_ASSERTE(NULL != lpString);
		return ::WritePrivateProfileString(lpSection, lpKeyName, lpString,
			_m_szFileName) ? S_OK : HRESULT_FROM_WIN32(::GetLastError());
	}

	BOOL ReadData(LPCTSTR lpSection, LPCTSTR lpKeyName, LPVOID lpData, UINT nSize)
	{
		_ASSERTE(NULL != lpSection);
		_ASSERTE(NULL != lpKeyName);
		_ASSERTE(NULL != lpData);
		return ::GetPrivateProfileStruct(lpSection, lpKeyName, lpData, nSize, _m_szFileName);
	}

	HRESULT WriteData(LPCTSTR lpSection, LPCTSTR lpKeyName, LPCVOID lpData, UINT nSize)
	{
		_ASSERTE(NULL != lpSection);
		_ASSERTE(NULL != lpKeyName);
		_ASSERTE(NULL != lpData);
		return ::WritePrivateProfileStruct(lpSection, lpKeyName,
			const_cast<LPVOID>(lpData), nSize, _m_szFileName) ?
			S_OK : HRESULT_FROM_WIN32(::GetLastError());
	}

	CString ReadSectionNames(void)
	{
		CString strResult;
		DWORD dwSize = 256;
		DWORD dwLen = 0;
		do {
			dwSize *= 2;
			dwLen = ::GetPrivateProfileSectionNames(
				strResult.GetBuffer(dwSize - 1), dwSize, _m_szFileName);
		} while (!(dwLen < (dwSize - 2 * sizeof(TCHAR))));
		strResult.ReleaseBuffer(dwLen);
		return strResult;
	}

	CString ReadSectionKeyNames(LPCTSTR lpSection)
	{
		_ASSERTE(NULL != lpSection);
		CString strResult;
		DWORD dwSize = 256;
		DWORD dwLen = 0;
		do {
			dwSize *= 2;
			dwLen = ::GetPrivateProfileString(lpSection, NULL, _T(""),
				strResult.GetBuffer(dwSize - 1), dwSize, _m_szFileName);
		} while (!(dwLen < (dwSize - 2 * sizeof(TCHAR))));
		strResult.ReleaseBuffer(dwLen);
		return strResult;
	}

	HRESULT DeleteKey(LPCTSTR lpSection, LPCTSTR lpKeyName)
	{
		_ASSERTE(NULL != lpSection);
		_ASSERTE(NULL != lpKeyName);
		return ::WritePrivateProfileString(lpSection, lpKeyName, NULL,
			_m_szFileName) ? S_OK : HRESULT_FROM_WIN32(::GetLastError());
	}

	HRESULT DeleteSection(LPCTSTR lpSection)
	{
		_ASSERTE(NULL != lpSection);
		return ::WritePrivateProfileString(lpSection, NULL, NULL,
			_m_szFileName) ? S_OK : HRESULT_FROM_WIN32(::GetLastError());
	}

	CString ReadSection(LPCTSTR lpSection)
	{
		_ASSERTE(NULL != lpSection);
		CString strResult;
		DWORD dwSize = 2048;
		DWORD dwLen = 0;
		do {
			dwSize *= 2;
			dwLen = ::GetPrivateProfileSection(lpSection,
				strResult.GetBuffer(dwSize - 1), dwSize, _m_szFileName);
		} while (!(dwLen < (dwSize - 2 * sizeof(TCHAR))));
		strResult.ReleaseBuffer(dwLen);
		return strResult;
	}

	HRESULT WriteSection(LPCTSTR lpSection, LPCTSTR lpData)
	{
		_ASSERTE(NULL != lpSection);
		return ::WritePrivateProfileSection(lpSection, lpData,
			_m_szFileName) ? S_OK : HRESULT_FROM_WIN32(::GetLastError());
	}

private:
	TCHAR _m_szFileName[MAX_PATH];
};

#endif
