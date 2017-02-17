#pragma once
#include "FileScanner.h"

class CRecycleBinFScanner : public CCleanScanner
{
protected:
    static const scanner_id m_nScannerId = RECYCLEBIN_SCANNER_ID;

    virtual void algorithm( void );
    void ( CRecycleBinFScanner::*m_pfnAlgoSelector )( void );

    // algorithm implementation in essence
    void algoStep1( void );

protected:
    virtual void ReinitAlgorithm( void );

public:
    explicit CRecycleBinFScanner( CCleanEngine * pEngine, INotifier * pNotifier = NULL, IScanProfiler * pProfiler = NULL );

    virtual ~CRecycleBinFScanner( void );
    virtual scanner_id ScannerId( void ) const { return m_nScannerId; }

    virtual void CleanupObject( countable< IReportItem > item );
};
