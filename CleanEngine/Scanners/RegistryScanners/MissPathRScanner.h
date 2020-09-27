#pragma once
#include "RegistryScanner.h"

class CMissPathRScanner : public CRegistryScanner
{
protected:
    static const scanner_id m_nScannerId = MISSPATH_SCANNER_ID;
    DWORD m_dwIndex;
    HKEY m_hScanningKey;
    
    virtual void postop( void );
    virtual void algorithm( void );
    void ( CMissPathRScanner::*m_pfnAlgoSelector ) ( void );

    // algorithm implementation in essence
    void algoStep1( void );
    void algoStep2( void );
    void algoStep3( void );

    void CheckMissedPaths(
        HKEY                hKey,
        ce_tstring&            rstrKeyName,
        int					nameSize,
        unsigned            nRecurseLevel = 0 );

protected:
    virtual void ReinitAlgorithm( void );

public:
    explicit CMissPathRScanner( CCleanEngine * pEngine, INotifier * pNotifier = NULL, IScanProfiler * pProfiler = NULL );

    virtual ~CMissPathRScanner( void );
    virtual scanner_id ScannerId( void ) const { return m_nScannerId; }
};
