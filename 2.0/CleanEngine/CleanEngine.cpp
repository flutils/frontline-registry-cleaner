#include "StdAfx.h"
#include "CleanEngine.h"
#include "ScannerFactory.h"
#include "CleanScanner.h"

#ifdef USE_DC // DriveCleaner specific scanners
	#include "AppBase.h"
#endif // USE_DC

#include "scopeguard.h"

// disable warning C4290: C++ exception specification ignored 
// except to indicate a function is not __declspec(nothrow)
#pragma warning( push )
#pragma warning( disable : 4290 )

/*************************************************************************
* CWhiteListProxy
*************************************************************************/
class CWhiteListProxy : public IPreAction
{

public:

	virtual BOOL Process( countable< IReportItem > item );

} WhiteListProxy; // Thread safety provided by PreAddList Critical Unit;

/*************************************************************************
* Function Name:        CCleanEngine::~CCleanEngine()
* Function Purpose:     Constructor
*
* Arguments:            None
* Return value:         None
*************************************************************************/
CCleanEngine::CCleanEngine(	INotifier * pNotifier /* = NULL */,
							IScanObserver * pObserver /* = NULL */,
							IScanProfiler * pProfiler /* = NULL */,
							BOOL bUsePostponedClean /* = FALSE */ ) throw( CGenException ) :
	m_pObserver( pObserver ),
	m_pNotifier( pNotifier )
{
	INIT_CRITICAL_UNIT(ScannersList);
	INIT_CRITICAL_UNIT(CompletionFlag);
	INIT_CRITICAL_UNIT(PreCleanupList);
	INIT_CRITICAL_UNIT(PreAddList);

	SetCompletionStatus( ST_WASNOTRUNNED );

	CScannerFactory sf;

	// Create All Scanners
	for( size_t i = 0; i < TotalScannersNum ; i++ )
	{
		CCleanScanner* scanner_ptr = sf.CreateScanner( ScannersSet[ i ], this, pNotifier, pProfiler );
		//alaqwer
		if(scanner_ptr == NULL)
			continue;
		countable< CCleanScanner > pScanner( scanner_ptr );
		SAFEEXEC(( pScanner.get() ));
		AddScanner( pScanner );
	}

	AddPreAddHandler( &WhiteListProxy );

	InterlockedExchange( &m_bUsePostponedClean, static_cast<LONG>( bUsePostponedClean ) );
}

/*************************************************************************
* Function Name:		CCleanEngine::~CCleanEngine()
* Function Purpose:     Destructor
*
* Arguments:            None
* Return value:         None
*************************************************************************/
CCleanEngine::~CCleanEngine()
{
	UNINIT_CRITICAL_UNIT(ScannersList);
	UNINIT_CRITICAL_UNIT(CompletionFlag);
	UNINIT_CRITICAL_UNIT(PreCleanupList);
	UNINIT_CRITICAL_UNIT(PreAddList);
}

/*************************************************************************
* Function Name:        CCleanEngine::AddScanner()
* Function Purpose:     
*
* Arguments:            None
* Return value:         None
*
*************************************************************************/
void CCleanEngine::AddScanner(
    countable< CCleanScanner >& rspScanner ) throw( CGenException )
{
	_ASSERT( rspScanner.get() );
	POSITION pos;

	LockScannersList();
	pos = m_mapScanners.SetAt( rspScanner->ScannerId(), rspScanner );
	UnlockScannersList();
}

/*************************************************************************
* Function Name:        CCleanEngine::RemoveScanner()
* Function Purpose:     
*
* Arguments:            None
* Return value:         None
*
*************************************************************************/
bool CCleanEngine::RemoveScanner( const scanner_id scannerId )
{
	LockScannersList();
	bool bRemove = m_mapScanners.RemoveKey( scannerId );
	UnlockScannersList();

	return bRemove;
}

/*************************************************************************
* Function Name:        CCleanEngine::StartEngine()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
RNRESULT CCleanEngine::StartEngine()
{
	// Note: it's still caller thread

	RNSTATUS status = GetStatus();


	if(	( status == ST_STARTING ) ||
		( status == ST_RUNNING  ) ||
		( status == ST_SLEEPING ) )
			return ERR_ALREADY_RUNNING;

	Reset();
	m_Report.Reset();
	CRunnableStateMngr::Instance().Reset();

	return Start();
}

/*************************************************************************
* Function Name:        CCleanEngine::prerun()
* Function Purpose:     It is called before run()
* Arguments:            None
* Return value:         None
*************************************************************************/
void CCleanEngine::prerun()
{
	SetCompletionStatus( ST_COMPLETED );
}

