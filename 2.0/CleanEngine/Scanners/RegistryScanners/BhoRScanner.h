#pragma once
#include "RegistryScanner.h"

class CBhoRScanner : public CRegistryScanner
{
protected:
    static const scanner_id m_nScannerId = BHO_SCANNER_ID;
    HKEY m_hScanningKey;
    DWORD m_dwIndex;

    virtual void postop( void );
    virtual void algorithm( void );
    void ( CBhoRScanner::*m_pfnAlgoSelector ) ( void );

    // algorithm implementation in essence
    void algoStep1( void );
    void algoStep2( void );

protected:
    virtual void ReinitAlgorithm( void );

public:
    explicit CBhoRScanner( CCleanEngine * pEngine, INotifier * pNotifier = NULL, IScanProfiler * pProfiler = NULL );

    virtual ~CBhoRScanner( void );

    virtual scanner_id ScannerId( void ) const { return m_nScannerId; }
};
