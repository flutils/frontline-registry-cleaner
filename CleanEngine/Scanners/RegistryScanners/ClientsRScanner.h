#pragma once
#include "RegistryScanner.h"

class CClientsRScanner : public CRegistryScanner
{
protected:
    static const scanner_id m_nScannerId = CLIENTS_SCANNER_ID;
    HKEY m_hScanningKey;
    DWORD m_dwIndex;

    virtual void postop( void );

    virtual void algorithm( void );
    void ( CClientsRScanner::*m_pfnAlgoSelector )( void );

    // algorithm implementation in essence
    void algoStep1( void );
    void algoStep2( void );

    void CheckClientsGroup( HKEY hKey, const TCHAR * szKeyName );

    CHECKPATH_RESULT CClientsRScanner::CheckKeyForCommand( 
            HKEY            hKey, 
            const TCHAR *   szKeyName,
            unsigned        nDepth = 0 );

protected:
    virtual void ReinitAlgorithm( void );

public:
    explicit CClientsRScanner( CCleanEngine * pEngine, INotifier * pNotifier = NULL, IScanProfiler * pProfiler = NULL );

    virtual ~CClientsRScanner( void );
    virtual scanner_id ScannerId( void ) const { return m_nScannerId; }
};
