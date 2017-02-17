#pragma once
#include "RegistryScanner.h"
#include "ReportChecker.h"

class CTypeLibRScanner : public CRegistryScanner, public CReportChecker
{
protected:
    static const scanner_id m_nScannerId = COMTYPELIB_SCANNER_ID;
    DWORD   m_dwIndex;
    HKEY    m_hScanningKey;
    
    virtual void postop( void );
    virtual void algorithm( void );
    void ( CTypeLibRScanner::*m_pfnAlgoSelector )( void );

    // algorithm implementation in essence
    void algoStep1( void );
    void algoStep2( void );
    void CheckTypeLib( HKEY hKey, const TCHAR * szKeyName );

    CHECKPATH_RESULT CheckKeyForPath(
        HKEY        hKey, 
        TCHAR *     szKeyName, 
        unsigned    nDepth = 0 );

protected:
    virtual void ReinitAlgorithm( void );

public:
    explicit CTypeLibRScanner( CCleanEngine * pEngine, INotifier * pNotifier = NULL, IScanProfiler * pProfiler = NULL );

    virtual ~CTypeLibRScanner( void );
    virtual scanner_id ScannerId( void ) const { return m_nScannerId; }
};
