#pragma once
#include "Exception.h"
#include "RunnableStateMngr.h"

// disable warning C4290: C++ exception specification ignored 
// except to indicate a function is not __declspec(nothrow)
#pragma warning( push )
#pragma warning( disable : 4290 )

/**
* Class Name: CRunnable
* See file: 'Runnable.cpp' for the implementation of this class
*/
class CRunnable
{

	friend class CCleanEngine;

private:

	inline void LockStatusFlag( void );
	inline void UnlockStatusFlag( void );

	// Function Description: run wrapper.
	static UINT WINAPI start( void *pObj );

	// Function Description: Set state of Runnable object.
	inline void SetStatus( RNSTATUS status );

protected:
	// Function Description: run proc.
	virtual void run( void ) = 0;

	// Before run action.
	virtual void prerun( void ) = 0;

	// Function Description: Before stop action.
	virtual void postop( void ) = 0;

	// Function Description: Sets stop flag for current runnable.
	void StopJustMe();

	// Function Description: Checks stop flag for current runnable.
	HRESULT AmIStopped();

	// Function Description: Resets runnable stop flag.
	void Reset();

	HRESULT InterrogateMngr();

public:

	// Function Description: Constructor
	CRunnable() throw( CGenException );

	// Function Description: Destructor
	virtual ~CRunnable();

	// Function Description: Starts runnable thread
	RNRESULT Start( void ) throw( CGenException );

	// Function Description: Get state of Runnable object.
	RNSTATUS GetStatus() throw( CGenException );

	// Function Description: Gets runnable thread handle.
	runnable_id GetThreadHandle() { return m_hThreadId; }

private:

	volatile LONG m_lStopFlag;
	RNSTATUS m_nStatus;
	CGenException * m_pException;
	runnable_id m_hThreadId;

	CRITICAL_SECTION m_RunnableCS;

};

#pragma warning( pop )
