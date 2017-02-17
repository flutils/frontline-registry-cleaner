#pragma once
#include "FileScanner.h"
#include "IETools.h"
#include <Wininet.h>

class CTempIEFScanner : public CFileScanner, protected CIETools
{
protected:
    static const scanner_id m_nScannerId = TEMPIE_SCANNER_ID;
    
    WIN32_FIND_DATA     m_sFD;
    HANDLE              m_hTempIeSearch;
    ce_tstring             m_strTempIeDir;
    
    virtual void postop( void );
    virtual void algorithm( void );
    void ( CTempIEFScanner::*m_pfnAlgoSelector ) ( void );

    // algorithm implementation in essence
    void algoStep1( void );

	HANDLE SafeFindFirstUrlCacheEntryEx(	LPCTSTR lpszUrlSearchPattern,
											DWORD dwFlags, DWORD dwFilter, GROUPID GroupId,
											LPINTERNET_CACHE_ENTRY_INFO lpFirstCacheEntryInfo,
											LPDWORD lpdwFirstCacheEntryInfoBufferSize,
											LPVOID lpReserved, LPDWORD pcbReserved2,
											LPVOID lpReserved3 );

	BOOL SafeFindNextUrlCacheEntry(	HANDLE hEnumHandle,
									LPINTERNET_CACHE_ENTRY_INFO lpNextCacheEntryInfo,
									LPDWORD lpdwNextCacheEntryInfoBufferSize );

protected:
    virtual void ReinitAlgorithm( void );

public:
    explicit CTempIEFScanner( CCleanEngine * pEngine, INotifier * pNotifier = NULL, IScanProfiler * pProfiler = NULL );

    virtual ~CTempIEFScanner( void );

    virtual scanner_id ScannerId( void ) const { return m_nScannerId; }
	virtual void CleanupObject( countable< IReportItem > item );
};
