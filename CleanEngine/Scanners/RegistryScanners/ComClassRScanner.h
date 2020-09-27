#pragma once
#include "RegistryScanner.h"
#include "IComServerChecker.h"


class CComClassRScanner : public CRegistryScanner, public ICLSIDChecker
{
protected:
    static const scanner_id m_nScannerId = COMCLASS_SCANNER_ID;
    HKEY m_hScanningKey;
    DWORD m_dwIndex;
    
    virtual void postop( void );
    virtual void algorithm( void );
    void ( CComClassRScanner::*m_pfnAlgoSelector ) ( void );

    // algorithm implementation in essence
    void algoStep1( void );
    void algoStep2( void );

    //CHECKITEM_RESULT CheckInProcServer32( const HKEY hKey );
    //CHECKITEM_RESULT CheckLocalServer32( const HKEY hKey );
    //CHECKITEM_RESULT CheckServer( const HKEY hKey, const TCHAR * szServer );

protected:
    virtual void ReinitAlgorithm( void );

	std::vector< IComServerChecker* > Checkers;

public:
    explicit CComClassRScanner( CCleanEngine * pEngine, INotifier * pNotifier = NULL, IScanProfiler * pProfiler = NULL );

    virtual ~CComClassRScanner( void );
    virtual scanner_id ScannerId( void ) const { return m_nScannerId; }

	BOOL CheckCLSID( const TCHAR * szSubKeyName );
};
