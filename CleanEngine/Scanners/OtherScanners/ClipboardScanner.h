#pragma once
#include "FileScanner.h"

class CClipboardScanner : public CCleanScanner
{
protected:
    static const scanner_id m_nScannerId = CLIPBOARD_SCANNER_ID;

    virtual void algorithm( void );
    void ( CClipboardScanner::*m_pfnAlgoSelector )( void );

    // algorithm implementation in essence
    void algoStep1( void );

protected:
    virtual void ReinitAlgorithm( void );

public:
    explicit CClipboardScanner( CCleanEngine * pEngine, INotifier * pNotifier = NULL, IScanProfiler * pProfiler = NULL );

    virtual ~CClipboardScanner( void );
    virtual scanner_id ScannerId( void ) const { return m_nScannerId; }

    virtual void CleanupObject( countable< IReportItem > item );
};
