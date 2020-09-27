#pragma once

#include <AtlColl.h>
#include <vector>

#include "Runnable.h"
#include "CleanScanner.h"
#include "countable.h"

// disable warning C4290: C++ exception specification ignored 
// except to indicate a function is not __declspec(nothrow)
#pragma warning( push )
#pragma warning( disable : 4290 )

// Disable 
// warning C4172: returning address of local variable or temporary: v
#pragma warning( disable : 4172 )

/**
* Class Name: IPreAction
*
* Class Description: Interface for
* PreAdd or PreFix handlers
*/

interface IPreAction
{
public:
    virtual BOOL Process( countable< IReportItem > item ) = 0;
};

/**
* Class Name: IScanObserver
*
* Class Description: Interface for
* scan ending observer
*/

interface IScanObserver
{
public:
    virtual void Notify( COMPLETION_STATUS CompletionReason ) = 0;
};

/**
* Class Name: CCleanEngine
*
*/

class CCleanEngine :	public CRunnable
{

private:

	IScanObserver *			m_pObserver;
	INotifier *				m_pNotifier;

	CReport					m_Report;

	COMPLETION_STATUS		m_nCompletionReason;

	ATL::CAtlMap< scanner_id, countable< CCleanScanner > > m_mapScanners;
	std::vector< IPreAction * > m_vPreCleanupActions;
	std::vector< IPreAction * > m_vPreAddActions;

private:

	DEFINE_CRITICAL_UNIT(ScannersList);
	DEFINE_CRITICAL_UNIT(CompletionFlag);
	DEFINE_CRITICAL_UNIT(PreCleanupList);
	DEFINE_CRITICAL_UNIT(PreAddList);

private:

	virtual void prerun( void );
	virtual void postop( void );
	virtual void run( void );

	void SetCompletionStatus( COMPLETION_STATUS status );

public:

	CCleanEngine(	INotifier * pNotifier = NULL,
					IScanObserver * pObserver = NULL,
					IScanProfiler * pProfiler = NULL,
					BOOL bUsePostponedClean = FALSE ) throw( CGenException );

	virtual ~CCleanEngine( void );

	RNRESULT StartEngine();
	BOOL IsBusy();
	void Pause( BOOL bSuspend );
	void Stop();
	BOOL CanExit(void);

	void AddItem( countable<IReportItem> item, bool bCountIt = true );

	bool FixItem( item_id id );
	void FixAllItems();
	void CallPreCleanupHandler( item_id id );

	// DC Only
	// Блядская оптимизация ;(
	void FlushCookiesFile();
	void ClearIEHistory();

	const std::vector< countable < MruKey > > * GetMRUPathList();

	void AddPreCleanupHandler( IPreAction * pNewHandler );
	void AddPreAddHandler( IPreAction * pNewHandler );

	void InitBuildinWhiteList( const LPTSTR szWhiteListPath );

	// DC Only
	void InitAppBase( LPCTSTR lpDbFolder, BASE_SOURCE uiLoadFrom = LOAD_FROM_DAT, HINSTANCE hResInstance = NULL );

	void AddScanner( countable< CCleanScanner >& rspScanner ) throw( CGenException );
	bool RemoveScanner( const scanner_id scannerId );

	RNRESULT EnableScanner( 
		const scanner_id Id, const bool bEnable );
	bool IsScannerEnabled( const scanner_id Id );
	void EnableAllScanners( const bool bEnable );
	size_t GetScannersNum( void );
	void GetScannersList( std::vector< scanner_id >& rvScannerList );
	CCleanScanner * GetPointerToScanner(
			const scanner_id scannerId ) throw ( COutOfRangeException );

	INT GetErrorCode( scanner_id scannerId ) throw ( COutOfRangeException );
	COMPLETION_STATUS GetCompletionStatus( void );

	CReport * GetReportObject() { return &m_Report; };

public:

	volatile LONG m_bUsePostponedClean;

};

#pragma warning( pop )
