#pragma once
#include "RegistryScanner.h"
#include "ReportChecker.h"

typedef enum{ C_FAILED, C_SUCCESS, C_SKIP, C_TREAT } CHECK_RESULT;

class IComServerChecker
{
protected:
	ICLSIDChecker* m_Parent; 

public:
	IComServerChecker(ICLSIDChecker* parent);
	virtual ~IComServerChecker();

	virtual CHECK_RESULT Check( HKEY hKey, const TCHAR* szKeyName, HKEY hRootKey, const TCHAR * szRoot ) = 0;
};
/*
*	Class for checking <CLSID>\InprocServer32 key
*/
class CInprocServerChecker: public IComServerChecker, public CRegistryTools
{
public:
	explicit CInprocServerChecker( ICLSIDChecker * parent, CCleanEngine* pEngine );
	virtual ~CInprocServerChecker( void );

	CHECK_RESULT Check( HKEY hKey, const TCHAR* szKeyName, HKEY hRootKey, const TCHAR * szRoot );
};

/*
*	Class for checking <CLSID>\LocalServer32 key
*/
class CLocalServerChecker: public IComServerChecker, public CRegistryTools
{
public:
	explicit CLocalServerChecker( ICLSIDChecker * parent, CCleanEngine* pEngine );
	virtual ~CLocalServerChecker( void );

	CHECK_RESULT Check( HKEY hKey, const TCHAR* szKeyName, HKEY hRootKey, const TCHAR * szRoot );
};

/*
*	Class for checking <CLSID>\InprocHandler32 key
*/
class CInprocHandlerChecker: public IComServerChecker, public CRegistryTools
{
public:
	explicit CInprocHandlerChecker( ICLSIDChecker * parent, CCleanEngine* pEngine );
	virtual ~CInprocHandlerChecker( void );

	CHECK_RESULT Check( HKEY hKey, const TCHAR* szKeyName, HKEY hRootKey, const TCHAR * szRoot );
};

/*
*	Class for checking <CLSID>\TypeLib key
*/
class CTypeLibChecker: public IComServerChecker, public CRegistryTools
{
public:
	explicit CTypeLibChecker( ICLSIDChecker * parent, CCleanEngine* pEngine );
	virtual ~CTypeLibChecker( void );

	CHECK_RESULT Check( HKEY hKey, const TCHAR* szKeyName, HKEY hRootKey, const TCHAR * szRoot );
};

/*
*	Class for checking <CLSID>\ProgID key
*/
class CProgIDChecker: public IComServerChecker, public CRegistryTools
{
public:
	explicit CProgIDChecker( ICLSIDChecker * parent, CCleanEngine* pEngine  );
	virtual ~CProgIDChecker( void );

	CHECK_RESULT Check( HKEY hKey, const TCHAR* szKeyName, HKEY hRootKey, const TCHAR * szRoot );
};

/*
*	Class for checking <CLSID>\AppID key
*/
class CAppIDChecker: public IComServerChecker, public CRegistryTools
{
public:
	explicit CAppIDChecker( ICLSIDChecker * parent, CCleanEngine* pEngine  );
	virtual ~CAppIDChecker( void );

	CHECK_RESULT Check( HKEY hKey, const TCHAR* szKeyName, HKEY hRootKey, const TCHAR * szRoot );
};

/*
*	Class for checking <CLSID>\AutoConvertTo key
*/
class CAutoConvertToChecker: public IComServerChecker, public CRegistryTools
{
public:
	explicit CAutoConvertToChecker( ICLSIDChecker * parent, CCleanEngine* pEngine  );
	virtual ~CAutoConvertToChecker( void );

	CHECK_RESULT Check( HKEY hKey, const TCHAR* szKeyName, HKEY hRootKey, const TCHAR * szRoot );
};

/*
*	Class for checking <CLSID>\AutoTreatAs key
*/
class CAutoTreatAsChecker: public IComServerChecker, public CRegistryTools
{
public:
	explicit CAutoTreatAsChecker( ICLSIDChecker * parent, CCleanEngine* pEngine  );
	virtual ~CAutoTreatAsChecker( void );

	CHECK_RESULT Check( HKEY hKey, const TCHAR* szKeyName, HKEY hRootKey, const TCHAR * szRoot );
};

/*
*	Class for checking <CLSID>\TreatAs key
*/
class CTreatAsChecker: public IComServerChecker, public CRegistryTools
{
public:
	explicit CTreatAsChecker( ICLSIDChecker * parent, CCleanEngine* pEngine  );
	virtual ~CTreatAsChecker( void );

	CHECK_RESULT Check( HKEY hKey, const TCHAR* szKeyName, HKEY hRootKey, const TCHAR * szRoot );
};

/*
*	Class for checking <CLSID>\PersistentHandler key
*/
class CPersistentHandlerChecker: public IComServerChecker, public CRegistryTools
{
public:	
	explicit CPersistentHandlerChecker( ICLSIDChecker * parent, CCleanEngine* pEngine );
	virtual ~CPersistentHandlerChecker( void );

	CHECK_RESULT Check( HKEY hKey, const TCHAR* szKeyName, HKEY hRootKey, const TCHAR * szRoot );
};

/*
*	Class for checking <CLSID>\PersistentHandler key
*/
class CPersistentAddinsRegisteredChecker: public IComServerChecker, public CRegistryTools
{
public:
	explicit CPersistentAddinsRegisteredChecker( ICLSIDChecker * parent, CCleanEngine* pEngine );
	virtual ~CPersistentAddinsRegisteredChecker( void );

	CHECK_RESULT Check( HKEY hKey, const TCHAR* szKeyName, HKEY hRootKey, const TCHAR * szRoot );
};

class CImplementedCategoriesChecker: public IComServerChecker, public CRegistryTools
{
public:
	explicit CImplementedCategoriesChecker( ICLSIDChecker * parent, CCleanEngine* pEngine );
	virtual ~CImplementedCategoriesChecker( void );

	CHECK_RESULT Check( HKEY hKey, const TCHAR* szKeyName, HKEY hRootKey, const TCHAR * szRoot );
};

/*
*	Class for skipping .Net <CLSID> keys
*/
class CNetSkipper: public IComServerChecker, public CRegistryTools
{
public:
	explicit CNetSkipper( ICLSIDChecker * parent, CCleanEngine* pEngine  );
	virtual ~CNetSkipper( void );

	CHECK_RESULT Check( HKEY hKey, const TCHAR* szKeyName, HKEY hRootKey, const TCHAR * szRoot );
};

/*
*
*/
class CEmptyKeyChecker: public IComServerChecker, public CRegistryTools
{
public:
	explicit CEmptyKeyChecker ( ICLSIDChecker * parent, CCleanEngine * pEngine);
	virtual ~CEmptyKeyChecker ( void );

	CHECK_RESULT Check( HKEY hKey, const TCHAR* szKeyName, HKEY hRootKey, const TCHAR * szRoot );
};