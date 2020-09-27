#ifndef ATLFILEFIND_H
#define ATLFILEFIND_H

#pragma once

//#include <Windows.h>
#include <atldef.h>
#include <atlcomcli.h>

class CAtlFileFind
{
public:
	CAtlFileFind(void) { Init(); }

	~CAtlFileFind(void) { Close(); }

	HRESULT FindFirst(LPCTSTR lpFileName)
	{
		ATLASSERT(NULL != lpFileName);
		HRESULT hr = Close();
		if (SUCCEEDED(hr)) {
			_m_hFind = ::FindFirstFile(lpFileName, &_m_FindData);
			hr = (INVALID_HANDLE_VALUE == _m_hFind) ? AtlHresultFromLastError() : S_OK;
		}
		return hr;
	}

	HRESULT FindNext(void)
	{
		ATLASSERT(IsValid());
		HRESULT hr = S_OK;
		if (!::FindNextFile(_m_hFind, &_m_FindData)) {
			DWORD dwErr = ::GetLastError();
			hr = (ERROR_NO_MORE_FILES == dwErr) ? S_FALSE : AtlHresultFromWin32(dwErr);
		}
		return hr;
	}

	HRESULT Close(void)
	{
		HRESULT hr = S_OK;
		if (INVALID_HANDLE_VALUE != _m_hFind) {
			if (::FindClose(_m_hFind)) Init();
			else hr = AtlHresultFromLastError();
		}
		return hr;
	}

	bool IsValid(void) const { return (INVALID_HANDLE_VALUE != _m_hFind); }

	WIN32_FIND_DATA const & GetInfo(void) const
	{
		ATLASSERT(IsValid());
		return _m_FindData;
	}

	bool IsDots(void) const
	{
		ATLASSERT(IsValid());
		return ((_T('.') == _m_FindData.cFileName[0]) &&
			((_T('\0') == _m_FindData.cFileName[1]) ||
			((_T('.') == _m_FindData.cFileName[1]) &&
			(_T('\0') == _m_FindData.cFileName[2]))));
	}

	bool IsDirectory(void) const
	{
		ATLASSERT(IsValid());
		return (0 != (FILE_ATTRIBUTE_DIRECTORY & _m_FindData.dwFileAttributes));
	}

	LPCTSTR GetFileName(void) const
	{
		ATLASSERT(IsValid());
		return _m_FindData.cFileName;
	}

private:
	void Init(void)
	{
		_m_hFind = INVALID_HANDLE_VALUE;
		::ZeroMemory(&_m_FindData, sizeof(_m_FindData));
	}

private:
	HANDLE _m_hFind;
	WIN32_FIND_DATA _m_FindData;
};

#endif
