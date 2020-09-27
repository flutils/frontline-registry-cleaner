#pragma once
#include "RegistryScanner.h"

class CAppPathRScanner : public CRegistryScanner
{
protected:
    static const scanner_id m_nScannerId = APPPATH_SCANNER_ID;
    DWORD m_dwIndex;
    HKEY m_hScanningKey;
    
    virtual void postop( void );
    virtual void algorithm( void );
    void ( CAppPathRScanner::*m_pfnAlgoSelector ) ( void );

    // algorithm implementation in essence
    void algoStep1( void );
    void algoStep2( void );
    void CheckAppPath( const TCHAR * szKeyName );

protected:
    virtual void ReinitAlgorithm( void );

public:
    explicit CAppPathRScanner( CCleanEngine * pEngine, INotifier * pNotifier = NULL, IScanProfiler * pProfiler = NULL );

    virtual ~CAppPathRScanner( void );
    virtual scanner_id ScannerId( void ) const { return m_nScannerId; }
};
