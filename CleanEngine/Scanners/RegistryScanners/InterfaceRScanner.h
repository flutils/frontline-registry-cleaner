#pragma once
#include "RegistryScanner.h"
#include "ReportChecker.h"


class CInterfaceRScanner : public CRegistryScanner, public CReportChecker
{
protected:
    static const scanner_id m_nScannerId = COMINTERFACE_SCANNER_ID;
    HKEY m_hScanningKey;
    DWORD m_dwIndex;

    virtual void postop( void );
    virtual void algorithm( void );
    void ( CInterfaceRScanner::*m_pfnAlgoSelector ) ( void );

    // algorithm implementation in essence
    void algoStep1( void );
    void algoStep2( void );
    void CheckInterface( const TCHAR * szKeyName );

	BOOL CheckPotentialThreat( HKEY hKey );

protected:
    virtual void ReinitAlgorithm( void );

public:
    explicit CInterfaceRScanner( CCleanEngine * pEngine, INotifier * pNotifier = NULL, IScanProfiler * pProfiler = NULL );

    virtual ~CInterfaceRScanner( void );
    virtual scanner_id ScannerId( void ) const { return m_nScannerId; }
};
