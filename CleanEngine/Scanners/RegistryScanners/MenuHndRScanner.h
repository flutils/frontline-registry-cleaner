#pragma once
#include "RegistryScanner.h"

class CMenuHndRScanner : public CRegistryScanner
{
protected:
    static const scanner_id m_nScannerId = MENUHND_SCANNER_ID;
    DWORD m_dwIndex;

    virtual void algorithm( void );
    void ( CMenuHndRScanner::*m_pfnAlgoSelector )( void );

    // algorithm implementation in essence
    void algoStep1( void );
    void CheckMenuHandler( const TCHAR * szKeyName );
    
    void ScanKeyForGuids(
        const HKEY          hKey,
        ce_tstring &           rstrKeyName,
        int					length,
        unsigned            nRecurseLevel = 0 );

protected:
    virtual void ReinitAlgorithm( void );

public:
    explicit CMenuHndRScanner( CCleanEngine * pEngine, INotifier * pNotifier = NULL, IScanProfiler * pProfiler = NULL );

    virtual ~CMenuHndRScanner( void );
    virtual scanner_id ScannerId( void ) const { return m_nScannerId; }
};
