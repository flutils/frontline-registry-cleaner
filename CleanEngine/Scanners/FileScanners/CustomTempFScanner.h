#pragma once
#include "FileScanner.h"

class CCustomTempFScanner : public CFileScanner
{
protected:
    static const scanner_id m_nScannerId = CUSTOMTEMP_SCANNER_ID;

    virtual void algorithm( void );
    void ( CCustomTempFScanner::*m_pfnAlgoSelector ) ( void );

    // algorithm implementation in essence
    void algoStep1( void );
    void algoStep2( void );
    void algoStep3( void );

protected:
    virtual void ReinitAlgorithm( void );

public:
    explicit CCustomTempFScanner( CCleanEngine * pEngine, INotifier * pNotifier = NULL, IScanProfiler * pProfiler = NULL );

    virtual ~CCustomTempFScanner( void );

    virtual scanner_id ScannerId( void ) const { return m_nScannerId; }
};