/*************************************************************************
* Function Name:        CCleanEngine::run()
* Function Purpose:     
*
* Arguments:            None
* Return value:         None
*
*************************************************************************/
void CCleanEngine::run()
{
	countable< CCleanScanner > pScanner( NULL );

	CRunnableStateMngr::Instance().SafeSetState( Working );

	for ( unsigned i = 0; i < TotalScannersNum; i++)
	{
		if( ABANDONNED == InterrogateMngr() )
		{
			SetCompletionStatus( ST_ABORTED );
			break;
		}

		if ( m_mapScanners.Lookup( ScannersSet[ i ], pScanner ) )
		{
			_ASSERT( pScanner.get() );

			if ( pScanner->IsScannerEnabled() )
			{
				pScanner->Start();

				HANDLE h[2] = {
					pScanner->GetThreadHandle(),
						CRunnableStateMngr::Instance().GetStopEventHandle()
				};

				DWORD dwWaitResult = WaitForMultipleObjects( 2, h, FALSE, INFINITE );

				if( WAIT_OBJECT_0 + 1 == dwWaitResult )
				{
					// Exit from scanning process
					break;
				}
			}
		}

	}

	//POSITION pos = m_mapScanners.GetStartPosition();
	//pScanner = m_mapScanners.GetNextValue( pos );

	//while( pos || ( pScanner.get() && pScanner->IsScannerEnabled() ) )
	//{
	//	if( ABANDONNED == InterrogateMngr() )
	//	{
	//		SetCompletionStatus( ST_ABORTED );
	//		break;
	//	}

	//	while( pos && !pScanner->IsScannerEnabled() )
	//	{
	//		pScanner = m_mapScanners.GetNextValue( pos );
	//	}

	//	if( pScanner->IsScannerEnabled() )
	//	{
	//		pScanner->Start();

	//		HANDLE h[2] = {
	//			pScanner->GetThreadHandle(),
	//			CRunnableStateMngr::Instance().GetStopEventHandle()
	//		};

	//		DWORD dwWaitResult = WaitForMultipleObjects( 2, h, FALSE, INFINITE );

	//		if( WAIT_OBJECT_0 + 1 == dwWaitResult )
	//		{
	//			// Exit from scanning process
	//			break;
	//		}

	//		if( pos )
	//			pScanner = m_mapScanners.GetNextValue( pos );
	//		else
	//			pScanner = NULL;
	//	}
	//}

	if( m_pObserver )
		m_pObserver->Notify( GetCompletionStatus() );
}

/*************************************************************************
* Function Name:		CCleanEngine::Pause()
* Function Purpose:     Suspend/resume scanning.
*
* Arguments:            bEnable - Indicate what we need to do. 
*                       Scanning will be suspended if this argument is 
*                       true and enabled otherwise.
*
* Return value:         None
*
*************************************************************************/
void CCleanEngine::Pause( BOOL bSuspend )
{
	if( bSuspend )
	{
		CRunnableStateMngr::Instance().PauseThemAll();
		SetStatus( ST_SLEEPING );
	}
	else
	{
		CRunnableStateMngr::Instance().ResumeThemAll();
		SetStatus( ST_RUNNING );
	}
}

/*************************************************************************
* Function Name:		CCleanEngine::Stop()
* Function Purpose:     Stop scanning.
*
* Arguments:            None.
* Return value:         None
*************************************************************************/
void CCleanEngine::Stop()
{
	if(CRunnableStateMngr::Instance().StopThemAll() != S_FALSE)
		SetStatus( ST_STOPPED );
}

/*************************************************************************
* Function Name:          CCleanEngine::EnableScanner()
* Function Purpose:       Enable or disable separate scanner for running.
*
* Arguments:              scannerId - Identifier of scanner which 
*                         we need be enabled or disabled.
*
*                         bEnable - Indicate what we need to do. Requested
*                         scanned will be disabled if this argument is 
*                         false and enabled otherwise.
*
* Return value:           CRunnable::ERR_SUCCESS if all ok,
*                         CRunnable::ERR_ID_NOT_FOUND if scanner with
*                         such id was not found in scanner list.
*************************************************************************/
RNRESULT CCleanEngine::EnableScanner( scanner_id Id, bool bEnable )
{
	RNRESULT result = ERR_SUCCESS;
	countable< CCleanScanner > spScanner;

	LockScannersList();

	if( m_mapScanners.Lookup( Id, spScanner ) )
	{
		_ASSERT( spScanner.get() );
		spScanner->EnableScanner( bEnable ); 
	}
	else
	{
		result = ERR_ID_NOT_FOUND;
	}

	UnlockScannersList();
	return result;
}

