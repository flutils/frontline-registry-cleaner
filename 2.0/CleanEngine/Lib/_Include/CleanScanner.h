#pragma once

#include <atlcoll.h>
#include "Runnable.h"
#include "countable.h"
#include "scanners.h"
#include "Report.h"
#include "Stera.h"
#include "WhiteList.h"

#include <string>
#include <list>

// disable warning C4290: C++ exception specification ignored 
// except to indicate a function is not __declspec(nothrow)
#pragma warning( push )
#pragma warning( disable : 4290 )

// disable warning warning C4172
// returning address of local variable or temporary: v
#pragma warning( disable : 4172 )

// Used for control of recursive function calling depth.
//
const ULONG MaxRecurseLevel = 20;

// Scanner use this class to notify user about every event.
//
interface INotifier
{
	virtual void OnScan(
		const scanner_id scannerId,
		const TCHAR * szObjectName,
		const TCHAR * szValueName ) = 0;

	virtual void OnItemFind( countable< IReportItem > item ) = 0;
	virtual void OnItemFix( countable< IReportItem > item ) = 0;

	virtual void OnScannerStarted( const scanner_id scannerId ) = 0;
	virtual void OnScannerFinished( const scanner_id scannerId ) = 0;

	inline void CallOnScan(
		const scanner_id scannerId,
		const TCHAR * szObjectName,
		const TCHAR * szValueName = NULL )
	{
		OnScan(
			scannerId,
			lstrlen( szObjectName ) ? szObjectName : NULL,
			lstrlen( szValueName ) ? szValueName : NULL
			);
	}

};

interface IScanProfiler
{
	virtual BOOL OnGetDWORD( param_id id, DWORD & dwParam ) = 0;
	virtual BOOL OnGetString( param_id id, std::string & strParam ) = 0;
	virtual BOOL OnGetStringVector( param_id id, std::vector< std::string > & vParam ) = 0;
	virtual BOOL OnGetStringList( param_id id, std::list< std::string > & lstParam ) = 0;

	virtual BOOL OnGetStringW( param_id id, std::wstring & strParam ) = 0;
	virtual BOOL OnGetStringWVector( param_id id, std::vector< std::wstring > & vParam ) = 0;
	virtual BOOL OnGetStringWList( param_id id, std::list< std::wstring > & lstParam ) = 0;

	BOOL GetDWORD( param_id id, DWORD & dwParam )
	{
		return OnGetDWORD( id, dwParam );
	}

	BOOL GetString( param_id id, std::string & strParam )
	{
		return OnGetString( id, strParam );
	}

	BOOL GetStringVector( param_id id, std::vector< std::string > & vParam )
	{
		return OnGetStringVector( id, vParam );
	}

	BOOL GetStringList( param_id id, std::list< std::string > & listParam )
	{
		return OnGetStringList( id, listParam );
	}


	BOOL GetStringW( param_id id, std::wstring & strParam )
	{
		return OnGetStringW( id, strParam );
	}

	BOOL GetStringWVector( param_id id, std::vector< std::wstring > & vParam )
	{
		return OnGetStringWVector( id, vParam );
	}

	BOOL GetStringWList( param_id id, std::list< std::wstring > & listParam )
	{
		return OnGetStringWList( id, listParam );
	}
};

class CCleanScanner;
class CCleanEngine;

typedef void ( CCleanScanner::*AlgoSelector )( void );

class CCleanScanner : public CRunnable
{
	bool m_bEnabled;
	INT m_nLastError;

	DEFINE_CRITICAL_UNIT(ErrorFlag);

	virtual void run( void );

protected:
	INotifier		* m_pNotifier;
	CCleanEngine	* m_pEngine;
	IScanProfiler	* m_pProfiler;

	// Redefine this function to implement scanning algorithm.
	// Function must return control after each iteration.
	virtual void algorithm( void ) = 0;
	virtual void ReinitAlgorithm( void ) = 0;

	virtual void prerun( void );
	virtual void postop( void );

	void SetErrorCode( const INT nErrCode );

public:
	explicit CCleanScanner( CCleanEngine * pEngine, INotifier * pNotifier = NULL, IScanProfiler * pProfiler = NULL );
	virtual ~CCleanScanner( void );
	virtual scanner_id ScannerId() const = 0;

	virtual void CleanupObject( countable< IReportItem > item ) = 0;

	bool IsScannerEnabled() const { return m_bEnabled; }
	void EnableScanner( bool bEnable ) { m_bEnabled = bEnable; }

	INT GetErrorCode( void );

	//DC Only
	virtual void FlushCookiesFile() {}
	virtual void ClearHistory()	{}

	//FT Only
	virtual const std::vector< countable < MruKey > > * GetMRUPathList()
	{
		ATLASSERT( FALSE );

		#pragma warning( push )
		#pragma warning( disable : 4172 )

		std::vector< countable < MruKey > > v;
		return &v;

		#pragma warning( pop )
	}
};

#define ON_FIX( item ) if( m_pNotifier ) { m_pNotifier->OnItemFix( item ); }
#define SET_ITEM_STATUS( item, status )\
	item->m_Status = status;\
	if( m_pNotifier ) { m_pNotifier->OnItemFix( item ); }

#pragma warning( pop )
