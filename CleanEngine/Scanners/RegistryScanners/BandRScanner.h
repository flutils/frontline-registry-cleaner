#pragma once
#include "RegistryScanner.h"

class CBandRScanner : public CRegistryScanner
{
protected:
    static const scanner_id m_nScannerId = BANDS_SCANNER_ID;
    HKEY m_hScanningKey;
    DWORD m_dwIndex;

    virtual void postop( void );
    virtual void algorithm( void );
    void ( CBandRScanner::*m_pfnAlgoSelector ) ( void );

    // algorithm implementation in essence
    void algoStep1( void );
    void algoStep2( void );
    void algoStep3( void );

protected:
    virtual void ReinitAlgorithm( void );

public:
    explicit CBandRScanner( CCleanEngine * pEngine, INotifier * pNotifier = NULL, IScanProfiler * pProfiler = NULL );

    virtual ~CBandRScanner( void );
    virtual scanner_id ScannerId( void ) const { return m_nScannerId; }
};
