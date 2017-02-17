#pragma once
#include "FileScanner.h"

class CPrefetchStuffFScanner : public CFileScanner
{
protected:
    static const scanner_id m_nScannerId = PRFTCHSTUFF_SCANNER_ID;

    virtual void algorithm( void );
    void ( CPrefetchStuffFScanner::*m_pfnAlgoSelector ) ( void );

    // algorithm implementation in essence
    void algoStep1( void );

protected:
    virtual void ReinitAlgorithm( void );

public:
    explicit CPrefetchStuffFScanner( CCleanEngine * pEngine, INotifier * pNotifier = NULL, IScanProfiler * pProfiler = NULL );

    virtual ~CPrefetchStuffFScanner( void );
    virtual scanner_id ScannerId( void ) const { return m_nScannerId; }
};
