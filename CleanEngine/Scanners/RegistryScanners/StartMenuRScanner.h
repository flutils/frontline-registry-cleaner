#pragma once
#include "RegistryScanner.h"

class CStartMenuRScanner : public CRegistryScanner
{
protected:
    static const scanner_id m_nScannerId = STARTMENU_SCANNER_ID;
    ce_tstring     m_strAllUserFolder;
    ce_tstring     m_strCurrentUserFolder;
    ce_tstring     m_strStartMenu;
    DWORD       m_dwIndex;
    HKEY        m_hScanningKey;
    
    virtual void postop( void );
    virtual void algorithm( void );
    void ( CStartMenuRScanner::*m_pfnAlgoSelector ) ( void );

    // algorithm implementation in essence
    void algoStep1( void );
    void algoStep2( void );    

protected:
    virtual void ReinitAlgorithm( void );

public:
    explicit CStartMenuRScanner( CCleanEngine * pEngine, INotifier * pNotifier = NULL, IScanProfiler * pProfiler = NULL );

    virtual ~CStartMenuRScanner( void );
    virtual scanner_id ScannerId( void ) const { return m_nScannerId; }
};
