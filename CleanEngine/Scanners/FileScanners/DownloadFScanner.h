#pragma once
#include "FileScanner.h"

class CDownloadFScanner : public CFileScanner
{
protected:
	static const scanner_id m_nScannerId = DOWNLOAD_SCANNER_ID;
	WIN32_FIND_DATA     m_sFD;
	HANDLE              m_hDownloadSearch;
	ce_tstring             m_strDownloadDir;

	virtual void postop( void );
	virtual void algorithm( void );
	void ( CDownloadFScanner::*m_pfnAlgoSelector ) ( void );

	// algorithm implementation in essence
	void algoStep1( void );
	void algoStep2( void );
	void algoStep3( void );
	void algoStep4( void );

	bool AddDownloadFile();

protected:
	virtual void ReinitAlgorithm( void );

public:
	explicit CDownloadFScanner( CCleanEngine * pEngine, INotifier * pNotifier = NULL, IScanProfiler * pProfiler = NULL );

	virtual ~CDownloadFScanner( void );
	virtual scanner_id ScannerId( void ) const { return m_nScannerId; }
};
