#include "StdAfx.h"
#include "RunnableStateMngr.h"
#include "Exception.h"

/*************************************************************************
* Function Name:            CRunnableStateMngr::CRunnableStateMngr()
* Function Description:     Constructor
*************************************************************************/
CRunnableStateMngr::CRunnableStateMngr()
{
	SAFEEXEC( m_hResumeEvent = CreateEvent( NULL, TRUE, FALSE, NULL ) );
	SAFEEXEC( m_hStopEvent = CreateEvent( NULL, TRUE, FALSE, NULL ) );

	InitializeCriticalSection( &cs );

	Reset();
}

/*************************************************************************
* Function Name:            CRunnableStateMngr::~CRunnableStateMngr()
* Function Description:     Destructor
*************************************************************************/
CRunnableStateMngr::~CRunnableStateMngr()
{

	DeleteCriticalSection( &cs );

	CloseHandle( m_hResumeEvent );
	CloseHandle( m_hStopEvent );
}

/*************************************************************************
* Function Name:            CRunnableStateMngr::Instance()
* Function Description:     Create Instance of the CRunnableStateMngr
*************************************************************************/
CRunnableStateMngr & CRunnableStateMngr::Instance()
{
	static CRunnableStateMngr obj;
	return obj;
}

/*************************************************************************
* Function Name:            CRunnableStateMngr::Reset()
* Function Description:     Sets CRunnableStateMngr to defaults ( when new scan started )
*************************************************************************/
void CRunnableStateMngr::Reset()
{
	InterlockedExchange( &m_lStopFlag, FALSE );
	InterlockedExchange( &m_lPauseFlag, FALSE );

	ResetEvent( m_hResumeEvent );
	ResetEvent( m_hStopEvent );

	SafeSetState( Abandoned );
}

/*************************************************************************
* Function Name:            CRunnableStateMngr::SafeGetState()
* Function Description:     Gets entire CleanEngine state
*************************************************************************/
RunnableState CRunnableStateMngr::SafeGetState()
{
	RunnableState state;

	LockStateFlag();
	state = m_State;
	UnlockStateFlag();

	return state;
}

/*************************************************************************
* Function Name:            CRunnableStateMngr::SafeSetState()
* Function Description:     Sets entire CleanEngine state
*************************************************************************/
void CRunnableStateMngr::SafeSetState( RunnableState NewState )
{
	LockStateFlag();
	m_State = NewState;
	UnlockStateFlag();
}

/*************************************************************************
* Function Name:            CRunnableStateMngr::StopThemAll()
* Function Description:     Stopps all Scanners and CleanEngine pool
*************************************************************************/
HRESULT CRunnableStateMngr::StopThemAll()
{
	if( SafeGetState() == Abandoned )
		return S_FALSE;

	ResetEvent( m_hResumeEvent );

	InterlockedExchange( &m_lPauseFlag, FALSE );

	//SafeGetState() == Sleeping ?
	//	SetEvent( m_hStopEvent ):	// Stop them through kernel
	//	ResetEvent( m_hStopEvent );	// Stop them fast

	SetEvent( m_hStopEvent );

	InterlockedExchange( &m_lStopFlag, TRUE );

	SafeSetState( Abandoned );

	return S_OK;
}

/*************************************************************************
* Function Name:            CRunnableStateMngr::PauseThemAll()
* Function Description:     Pause all Scanners and CleanEngine pool
*************************************************************************/
HRESULT CRunnableStateMngr::PauseThemAll()
{
	if( SafeGetState() == Sleeping )
		return S_FALSE;

	ResetEvent( m_hResumeEvent );
	ResetEvent( m_hStopEvent );
	InterlockedExchange( &m_lStopFlag, FALSE );
	InterlockedExchange( &m_lPauseFlag, TRUE );

	SafeSetState( Sleeping );

	return S_OK;
}

/*************************************************************************
* Function Name:            CRunnableStateMngr::ResumeThemAll()
* Function Description:     Resume all Scanners and CleanEngine pool
*************************************************************************/
HRESULT CRunnableStateMngr::ResumeThemAll()
{
	if( SafeGetState() != Sleeping )
		return S_FALSE;

	SetEvent( m_hResumeEvent );
	ResetEvent( m_hStopEvent );
	InterlockedExchange( &m_lStopFlag, FALSE );
	InterlockedExchange( &m_lPauseFlag, FALSE );

	SafeSetState( Working );

	return S_OK;
}

/*************************************************************************
* Function Name:            CRunnableStateMngr::IsQuitPauseRequest()
* Function Description:     Asks CRunnableStateMngr for Quit or Pause necessity
*************************************************************************/
HRESULT CRunnableStateMngr::IsQuitPauseRequest()
{
	if( InterlockedCompareExchange( &m_lStopFlag, TRUE, TRUE ) )
		return ABANDONNED;

	if( InterlockedCompareExchange( &m_lPauseFlag, TRUE, TRUE ) )
		return PAUSED;

	return NONE;
}

/*************************************************************************
* Function Name:            CRunnableStateMngr::GoToSleep()
* Function Description:     Wait for Resuming or Stopping
*************************************************************************/
HRESULT CRunnableStateMngr::GoToSleep()
{
	HANDLE h[2] = { m_hResumeEvent, m_hStopEvent };
	DWORD dwWaitResult = WaitForMultipleObjects( 2, h, FALSE, INFINITE );

	switch( dwWaitResult )
	{
	case WAIT_OBJECT_0:
		SafeSetState( Working );
		return RESUMED;

	case WAIT_OBJECT_0 + 1:
		SafeSetState( Abandoned );
		return ABANDONNED;

	default:
		return S_FALSE;
	}
}
