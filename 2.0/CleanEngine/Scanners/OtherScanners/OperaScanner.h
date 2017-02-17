#pragma once
#include "FileScanner.h"
#include "CookieAPI.h"

using namespace CookieAPI;

class COperaScanner : public CFileScanner
{
protected:
    static const scanner_id m_nScannerId = OPERA_SCANNER_ID;

    virtual void algorithm( void );
    void ( COperaScanner::*m_pfnAlgoSelector )( void );

    // algorithm implementation in essence
    void algoStep1( void );
    void algoStep2( void );
    void algoStep3( void );
    void algoStep4( void );
    void algoStep5( void );
    void algoStep6( void );

	bool GetOperaDir(  CAtlArray<CString> & arrDir );
	void GetOperaDirByRegKey( CString strKeyValue, CAtlArray<CString> & arrDir );
	bool GetOperaHistoryPath( CAtlArray<CString> &arrPathes );
	bool GetOperaVLinksPath( CAtlArray<CString> &arrPathes );
	bool GetDownloadInfoPath( CAtlArray<CString> &arrPathes );
	bool GetCacheFilesPathes( CAtlArray<CString> &arrPathes );
	bool GetMRULIstsPath( CAtlArray<CString> &arrPathes );
	bool GetOperaCookiesPath( CAtlArray<CString> &arrPathes );

private:
	BOOL m_bFlushed;
	CAtlArray<COperaDatFile> m_OperaCookieFilesArray;

protected:
	virtual void prerun( void );
    virtual void ReinitAlgorithm( void );

public:
    explicit COperaScanner( CCleanEngine * pEngine, INotifier * pNotifier = NULL, IScanProfiler * pProfiler = NULL );

    virtual ~COperaScanner( void );
    virtual scanner_id ScannerId( void ) const { return m_nScannerId; }

    virtual void CleanupObject( countable< IReportItem > item );

	virtual void FlushCookiesFile();
};
