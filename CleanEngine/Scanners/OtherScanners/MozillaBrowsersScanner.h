#pragma once
#include "FileScanner.h"
#include "CookieAPI.h"

using namespace CookieAPI;

class CMozillaBrowsersScanner : public CFileScanner
{
protected:
    static const scanner_id m_nScannerId = MOZILLA_BROWSERS_SCANNER_ID;

    virtual void algorithm( void );
    void ( CMozillaBrowsersScanner::*m_pfnAlgoSelector )( void );

    // algorithm implementation in essence
    void algoStep1( void );
    void algoStep2( void );

	// Mozilla\Firefox\Netscape
	bool GetMozillaHistoryDirs( FF_TYPE ffType, CAtlArray<CString> & arrDirs, CString strProfile );

	bool GetMozillaDirs(  CAtlArray<CString> & arrDirs, CString strProfile );
	bool GetNetscapeDirs( CAtlArray<CString> & arrDirs, CString strProfile );
	bool GetFirefoxDirs( CAtlArray<CString> & arrDirs, CString strProfile );

	bool GetMozillaCachePath( FF_TYPE ffType, CAtlArray<CString> &arrPathes, CString strProfile );
	bool GetTypedURLsFilePath( CAtlArray<CString> &arrPathes, CString strProfile = _T("") );
	bool GetPasswordsFilesPath( FF_TYPE ffType, CAtlArray<CString> &arrPathes, CString strProfile = _T("") );
	void FindNeededFiles( CPath pathTmp, CAtlArray<CString> &arrPathes );
	void GetCookiesSet( FF_TYPE ffType, std::set<CString> & setDeleteCookies, CString strProfile = _T("") ) const;

private:
	BOOL m_bFlushed;
	CFFCookiesFile * m_MozillasCookieFilesArray[3];

protected:
	virtual void prerun( void );
    virtual void ReinitAlgorithm( void );

public:
    explicit CMozillaBrowsersScanner( CCleanEngine * pEngine, INotifier * pNotifier = NULL, IScanProfiler * pProfiler = NULL );

    virtual ~CMozillaBrowsersScanner( void );
    virtual scanner_id ScannerId( void ) const { return m_nScannerId; }

    virtual void CleanupObject( countable< IReportItem > item );

	virtual void FlushCookiesFile();
};
