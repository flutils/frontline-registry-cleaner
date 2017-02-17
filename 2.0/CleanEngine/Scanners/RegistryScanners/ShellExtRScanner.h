#pragma once
#include "RegistryScanner.h"

class CShellExtRScanner : public CRegistryScanner
{
protected:
    static const scanner_id m_nScannerId = SHELLEXT_SCANNER_ID;
    DWORD m_dwIndex;
    HKEY m_hScanningKey;
    
    virtual void postop( void );
    virtual void algorithm( void );
    void ( CShellExtRScanner::*m_pfnAlgoSelector ) ( void );

    // algorithm implementation in essence
    void algoStep1( void );
    void algoStep2( void );

protected:
    virtual void ReinitAlgorithm( void );

public:
    explicit CShellExtRScanner( CCleanEngine * pEngine, INotifier * pNotifier = NULL, IScanProfiler * pProfiler = NULL );

    virtual ~CShellExtRScanner( void );
    virtual scanner_id ScannerId( void ) const { return m_nScannerId; }
};
