#pragma once
#include "FileScanner.h"

class CRecentDocFScanner : public CFileScanner
{
protected:
    static const scanner_id m_nScannerId = RECENTDOC_SCANNER_ID;

    WIN32_FIND_DATA     m_sFD;
    HANDLE              m_hRecentSearch;
    ce_tstring             m_strRecentDir;
    
    virtual void postop( void );
    virtual void algorithm( void );
    void ( CRecentDocFScanner::*m_pfnAlgoSelector )( void );

    // algorithm implementation in essence
    void algoStep1( void );
    void algoStep2( void );

protected:
    virtual void ReinitAlgorithm( void );

public:
    explicit CRecentDocFScanner( CCleanEngine * pEngine, INotifier * pNotifier = NULL, IScanProfiler * pProfiler = NULL );

    virtual ~CRecentDocFScanner( void );
    virtual scanner_id ScannerId( void ) const { return m_nScannerId; }
};
