#pragma once
#include "RegistryScanner.h"

class CMruRScanner : public CRegistryScanner
{
protected:
    static const scanner_id m_nScannerId = MRU_SCANNER_ID;
    ce_tstring m_strScanObject;

    DWORD   m_dwIndex;
    HKEY    m_hScanningKey;

    std::vector< countable < MruKey > >             m_vMruList;
    std::vector< countable < MruKey > >::iterator   m_theIterator;

    virtual void postop( void );
    virtual void algorithm( void );
    void ( CMruRScanner::*m_pfnAlgoSelector ) ( void );

    // algorithm implementation in essence
    void algoStep1( void );
    void algoStep2( void );

	virtual const std::vector< countable < MruKey > > * GetMRUPathList() { return &m_vMruList; }

protected:
    virtual void ReinitAlgorithm( void );

public:
    explicit CMruRScanner( CCleanEngine * pEngine, INotifier * pNotifier = NULL, IScanProfiler * pProfiler = NULL );

    virtual ~CMruRScanner( void );
    virtual scanner_id ScannerId( void ) const { return m_nScannerId; }
    bool AddMruKey( const HKEY hKey, const TCHAR * szMruKeyName );
};
