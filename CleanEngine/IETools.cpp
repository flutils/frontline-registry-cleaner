#include "StdAfx.h"
#include "IETools.h"
#include <AtlPath.h>
#include "shlobj.h"

const TCHAR c_szIndexDat[] = _T("index.dat");

void CIETools::SubEnumIndexDat( std::set<CString> & arrIndexDat, LPTSTR szRoot )
{
	lstrcat( szRoot, _T("\\History.IE5\\") );
	//get root dat file
	CString sz( szRoot );
	sz += c_szIndexDat;
	//if somebody create there directory it is his fault
	if( INVALID_FILE_ATTRIBUTES != GetFileAttributes( sz ) )
		arrIndexDat.insert( sz );
	//enum subfolder
	WIN32_FIND_DATA wfd;
	sz = szRoot;
	sz += _T("MSHist*");
	HANDLE hFind = FindFirstFile( sz, &wfd );
	if( INVALID_HANDLE_VALUE != hFind )
	{
		do {
			sz = szRoot;
			sz += wfd.cFileName;
			sz.AppendChar( _T('\\') );
			sz += c_szIndexDat;
			if( INVALID_FILE_ATTRIBUTES != GetFileAttributes( sz ) )
				arrIndexDat.insert( sz );
		}
		while( FindNextFile( hFind, &wfd ) );
		FindClose( hFind );
	}
}

void CIETools::GetAllIEHistoryIndexDat( std::set<CString> & arrIndexDat )
{
	TCHAR szRoot[MAX_PATH];
	if( SHGetSpecialFolderPath( 0, szRoot, CSIDL_HISTORY, FALSE ) )
		SubEnumIndexDat( arrIndexDat, szRoot );
	if( GetTempPath( MAX_PATH, szRoot ) )
		SubEnumIndexDat( arrIndexDat, szRoot );
}
