#pragma once

#include "CETypes.h"

/**
* Class Name: CRunnableStateMngr
*
* Class Description: Used for Stop\Pause\Resume
* CRunnable threads in CleanEngine scope
*
*/

class CRunnableStateMngr
{

private:

	/*************************************************************************
	* Function Description:     Constructor
	*************************************************************************/
	CRunnableStateMngr();

public:

	/*************************************************************************
	* Function Description:     Destructor
	*************************************************************************/
	~CRunnableStateMngr();

	/*************************************************************************
	* Function Description:     Create Instance of the CRunnableStateMngr
	*************************************************************************/
	static CRunnableStateMngr & Instance();

	/*************************************************************************
	* Function Description:     Sets CRunnableStateMngr to defaults ( when new scan started )
	*************************************************************************/
	void Reset();

	/*************************************************************************
	* Function Description:     Gets entire CleanEngine state
	*************************************************************************/
	RunnableState SafeGetState();

	/*************************************************************************
	* Function Description:     Sets entire CleanEngine state
	*************************************************************************/
	void SafeSetState( RunnableState NewState );

	/*************************************************************************
	* Function Description:     Stopps all Scanners and CleanEngine pool
	*************************************************************************/
	HRESULT StopThemAll();

	/*************************************************************************
	* Function Description:     Pause all Scanners and CleanEngine pool
	*************************************************************************/
	HRESULT PauseThemAll();

	/*************************************************************************
	* Function Description:     Resume all Scanners and CleanEngine pool
	*************************************************************************/
	HRESULT ResumeThemAll();

	/*************************************************************************
	* Function Description:     Asks CRunnableStateMngr for Quit or Pause necessity
	*************************************************************************/
	HRESULT IsQuitPauseRequest();

	/*************************************************************************
	* Function Description:     Wait for Resuming or Stopping
	*************************************************************************/
	HRESULT GoToSleep();

	/*************************************************************************
	* Function Description:     Gets Stop event handle
	*************************************************************************/
	HANDLE GetStopEventHandle() { return m_hStopEvent; }

private:

	void LockStateFlag() { EnterCriticalSection( &cs ); }
	void UnlockStateFlag() { LeaveCriticalSection( &cs ); }

	volatile LONG m_lStopFlag;
	volatile LONG m_lPauseFlag;

	HANDLE m_hResumeEvent;
	HANDLE m_hStopEvent;

	RunnableState m_State;

	CRITICAL_SECTION cs;

};
