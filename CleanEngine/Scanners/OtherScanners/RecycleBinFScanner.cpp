#include "StdAfx.h"
#include "RecycleBinFScanner.h"
#include "CleanEngine.h"

/*************************************************************************
* Function Name:          CRecycleBinFScanner::CRecycleBinFScanner()
* Function Description:   Constructor
*************************************************************************/
CRecycleBinFScanner::CRecycleBinFScanner( CCleanEngine * pEngine, INotifier * pNotifier /* = NULL */, IScanProfiler * pProfiler /* = NULL */ ) :
    CCleanScanner( pEngine, pNotifier, pProfiler ),
    m_pfnAlgoSelector( NULL )
{
}

/*************************************************************************
* Function Name:          CRecycleBinFScanner::~CRecycleBinFScanner()
* Function Description:   Destructor
*************************************************************************/
CRecycleBinFScanner::~CRecycleBinFScanner()
{
}

/*************************************************************************
* Function Name:        CRecycleBinFScanner::ReinitAlgorithm()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CRecycleBinFScanner::ReinitAlgorithm()
{
    m_pfnAlgoSelector = &CRecycleBinFScanner::algoStep1;
}

/*************************************************************************
* Function Name:        CRecycleBinFScanner::algorithm()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CRecycleBinFScanner::algorithm()
{
    ( this->*m_pfnAlgoSelector )();
}

/*************************************************************************
* Function Name:        CRecycleBinFScanner::algoStep1()
* Function Purpose:     Get items count in resycle bins in every disk.
*                       Result is saved in the following format: 
*                           drive:\numbrer_of_items.
*
* Arguments:            None
* Return value:         None
*************************************************************************/
void CRecycleBinFScanner::algoStep1()
{
    DWORD dwBufferLength = GetLogicalDriveStrings( 0, NULL );

    if( !dwBufferLength ) 
    {
        StopJustMe();
        return;
    }

    TCHAR *szDriveBuffers = new TCHAR[ dwBufferLength + 1 ];

    if( !szDriveBuffers )
    {
        StopJustMe();
        return;
    }

    if( !GetLogicalDriveStrings( dwBufferLength, szDriveBuffers ) )
    {
        delete [] szDriveBuffers;
        StopJustMe();
        return;
    }

    TCHAR *pLogicalDrive = szDriveBuffers;

    while( _T( '\0' ) != *pLogicalDrive )
    {
        if( DRIVE_FIXED == GetDriveType( pLogicalDrive ))
        {
            //
            // Notify user about current scanning object.
            //
            if( m_pNotifier )
            {   
                m_pNotifier->CallOnScan( ScannerId(), pLogicalDrive );
            }

            SHQUERYRBINFO qbinfo;
            
            memset( &qbinfo, 0, sizeof( SHQUERYRBINFO ));
            qbinfo.cbSize = sizeof( SHQUERYRBINFO );

            HRESULT hResult = SHQueryRecycleBin( pLogicalDrive, &qbinfo );

            if( S_OK == hResult )
            {
                if( qbinfo.i64NumItems != 0 )
                {
                    TCHAR szNumItems[ 512 ];
                    _stprintf( 
                        szNumItems, _T( " (%I64u %%s)" ), qbinfo.i64NumItems );

					m_pEngine->AddItem(
						countable<IReportItem>(
							new SCustomReportItem(
								fn::bind1st1( fn::mem_fun( &CRecycleBinFScanner::CleanupObject ), this ),
								ScannerId(),
								IT_OTHER,
								SEVERITY_MEDIUM,
								STATUS_FOUND,
								pLogicalDrive ) ) );
                }
            }
        }

        pLogicalDrive += _tcslen( pLogicalDrive ) + 1;
    }

    delete [] szDriveBuffers;
    StopJustMe();
}

/*************************************************************************
* Function Name:        CRecycleBinFScanner::CleanupObject()
* Function Purpose:     Empty recycle bins.
* Arguments:            None
* Return value:         None
*************************************************************************/
void CRecycleBinFScanner::CleanupObject( countable< IReportItem > item )
{
	item->m_Status = STATUS_CLEANING;
	ON_FIX( item );

	SCustomReportItem * pItem = item.get()->GetAsCustomItem();

	if( NULL == pItem )
    {
		item->m_Status = STATUS_CLEAN_FAILED;
		ON_FIX( item );
		return;
    }

	HRESULT hRes = SHEmptyRecycleBin(
			NULL, pItem->m_strCustomDescription,
			SHERB_NOCONFIRMATION |
			SHERB_NOPROGRESSUI |
			SHERB_NOSOUND );

	item->m_Status = ( S_OK == hRes ) ? STATUS_CLEANED : STATUS_CLEAN_FAILED;

	ON_FIX( item );
}