/*************************************************************************
* Function Name:        CCleanEngine::EnableAllScanners()
* Function Purpose:     Enable or disable all scanners containing 
*                       in CleanEngine.
*
* Arguments:            bEnable - Indicate scanners will enabled or
*                       disabled.
*
* Return value:         None.
*************************************************************************/
void CCleanEngine::EnableAllScanners( const bool bEnable )
{
	countable< CCleanScanner > spScanner( NULL );
	POSITION pos;

	LockScannersList();
	pos = m_mapScanners.GetStartPosition();

	while( pos )
	{
		spScanner = m_mapScanners.GetNextValue( pos );

		_ASSERT( spScanner.get() );
		spScanner->EnableScanner( bEnable ); 
	}

	UnlockScannersList();
}

/*************************************************************************
* Function Name:        CCleanEngine::IsScannerEnabled()
* Function Purpose:     Check is separate scanner enabled or disabled.
*
* Arguments:            Id - Identifier of scanner for which we 
*                       make request.
*
* Return value:         True if scanner with given Id is enabled, 
*                       false if scanner disabled or not exist.
*************************************************************************/
bool CCleanEngine::IsScannerEnabled( const scanner_id Id )
{
	countable< CCleanScanner > spScanner;
	bool bEnabled;

	LockScannersList();

	if( m_mapScanners.Lookup( Id, spScanner ))
	{
		_ASSERT( spScanner.get() );
		bEnabled = spScanner->IsScannerEnabled(); 
	}
	else
	{
		bEnabled = false;
	}

	UnlockScannersList();
	return bEnabled;
}

/*************************************************************************
* Function Name:        CCleanEngine::GetScannersNum()
* Function Purpose:     Get total number of scanners contained 
*                                                       in ScanEngine.
* Arguments:            None
* Return value:         None
*************************************************************************/
size_t CCleanEngine::GetScannersNum()
{
	LockScannersList();   
	size_t count = m_mapScanners.GetCount();
	UnlockScannersList();

	return count;
}

/*************************************************************************
* Function Name:          CCleanEngine::GetScannersList()
* Function Purpose:       Get list of scanner identifiers that contained
*                         in ScanEngine. 
*
* Arguments:              rvScannerList - vector where identifiers list 
*                         is saved. Vector is erased before saving 
*                         results.
*
* Return value:           None
*
*************************************************************************/
void CCleanEngine::GetScannersList( 
    std::vector< scanner_id >& rvScannerList )
{
	POSITION pos;
	scanner_id Id;

	rvScannerList.erase( rvScannerList.begin(), rvScannerList.end() );

	LockScannersList();
	pos = m_mapScanners.GetStartPosition();

	while( pos )
	{
		Id = m_mapScanners.GetNextKey( pos );
		rvScannerList.push_back( Id );
	}

	UnlockScannersList();
}

/*************************************************************************
* Function Name:          CCleanEngine::GetPointerToScanner()
* Function Purpose:       Return "pure" pointer to selected scanner.
* Arguments:              scannerId - scanner identifier
* Return value:           
*************************************************************************/
CCleanScanner * CCleanEngine::GetPointerToScanner( 
        const scanner_id scannerId ) throw ( COutOfRangeException )
{
	LockScannersList();
	countable< CCleanScanner > spScanner;

	if( m_mapScanners.Lookup( scannerId, spScanner ))
	{
		UnlockScannersList();

		_ASSERT( spScanner.get() );
		return spScanner.get();
	}

	UnlockScannersList();
	OUTOFRANGE();

	return NULL;
}

/*************************************************************************
* Function Name:            CCleanEngine::GetCompletionStatus()
* Function Description:     
* Parameters:               
* Return value:             
*************************************************************************/
COMPLETION_STATUS CCleanEngine::GetCompletionStatus()
{
	LockCompletionFlag();
	COMPLETION_STATUS status = m_nCompletionReason;
	UnlockCompletionFlag();

	return status;
}

/*************************************************************************
* Function Name:            CCleanEngine::SetCompletionStatus()
* Function Description:     
* Parameters:               
* Return value:             
*************************************************************************/
void CCleanEngine::SetCompletionStatus( COMPLETION_STATUS status )
{
	LockCompletionFlag();
	m_nCompletionReason = status;
	UnlockCompletionFlag();
}

