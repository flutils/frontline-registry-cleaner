#include "StdAfx.h"
#include "CleanScanner.h"
#include "CleanEngine.h"

/*************************************************************************
* Function Name:        CCleanScanner::CCleanScanner
* Function Purpose:     Constructor
*
* Arguments:			pNotifier - contains callback to notify user
*                       about new objects found during scanning.
*************************************************************************/
CCleanScanner::CCleanScanner( CCleanEngine * pEngine, INotifier * pNotifier /* = NULL */, IScanProfiler * pProfiler /* = NULL */ ) :
    m_pNotifier( pNotifier ),
	m_pEngine( pEngine ),
	m_pProfiler( pProfiler ),
    m_bEnabled( false )
{
	INIT_CRITICAL_UNIT(ErrorFlag);
    SetErrorCode( ERROR_SUCCESS );
}

/*************************************************************************
* Function Name:        CCleanScanner::~CCleanScanner
* Function Purpose:     Destructor
*************************************************************************/
CCleanScanner::~CCleanScanner()
{
	UNINIT_CRITICAL_UNIT(ErrorFlag);
}

/*************************************************************************
* Function Name:        CCleanScanner::run()
* Function Purpose:     Main Loop
* Arguments:            None
* Return Value:         None
*************************************************************************/
void CCleanScanner::run()
{
    if ( !IsScannerEnabled() ) 
    {
        return;
    }
    
    while( 1 )
    {
		if( ABANDONNED == InterrogateMngr() )
		{
			break;
		}

        algorithm();
    }
}

/*************************************************************************
* Function Name:    CCleanScanner::prerun()
* Function Purpose: Called by CRunnable before run() function started. 
*
* Arguments:        None
* Return value:     None
*************************************************************************/
void CCleanScanner::prerun()
{
    ReinitAlgorithm();

	if( m_pNotifier )
	{
		m_pNotifier->OnScannerStarted( ScannerId() );
	}
}

/*************************************************************************
* Function Name:    CCleanScanner::postop()
*
* Function Purpose: Called by CRunnable after run() function completed. 
*                   Performs correctly thread completion (close opened 
*                   handles etc.)
*
* Arguments:        None
* Return value:     None
*************************************************************************/
void CCleanScanner::postop()
{
	if( m_pNotifier && ABANDONNED == AmIStopped() )
	{
		m_pNotifier->OnScannerFinished( ScannerId() );
	}
}

/*************************************************************************
* Function Name:      CCleanScanner::GetErrorCode()
* Function Purpose:   Return last error code occured in 
*                     scanner object. See details in winerror.h
* Arguments:              
* Return value:       Error codes defined in winerror.h
*************************************************************************/
INT CCleanScanner::GetErrorCode( void )
{
	LockErrorFlag();
	INT nLastError = m_nLastError;
	UnlockErrorFlag();

	return nLastError;
}

/*************************************************************************
* Function Name:      CCleanScanner::SetErrorCode()
* Function Purpose:   Sets last error code occured in 
*                     scanner object. See details in winerror.h
* Arguments:              
* Return value:       Error codes defined in winerror.h
*************************************************************************/
void CCleanScanner::SetErrorCode( const INT nErrCode )
{
	LockErrorFlag();
    m_nLastError = nErrCode;
	UnlockErrorFlag();
}
