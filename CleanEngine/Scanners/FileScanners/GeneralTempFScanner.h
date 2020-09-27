#pragma once
#include "FileScanner.h"

class CGeneralTempFScanner : public CFileScanner
{
protected:
    static const scanner_id m_nScannerId = GENERALTEMP_SCANNER_ID;

    std::vector< countable < ce_tstring > >             m_vTempList;
    std::vector< countable < ce_tstring > >::iterator   m_theIterator;
    
    virtual void postop( void );
    virtual void algorithm( void );
    void ( CGeneralTempFScanner::*m_pfnAlgoSelector ) ( void );

    // algorithm implementation in essence
    void algoStep1( void );
    void algoStep2( void );

protected:
    virtual void ReinitAlgorithm( void );
    bool IsAdded( const TCHAR * szTempPath );

public:
    explicit CGeneralTempFScanner( CCleanEngine * pEngine, INotifier * pNotifier = NULL, IScanProfiler * pProfiler = NULL );

    virtual ~CGeneralTempFScanner( void );
    virtual scanner_id ScannerId( void ) const { return m_nScannerId; }
    bool AddTempPath( const TCHAR * szTempPath );
};