/*************************************************************************
* Function Name:            CCleanEngine::GetLastError()
* Function Description:     
* Parameters:               
* Return value:             
*************************************************************************/
INT CCleanEngine::GetErrorCode( scanner_id scannerId )
{
	LockScannersList();
	countable< CCleanScanner > spScanner;

	if( m_mapScanners.Lookup( scannerId, spScanner ) )
	{
		UnlockScannersList();

		_ASSERT( spScanner.get() );
		return spScanner->GetErrorCode();
	}

	UnlockScannersList();
	OUTOFRANGE();

	return 0;
}

/*************************************************************************
* Function Name:            CCleanEngine::CallPreCleanupHandler()
* Function Description:     
* Parameters:				
* Return value:             
*************************************************************************/
void CCleanEngine::CallPreCleanupHandler( item_id id )
{
	if( !m_Report.IsValidItem( id ) )
		return;

	LockPreCleanupList();

	std::vector< IPreAction * >::size_type i;
	std::vector< IPreAction * >::size_type i_max = m_vPreCleanupActions.size();

	for( i = 0; i < i_max; ++i )
		m_vPreCleanupActions[i]->Process( m_Report.GetItem( id ) );

	UnlockPreCleanupList();
}

/*************************************************************************
* Function Name:            CCleanEngine::FixItem()
* Function Description:     
* Parameters:				
* Return value:             
*************************************************************************/
bool CCleanEngine::FixItem( item_id id )
{
	if( !m_Report.IsValidItem( id ) )
		return false;

	CallPreCleanupHandler( id );

	return (m_Report.FixItem( id ) == FR_FIXED);
}

/*************************************************************************
* Function Name:            CCleanEngine::FixItem()
* Function Description:     
* Parameters:				
* Return value:             
*************************************************************************/
void CCleanEngine::FixAllItems()
{
	m_Report.FixAllItems( fn::bind1st1( fn::mem_fun( &CCleanEngine::CallPreCleanupHandler ), this ) );

	FlushCookiesFile();
	ClearIEHistory();
}

/*************************************************************************
* Function Name:            CCleanEngine::AddItem()
* Function Description:     
* Parameters:				
* Return value:             
*************************************************************************/
void CCleanEngine::AddItem( countable<IReportItem> item , bool bCountIt)
{
	LockPreAddList();

	std::vector< IPreAction * >::size_type i;
	std::vector< IPreAction * >::size_type i_max = m_vPreAddActions.size();

	BOOL bRes;

	for( i = 0; i < i_max; ++i )
	{
		bRes = m_vPreAddActions[i]->Process( item );

		if( bRes )
			break;
	}

	UnlockPreAddList();

	if( !bRes )
	{
		m_Report.AddItem( item );

		if( m_pNotifier && bCountIt)
			m_pNotifier->OnItemFind( item );
	}
}

/*************************************************************************
* Function Name:            CCleanEngine::AddPreCleanupHandler()
* Function Description:     
* Parameters:				
* Return value:             
*************************************************************************/
void CCleanEngine::AddPreCleanupHandler( IPreAction * pNewHandler )
{
	LockPreCleanupList();
	m_vPreCleanupActions.push_back( pNewHandler );
	UnlockPreCleanupList();
}

/*************************************************************************
* Function Name:            CCleanEngine::AddPreAddHandler()
* Function Description:     
* Parameters:				
* Return value:             
*************************************************************************/
void CCleanEngine::AddPreAddHandler( IPreAction * pNewHandler )
{
	LockPreAddList();
	m_vPreAddActions.push_back( pNewHandler );
	UnlockPreAddList();
}

/*************************************************************************
* Function Name:            CCleanEngine::InitBuildinWhiteList()
* Function Description:     
* Parameters:				
* Return value:             
*************************************************************************/
void CCleanEngine::InitBuildinWhiteList( const LPTSTR szWhiteListPath )
{
	RegKeysWhiteList->Init( szWhiteListPath );
}

/*************************************************************************
* Function Name:            CCleanEngine::InitAppBase()
* Function Description:     
* Parameters:				
* Return value:             
*************************************************************************/
void CCleanEngine::InitAppBase(
								LPCTSTR lpDbFolder,
								BASE_SOURCE uiLoadFrom /* = LOAD_FROM_DAT */,
								HINSTANCE hResInstance /* = NULL */ )
{
#ifdef USE_DC // DriveCleaner specific scanners
	SAppBase::Instance()->Init( lpDbFolder, uiLoadFrom, hResInstance );
#endif // USE_DC
}

