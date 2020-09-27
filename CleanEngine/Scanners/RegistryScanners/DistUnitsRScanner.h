#pragma once
#include "RegistryScanner.h"

class CDistUnitsRScanner : public CRegistryScanner
{
protected:
    static const scanner_id m_nScannerId = DISTUNITS_SCANNER_ID;
    DWORD m_dwIndex;
    HKEY m_hScanningKey;
    
    void postop( void );
    virtual void algorithm( void );
    void ( CDistUnitsRScanner::*m_pfnAlgoSelector ) ( void );

    // algorithm implementation in essence
    void algoStep1( void );
    void algoStep2( void );

protected:
    virtual void ReinitAlgorithm( void );

public:
    explicit CDistUnitsRScanner( CCleanEngine * pEngine, INotifier * pNotifier = NULL, IScanProfiler * pProfiler = NULL );

    virtual ~CDistUnitsRScanner( void );

    virtual scanner_id ScannerId( void ) const { return m_nScannerId; }
};
