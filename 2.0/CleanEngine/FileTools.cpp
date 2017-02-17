#include "StdAfx.h"
#include "FileTools.h"
#include <AtlPath.h>

HRESULT CFileTools::DeleteFolder( LPCTSTR lpFolder ) const
{
	SHFILEOPSTRUCT op = { 0 };
	op.wFunc = FO_DELETE;
	CString strFolder(lpFolder);
	strFolder.AppendChar(_T('\0'));
	op.pFrom = strFolder;
	op.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT;

	int iRes = ::SHFileOperation(&op);
	HRESULT hr = AtlHresultFromWin32(iRes);
	return hr;
}

HRESULT CFileTools::IsPathExist( LPCTSTR lpFolder ) const
{
	return( INVALID_FILE_ATTRIBUTES == GetFileAttributes( lpFolder ) ? S_FALSE : S_OK );
};

void CFileTools::GetDrivesArray( DWORD dwDrives, std::list<std::string> & lstDrives ) const
{
	lstDrives.clear();

	for ( TCHAR ch = _T('A'); ch <= _T('Z'); ++ch, dwDrives >>= 1 )
	{
		if ( dwDrives & 1 )
		{
			CPath path;
			path.m_strPath.Format( _T("%c:"), ch );

			if ( DRIVE_FIXED == ::GetDriveType( path ) )
				lstDrives.push_back( path.m_strPath.GetString() );
		}
	}
}