/*************************************************************************
* Function Name:            CCleanEngine::FlushCookiesFile()
* Function Description:     
* Parameters:				
* Return value:             
*************************************************************************/
void CCleanEngine::FlushCookiesFile()
{
	countable< CCleanScanner > spScanner;

	LockScannersList();

	if( m_mapScanners.Lookup( MOZILLA_BROWSERS_SCANNER_ID, spScanner ) )
	{
		ATLASSERT( spScanner.get() );
		spScanner->FlushCookiesFile();
	}

	if( m_mapScanners.Lookup( OPERA_SCANNER_ID, spScanner ) )
	{
		ATLASSERT( spScanner.get() );
		spScanner->FlushCookiesFile();
	}

	UnlockScannersList();
}

void CCleanEngine::ClearIEHistory()
{
	countable< CCleanScanner > spScanner;

	LockScannersList();

	if( m_mapScanners.Lookup( IEHISTORY_SCANNER_ID, spScanner ) )
	{
		ATLASSERT( spScanner.get() );
		spScanner->ClearHistory();
	}
	
	UnlockScannersList();
}

/*************************************************************************
* Function Name:            CCleanEngine::GetMRUPathList()
* Function Description:     
* Parameters:				
* Return value:             
*************************************************************************/
const std::vector< countable < MruKey > > * CCleanEngine::GetMRUPathList()
{
	countable< CCleanScanner > spScanner;

	LockScannersList();

	ON_BLOCK_EXIT_POBJ( this, &CCleanEngine::UnlockScannersList );

	if( m_mapScanners.Lookup( MRU_SCANNER_ID, spScanner ) )
		return spScanner->GetMRUPathList();

	ATLASSERT( FALSE );

	#pragma warning( push )
	#pragma warning( disable : 4172 )

	std::vector< countable < MruKey > > v;
	return &v;

	#pragma warning( pop )
}

BOOL CCleanEngine::CanExit( void )
{
	return (CRunnableStateMngr::Instance().SafeGetState() == Abandoned);
}

void CCleanEngine::postop( void )
{

}

BOOL CCleanEngine::IsBusy()
{
	RNSTATUS current_status = GetStatus();
	return (current_status == ST_STARTING)
		|| (current_status == ST_RUNNING)
		|| (current_status == ST_SLEEPING);
}
/*************************************************************************
* Function Name:            CWhiteListProxy::Process()
* Function Description:     
* Parameters:				
* Return value:             
*************************************************************************/
BOOL CWhiteListProxy::Process( countable< IReportItem > item )
{
	switch( item->m_Type )
	{
	case IT_REG_KEY:
		if( SRegKReportItem * pRegKReportItem = item.get()->GetAsRegKItem() )
		{
			if( RegKeysWhiteList->IsKeyWhite( pRegKReportItem->m_strKeyPath ) )
				return TRUE;

			if( WordsWhiteList->IsStringWhite( pRegKReportItem->m_strKeyPath ) )
				return TRUE;
		}
	break;

	case IT_REG_VALUE:
		if( SRegVReportItem * pRegVReportItem = item.get()->GetAsRegVItem() )
		{
			if( RegKeysWhiteList->IsKeyWhite( pRegVReportItem->m_strKeyPath ) )
				return TRUE;

			if( RegKeysWhiteList->IsSubkeyWhite( pRegVReportItem->m_strSubkeyName ) )
				return TRUE;

			if( WordsWhiteList->IsStringWhite( pRegVReportItem->m_strKeyPath ) )
				return TRUE;

			if( WordsWhiteList->IsStringWhite( pRegVReportItem->m_strSubkeyName ) )
				return TRUE;
		}
	break;

	case IT_FILE:
		if( SFileReportItem * pFileReportItem = item.get()->GetAsFileItem() )
		{
			if( FilesWhiteList->IsFileWhite( pFileReportItem->m_strFile ) )
				return TRUE;

			if( WordsWhiteList->IsStringWhite( pFileReportItem->m_strDirectory ) )
				return TRUE;

			if( WordsWhiteList->IsStringWhite( pFileReportItem->m_strFile ) )
				return TRUE;
		}
	break;

	case IT_DIRECTORY:
		if( SFileReportItem * pFileReportItem = item.get()->GetAsFileItem() )
		{
			if( FilesWhiteList->IsDirectoryWhite( pFileReportItem->m_strDirectory ) )
				return TRUE;

			if( WordsWhiteList->IsStringWhite( pFileReportItem->m_strDirectory ) )
				return TRUE;
		}
	break;

	case IT_OTHER:
		// Custom threats can't be chacked
	break;

	default:
		ATLASSERT( FALSE );
	}

	return FALSE;
}

#pragma warning( pop )

