#pragma once
#include "FileScanner.h"

class CIEHistoryFScanner : public CCleanScanner
{
protected:
    static const scanner_id m_nScannerId = IEHISTORY_SCANNER_ID;
    HRESULT m_hrInitialize;

    virtual void algorithm( void );
    void ( CIEHistoryFScanner::*m_pfnAlgoSelector )( void );

    // algorithm implementation in essence
    void algoStep1( void );

	void ClearHistory( void );

protected:
    virtual void ReinitAlgorithm( void );

public:
    explicit CIEHistoryFScanner( CCleanEngine * pEngine, INotifier * pNotifier = NULL, IScanProfiler * pProfiler = NULL );

    virtual ~CIEHistoryFScanner( void );
    virtual scanner_id ScannerId( void ) const { return m_nScannerId; }

    virtual void CleanupObject( countable< IReportItem > item );
};
