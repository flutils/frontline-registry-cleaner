#pragma once
#include "RegistryScanner.h"
#include "IComServerChecker.h"

#define RS_USER 0x00000000
#define RS_ADMIN 0x00000001
#define RS_SUPER 0x00000002

class CComObjectRScanner : public CRegistryScanner, public ICLSIDChecker
{
protected:
	static const scanner_id m_nScannerId = COMOBJECT_SCANNER_ID;
	HKEY m_hScanningKey;
	HKEY m_hRootKey;
	DWORD m_dwIndex;
	DWORD m_dwStep;
	DWORD m_dwTop;

	virtual void postop( void );
	virtual void algorithm( void );
	void ( CComObjectRScanner::*m_pfnAlgoSelector ) ( void );

	// algorithm implementation in essence
	void algoStep1( void );
	void algoStep2( void );
	void algoStep3( void );
	void algoStep4( void );
	void algoStep5( void );

	BOOL CheckPotentialThreat( HKEY hKey );

	CHECKPATH_RESULT CheckKeyForPath(
		HKEY        hKey, 
		TCHAR *     szKeyName, 
		unsigned    nDepth = 0 );

	void CheckInterface( const TCHAR * szKeyName, const TCHAR * szRoot );
	void CheckTypeLib( HKEY hKey, const TCHAR * szKeyName);

protected:
	virtual void ReinitAlgorithm( void );

	std::vector< IComServerChecker* > Checkers;

	std::vector< CString > TreatCLSID;
	std::vector< CString > TreatTypeLib;
	std::vector< CString > Roots;
	std::vector< CString > CheckingNowCLSID;
	std::map< int, CString> SubKeys;

public:
	explicit CComObjectRScanner( CCleanEngine * pEngine, INotifier * pNotifier = NULL, IScanProfiler * pProfiler = NULL );
	~CComObjectRScanner( void );

	virtual scanner_id ScannerId( void ) const { return m_nScannerId; }

	BOOL CheckCLSID( const TCHAR * szSubKeyName, const TCHAR * szRoot, BOOL bAddKeys = TRUE );
	BOOL CheckProgID( const TCHAR * szSubKeyName, const TCHAR * szRoot, BOOL bAddKeys );

	BOOL CLSIDIsInReport( const TCHAR * szCLSID, const TCHAR * szRoot );
	BOOL ProgIDIsInReport( const TCHAR * szCLSID, const TCHAR * szRoot );
	BOOL PathIsInReport( const TCHAR * szPath );
	BOOL TypeLibIsInReport( const TCHAR * szTypeLib );
	//BOOL InterfaceIsInReport( const TCHAR * szInteface );
	BOOL UninstallIsInReport( const TCHAR * szUninstall );
	BOOL CLSIDIsCheckingNow( const TCHAR * szCLSID );
};
