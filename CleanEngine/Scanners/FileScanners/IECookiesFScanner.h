#pragma once
#include "FileScanner.h"

class CIECookiesFScanner : public CFileScanner
{
protected:
    static const scanner_id m_nScannerId = IECOOKIES_SCANNER_ID;

    WIN32_FIND_DATA     m_sFD;
    HANDLE              m_hCookieSearch;
    ce_tstring             m_strCookieDir;

    virtual void postop( void );
    virtual void algorithm( void );
    void ( CIECookiesFScanner::*m_pfnAlgoSelector ) ( void );

    // algorithm implementation in essence
    void algoStep1( void );
    void algoStep2( void );
    void algoStep3( void );

protected:
    virtual void ReinitAlgorithm( void );

public:
    explicit CIECookiesFScanner( CCleanEngine * pEngine, INotifier * pNotifier = NULL, IScanProfiler * pProfiler = NULL );

    virtual ~CIECookiesFScanner( void );
    virtual scanner_id ScannerId( void ) const { return m_nScannerId; }
};
