#pragma once
#include "RegistryScanner.h"

class CFileAssocRScanner : public CRegistryScanner
{
protected:
    static const scanner_id m_nScannerId = FILEASSOC_SCANNER_ID;
    
    HKEY    m_hScanningKey;
    DWORD   m_dwIndex;

    virtual void algorithm( void );
    void ( CFileAssocRScanner::*m_pfnAlgoSelector ) ( void );
    
    // algorithm implementation in essence
    //
    void algoStep1( void );
    void algoStep2( void );
    void algoStep3( void );
    
    virtual void postop( void );

    void CheckRegisteredType( 
        HKEY        hKey, 
        LPTSTR      lpKeyName, 
        LPTSTR      lpSubkeyName );

    CHECKITEM_RESULT CheckProgId( HKEY hKey );
    CHECKITEM_RESULT CheckPersistentHandler( HKEY hKey );
    CHECKITEM_RESULT CheckContentType( HKEY hKey );
	CHECKITEM_RESULT CheckOpenWithList1( HKEY hKey );
	CHECKITEM_RESULT CheckOpenWithList2( HKEY hKey );
	CHECKITEM_RESULT CheckOpenWithProgids1( HKEY hKey );
	CHECKITEM_RESULT CheckOpenWithProgids2( HKEY hKey );

protected:
    virtual void ReinitAlgorithm( void );

public:
    explicit CFileAssocRScanner( CCleanEngine * pEngine, INotifier * pNotifier = NULL, IScanProfiler * pProfiler = NULL );

    virtual ~CFileAssocRScanner( void );
    virtual scanner_id ScannerId( void ) const { return m_nScannerId; }
};
