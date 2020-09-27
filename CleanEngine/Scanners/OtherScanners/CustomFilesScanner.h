#pragma once
#include "FileScanner.h"

class CCustomFilesScanner : public CFileScanner
{
protected:
    static const scanner_id m_nScannerId = CUSTOMFILES_SCANNER_ID;

    virtual void algorithm( void );
    void ( CCustomFilesScanner::*m_pfnAlgoSelector )( void );

    // algorithm implementation in essence
    void algoStep1( void );

protected:
    virtual void ReinitAlgorithm( void );

public:
    explicit CCustomFilesScanner( CCleanEngine * pEngine, INotifier * pNotifier = NULL, IScanProfiler * pProfiler = NULL );

    virtual ~CCustomFilesScanner( void );
    virtual scanner_id ScannerId( void ) const { return m_nScannerId; }
};
