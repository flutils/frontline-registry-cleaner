#pragma once
#include "RegistryScanner.h"
#include "ReportChecker.h"

class CComProgRScanner : public CRegistryScanner, public CReportChecker
{
protected:
    static const scanner_id m_nScannerId = COMPROG_SCANNER_ID;
    DWORD m_dwIndex;

    virtual void algorithm( void );
    void ( CComProgRScanner::*m_pfnAlgoSelector ) ( void );

    // algorithm implementation in essence
    void algoStep1( void );
    void CheckCLSID( const TCHAR * szKeyName );
	

protected:
    virtual void ReinitAlgorithm( void );

public:
    explicit CComProgRScanner( CCleanEngine * pEngine, INotifier * pNotifier = NULL, IScanProfiler * pProfiler = NULL );

    virtual ~CComProgRScanner( void );

    virtual scanner_id ScannerId( void ) const { return m_nScannerId; }
};
