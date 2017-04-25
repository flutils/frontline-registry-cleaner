#pragma once

#include "CleanScanner.h"
#include "FileTools.h"
#include <AtlPath.h>

// Cease warnings for 
// warning C4996: 'GetVersionExA'
#pragma warning( disable: 4996 )

class CFileScanner : virtual public CCleanScanner, virtual public CFileTools
{

public:

	explicit CFileScanner( CCleanEngine * pEngine, INotifier * pNotifier = NULL, IScanProfiler * pProfiler = NULL );

	virtual void CleanupObject( countable< IReportItem > item );

protected:

    void RecursivelyAddDirectory(
        ce_tstring&            rstrDirName,
        const bool          bAddDirs = true,
        const unsigned      nDepth = 0 );

	void GetFilesFromDirectory( CPath , CAtlArray<CString> & );
	void CheckDirs( CAtlArray<CString> & arrDirs );
	void RemoveDublicates( CAtlArray<CString> & arrDirs );

	void RecursivelyScanByMask(	std::string & strCurrentDir,
								const std::list<std::string> & lstExtMask,
								const unsigned nDepth = 0 );

	HRESULT _SecureDeleteFile( LPCSTR lpFileName, UINT uiRetries );
	HRESULT SecureDeleteFile( LPCSTR lpFileName, UINT uiRetries );
	HRESULT SecureDeleteFolder(LPCTSTR lpFolder, UINT uiRetries);
	void SecureDeleteFolderItems(LPCTSTR lpFolder, UINT uiRetries);
	virtual void OnScanConditionDone(LPCTSTR strCurrentDir, LPCTSTR strFileName);

};
