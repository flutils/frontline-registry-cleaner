#pragma once

#include <Windows.h>
#include <atlstr.h>
#include <atlfile.h>
#include <map>

class CIndexDatFile
{
public:
	CIndexDatFile(void);
	~CIndexDatFile(void);

	HRESULT Load(LPCTSTR lpFileName);

	HRESULT DeleteUrl(LPCTSTR lpUrl, bool bFlush = false);

	HRESULT CleanHistory(void);

	void RemoveEmptyRootItems( LPCTSTR lpFileName );

private:
	HRESULT DeleteRecord(DWORD dwOffset, DWORD dwTag);

private:
	// url -> record offset
	typedef std::map<CString, DWORD> map_type;
	map_type _m_mapUrls;
	CAtlFile _m_file;
};
