#pragma once
#include "RegistryScanner.h"

class CSharedDllRScanner : public CRegistryScanner
{
protected:
    static const scanner_id m_nScannerId = SHAREDDLL_SCANNER_ID;
    DWORD m_dwIndex;
    HKEY m_hScanningKey;
    
    virtual void postop( void );
    virtual void algorithm( void );
    void ( CSharedDllRScanner::*m_pfnAlgoSelector ) ( void );

    // algorithm implementation in essence
    void algoStep1( void );
    void algoStep2( void );

protected:
    virtual void ReinitAlgorithm( void );

public:
    explicit CSharedDllRScanner( CCleanEngine * pEngine, INotifier * pNotifier = NULL, IScanProfiler * pProfiler = NULL );

    virtual ~CSharedDllRScanner( void );
    virtual scanner_id ScannerId( void ) const { return m_nScannerId; }
};
