#include "StdAfx.h"
#include "TempFilesScanner.h"
#include "CleanEngine.h"

/*************************************************************************
* Function Name:          CTempFilesScanner::CTempFilesScanner()
* Function Description:   Constructor
*************************************************************************/
CTempFilesScanner::CTempFilesScanner( CCleanEngine * pEngine, INotifier * pNotifier /* = NULL */, IScanProfiler * pProfiler /* = NULL */ ) :
    CCleanScanner( pEngine, pNotifier, pProfiler ),
	CFileScanner( pEngine, pNotifier, pProfiler ),
    m_pfnAlgoSelector( NULL )
{
}

/*************************************************************************
* Function Name:          CTempFilesScanner::~CTempFilesScanner()
* Function Description:   Destructor
*************************************************************************/
CTempFilesScanner::~CTempFilesScanner()
{
}

/*************************************************************************
* Function Name:        CTempFilesScanner::ReinitAlgorithm()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CTempFilesScanner::ReinitAlgorithm()
{
    m_pfnAlgoSelector = &CTempFilesScanner::algoStep1;
}

/*************************************************************************
* Function Name:        CTempFilesScanner::algorithm()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CTempFilesScanner::algorithm()
{
    ( this->*m_pfnAlgoSelector )();
}

/*************************************************************************
* Function Name:        CTempFilesScanner::algoStep1()
* Function Purpose:     
*                       
*                       
*
* Arguments:            None
* Return value:         None
*************************************************************************/
void CTempFilesScanner::algoStep1()
{
	DWORD dwDrives = 0;
	std::list<std::string> lstExtMask;
	std::list<std::string> lstDrives;

	if( m_pProfiler )
	{
		m_pProfiler->GetDWORD( DRIVES_MASK, dwDrives );
		m_pProfiler->GetStringList( EXT_MASK, lstExtMask );
	}

	GetDrivesArray( dwDrives, lstDrives );

	if( lstDrives.empty() || lstExtMask.empty() )
	{
		StopJustMe();
		return;
	}

	for( std::list<std::string>::const_iterator it = lstDrives.begin(), itEnd = lstDrives.end(); it != itEnd; ++it )
	{
		DriveTraverse( *it, lstExtMask );
	}

	StopJustMe();
}

/*************************************************************************
* Function Name:        CTempFilesScanner::DriveTraverse()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CTempFilesScanner::DriveTraverse( std::string strDrive, const std::list<std::string> & lstExtMask )
{
	if( S_OK != IsPathExist( strDrive.c_str() ) )
		return;

	RecursivelyScanByMask( strDrive, lstExtMask );
}
