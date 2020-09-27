#pragma once
#include "RegistryScanner.h"

class CHelpRScanner : public CRegistryScanner
{
protected:
    static const scanner_id m_nScannerId = HELP_SCANNER_ID;
    HKEY m_hScanningKey;

    virtual void algorithm( void );
    void ( CHelpRScanner::*m_pfnAlgoSelector )( void );

    // algorithm implementation in essence
    void algoStep1( void );
    void CheckHelp( HKEY hKey, const TCHAR * szKeyName );

protected:
    virtual void ReinitAlgorithm( void );

public:
    explicit CHelpRScanner( CCleanEngine * pEngine, INotifier * pNotifier = NULL, IScanProfiler * pProfiler = NULL );

    virtual ~CHelpRScanner( void );
    virtual scanner_id ScannerId( void ) const { return m_nScannerId; }
};
