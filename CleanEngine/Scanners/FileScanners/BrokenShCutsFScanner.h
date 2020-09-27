#pragma once
#include "FileScanner.h"

class CBrokenShCutsFScanner : public CFileScanner
{
protected:
    static const scanner_id m_nScannerId = BROKENSHCUTS_SCANNER_ID;

    virtual void algorithm( void );
    void ( CBrokenShCutsFScanner::*m_pfnAlgoSelector ) ( void );

    // algorithm implementation in essence
    void algoStep1( void );
    void algoStep2( void );
    void algoStep3( void );

protected:
    virtual void ReinitAlgorithm( void );

public:
    explicit CBrokenShCutsFScanner( CCleanEngine * pEngine, INotifier * pNotifier = NULL, IScanProfiler * pProfiler = NULL );

    virtual ~CBrokenShCutsFScanner( void );

    virtual scanner_id ScannerId( void ) const { return m_nScannerId; }

	virtual void OnScanConditionDone(LPCTSTR strCurrentDir, LPCTSTR strFileName);
};
