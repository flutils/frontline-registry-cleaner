#include "StdAfx.h"
#include "Runnable.h"
#include <process.h>

// disable warning C4290: C++ exception specification ignored 
// except to indicate a function is not __declspec(nothrow)
#pragma warning( push )
#pragma warning( disable : 4290 )

/*************************************************************************
* Function Name:            CRunnable::CRunnable()
* Function Description:     Constructor
*
* Parameters:               pObserver - Pointer to class used for 
*                           scan complete notification.
*************************************************************************/
CRunnable::CRunnable() :
    m_pException( NULL ),
    m_hThreadId( NULL )
{
	InitializeCriticalSection( &m_RunnableCS );
	SetStatus( ST_CREATED );
}

/*************************************************************************
* Function Name:            CRunnable::~CRunnable()
* Function Description:     Destructor
*************************************************************************/
CRunnable::~CRunnable()
{
	DeleteCriticalSection( &m_RunnableCS );

	if( m_hThreadId )
    {
        SAFEEXEC(( CloseHandle( m_hThreadId ) ));
    }

    delete m_pException;
}

/*************************************************************************
* Function Name:            CRunnable::LockStatusFlag()
* Function Description:     
* Parameters:               
*************************************************************************/
inline void CRunnable::LockStatusFlag()
{
	EnterCriticalSection( &m_RunnableCS );
}

/*************************************************************************
* Function Name:            CRunnable::UnlockStatusFlag()
* Function Description:     
* Parameters:               
*************************************************************************/
inline void CRunnable::UnlockStatusFlag()
{
	LeaveCriticalSection( &m_RunnableCS );
}

/*************************************************************************
* Function Name:            CRunnable::start()
* Function Description:     
* Parameters:               
*************************************************************************/
UINT WINAPI CRunnable::start( void *pObj )
	{
	CRunnable * th = reinterpret_cast< CRunnable * >( pObj );

	try
	{
		th->prerun();

		th->Reset();

		th->SetStatus( ST_RUNNING );
		th->run();
		th->SetStatus( ST_STOPPED );

		th->postop();
	}
	catch( CGenException * ex )
	{
		th->SetStatus( ST_ERROR );
		delete th->m_pException;
		th->m_pException = ex;
	}
	catch( std::exception & )
	{
		th->SetStatus( ST_ERROR );
	}

    return 0;
}

/*************************************************************************
* Function Name:			CRunnable::Start()
* Function Description:		Starts runnable thread
* Parameters:				none
* Return value:				Runnable result
*							ERR_SUCCESS\ERR_ALREADY_RUNNING
*************************************************************************/
RNRESULT CRunnable::Start() throw( CGenException )
{
    RNRESULT result = ERR_SUCCESS;
	unsigned tid;

	RNSTATUS status = GetStatus();


    if(( status != ST_STARTING ) &&
       ( status != ST_RUNNING  ) &&
       ( status != ST_SLEEPING ))
    {
		SetStatus( ST_STARTING );

		if( m_hThreadId )
        {
            CloseHandle( m_hThreadId );
            m_hThreadId = NULL;
        }

	    m_hThreadId = reinterpret_cast< HANDLE >(
            _beginthreadex(
                    NULL,			  // security attributes
                    0,				  // use default stack size
                    start,			  // thread function
                    this,			  // argument list
                    CREATE_SUSPENDED, // suspended 
                    &tid			  // out: thread id
                )
            );

		/*CHECK_IT_LATER*/

		//[jenya fix]
		SetThreadPriority(m_hThreadId, THREAD_PRIORITY_BELOW_NORMAL);
		ResumeThread(m_hThreadId);
        SAFEEXEC( m_hThreadId );
    }
    else
    {
        result = ERR_ALREADY_RUNNING;
    }

    return result;
}

/*************************************************************************
* Function Name:            CRunnable::SetStatus()
* Function Description:     Set state of Runnable object.
* Parameters:               status - Runnable object state to be set.
*************************************************************************/
inline void CRunnable::SetStatus( RNSTATUS status )
{
    LockStatusFlag();
    m_nStatus = status;
    UnlockStatusFlag();
}

/*************************************************************************
* Function Name:            CRunnable::GetStatus()
* Function Description:     Get current runnable object state.
* Parameters:               None.
* Return value:             Current runnable object state.
*************************************************************************/
RNSTATUS CRunnable::GetStatus() throw( CGenException )
{
	RNSTATUS status;
	
    if( m_pException )
	{
		CGenException * ex = m_pException;
		m_pException = NULL;
		throw ex;
	}

    LockStatusFlag();
    status = m_nStatus;
    UnlockStatusFlag();
    
    return status;
}

/*************************************************************************
* Function Name:            CRunnable::Reset()
* Function Description:     Resets runnable stop flag
* Parameters:               None.
* Return value:             None.
*************************************************************************/
void CRunnable::Reset()
{
	InterlockedExchange( &m_lStopFlag, FALSE );
}

/*************************************************************************
* Function Name:            CRunnable::StopJustMe()
* Function Description:     Sets stop flag for current runnable.
* Parameters:               None.
* Return value:             None.
*************************************************************************/
void CRunnable::StopJustMe()
{
	InterlockedExchange( &m_lStopFlag, TRUE );
}

/*************************************************************************
* Function Name:            CRunnable::AmIStopped()
* Function Description:     Checks stop flag for current runnable.
* Parameters:               None.
* Return value:             ABANDONNED or NONE.
*************************************************************************/
HRESULT CRunnable::AmIStopped()
{
	if( InterlockedCompareExchange( &m_lStopFlag, TRUE, TRUE ) )
		return ABANDONNED;

	return NONE;
}

HRESULT CRunnable::InterrogateMngr()
{
	/*NAV_POINT*/
	HRESULT hRes = CRunnableStateMngr::Instance().IsQuitPauseRequest();

	if( PAUSED == hRes )
	{
		SetStatus( ST_SLEEPING );
		hRes = CRunnableStateMngr::Instance().GoToSleep();
		SetStatus( ST_RUNNING );
	}

	if( hRes != ABANDONNED )
		hRes = AmIStopped();

	return hRes;
}

#pragma warning( pop )
