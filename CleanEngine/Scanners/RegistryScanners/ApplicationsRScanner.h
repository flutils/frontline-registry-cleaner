#pragma once
#include "RegistryScanner.h"

class CApplicationsRScanner : public CRegistryScanner
{
protected:
    static const scanner_id m_nScannerId = APPLICATIONS_SCANNER_ID;
    HKEY m_hScanningKey;
    DWORD m_dwIndex;

    virtual void postop( void );
    virtual void algorithm( void );
    void ( CApplicationsRScanner::*m_pfnAlgoSelector ) ( void );

    // algorithm implementation in essence
    void algoStep1( void );
    void algoStep2( void );

    void CheckApplication( const TCHAR * szKeyName );

    CHECKPATH_RESULT CheckKeyForCommand( 
        HKEY hKey, 
        const TCHAR * szKeyName,
        unsigned nDepth = 0 );

protected:
    virtual void ReinitAlgorithm( void );

public:
    explicit CApplicationsRScanner( CCleanEngine * pEngine, INotifier * pNotifier = NULL, IScanProfiler * pProfiler = NULL );

    virtual ~CApplicationsRScanner( void );

    virtual scanner_id ScannerId( void ) const { return m_nScannerId; }
};
