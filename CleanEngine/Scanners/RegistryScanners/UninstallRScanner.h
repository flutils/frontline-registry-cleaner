#pragma once
#include "RegistryScanner.h"
#include "ReportChecker.h"

class CUninstallRScanner : public CRegistryScanner, public CReportChecker
{
protected:
    static const scanner_id m_nScannerId = UNINSTALL_SCANNER_ID;
    DWORD m_dwIndex;
    HKEY m_hScanningKey;
    
    virtual void postop( void );
    virtual void algorithm( void );
    void ( CUninstallRScanner::*m_pfnAlgoSelector ) ( void );

    // algorithm implementation in essence
    void algoStep1( void );
    void algoStep2( void );
    void algoStep3( void );
    void algoStep4( void );
    void algoStep5( void );

protected:
    virtual void ReinitAlgorithm( void );

public:
    explicit CUninstallRScanner( CCleanEngine * pEngine, INotifier * pNotifier = NULL, IScanProfiler * pProfiler = NULL );

    virtual ~CUninstallRScanner( void );
    virtual scanner_id ScannerId( void ) const { return m_nScannerId; }
};
