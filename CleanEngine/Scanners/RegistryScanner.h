#pragma once

#include "CleanScanner.h"
#include "RegistryTools.h"

class ICLSIDChecker
{
public:
	virtual BOOL CheckCLSID( const TCHAR * szSubKeyName, const TCHAR * szRoot, BOOL bAddKeys = TRUE ) = 0;
	virtual BOOL CheckProgID( const TCHAR * szSubKeyName, const TCHAR * szRoot, BOOL bAddKeys ) = 0;
	virtual BOOL CLSIDIsInReport( const TCHAR * szCLSID, const TCHAR * szRoot ) = 0;
	virtual BOOL ProgIDIsInReport( const TCHAR * szProgID, const TCHAR * szRoot ) = 0;
	virtual BOOL PathIsInReport( const TCHAR * szPath ) = 0;
	virtual BOOL TypeLibIsInReport( const TCHAR * szTypeLib ) = 0;
	virtual BOOL UninstallIsInReport( const TCHAR * szUninstall ) = 0;
	//virtual BOOL InterfaceIsInReport( const TCHAR * szInteface ) = 0;
};

class CRegistryScanner : virtual public CCleanScanner, virtual public CRegistryTools
{
//protected:
//	
//	BOOL IsFakeCLSID ( const TCHAR * szCLSID );
//	BOOL IsFakePath( const TCHAR * szPath );

public:
	explicit CRegistryScanner( CCleanEngine * pEngine, INotifier * pNotifier = NULL, IScanProfiler * pProfiler = NULL );

	void RecursivelyAddKey(
        const HKEY      hKey,
        const ce_tstring&  rstrFullKeyName,
		const SEVERITY	severity = SEVERITY_MEDIUM,
        const unsigned  nDepth = 0 ,
		bool bEmtyKey = false);

	virtual void CleanupObject( countable< IReportItem > item );

};
