#pragma once
#include "FileScanner.h"

class CTempWUpdateFScanner : public CFileScanner
{
protected:
    static const scanner_id m_nScannerId = TEMPWUPDATE_SCANNER_ID;
    WIN32_FIND_DATA     m_sFD;
    HANDLE              m_hUpdateSearch;
    ce_tstring             m_strUpdateDir;
    
    virtual void postop( void );
    virtual void algorithm( void );
    void ( CTempWUpdateFScanner::*m_pfnAlgoSelector ) ( void );

    // algorithm implementation in essence
    void algoStep1( void );
    void algoStep2( void );    

protected:
    virtual void ReinitAlgorithm( void );

public:
    explicit CTempWUpdateFScanner( CCleanEngine * pEngine, INotifier * pNotifier = NULL, IScanProfiler * pProfiler = NULL );

    virtual ~CTempWUpdateFScanner( void );
    virtual scanner_id ScannerId( void ) const { return m_nScannerId; }
};
