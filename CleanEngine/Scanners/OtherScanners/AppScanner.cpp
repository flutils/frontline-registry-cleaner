#include "StdAfx.h"
#include "AppScanner.h"
#include "CleanEngine.h"
#include <AtlPath.h>

/*************************************************************************
* Function Name:          CAppScanner::CAppScanner()
* Function Description:   Constructor
*************************************************************************/
CAppScanner::CAppScanner( CCleanEngine * pEngine, INotifier * pNotifier /* = NULL */, IScanProfiler * pProfiler /* = NULL */ ) :
    CFileScanner( pEngine, pNotifier, pProfiler ),
	CRegistryScanner( pEngine, pNotifier, pProfiler ),
	CCleanScanner( pEngine, pNotifier, pProfiler ),
    m_pfnAlgoSelector( NULL ),
	m_mapActionRef( SAppBase::Instance()->m_mapAction )
{
}

/*************************************************************************
* Function Name:          CAppScanner::~CAppScanner()
* Function Description:   Destructor
*************************************************************************/
CAppScanner::~CAppScanner()
{
}

/*************************************************************************
* Function Name:          CAppScanner::prerun()
* Function Description:   
*************************************************************************/
void CAppScanner::prerun()
{
	m_arrAppInfoList.RemoveAll();
	SAppBase::Instance()->GetAppList( m_arrAppInfoList );

	CCleanScanner::prerun();
}

/*************************************************************************
* Function Name:        CAppScanner::ReinitAlgorithm()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CAppScanner::ReinitAlgorithm()
{
    m_pfnAlgoSelector = &CAppScanner::algoStep1;
}

/*************************************************************************
* Function Name:        CAppScanner::algorithm()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CAppScanner::algorithm()
{
    ( this->*m_pfnAlgoSelector )();
}

/*************************************************************************
* Function Name:        CAppScanner::algoStep1()
* Function Purpose:     
*                       
*                       
*
* Arguments:            None
* Return value:         None
*************************************************************************/
void CAppScanner::algoStep1( void )
{
	size_t i, iMax = m_arrAppInfoList.GetCount();

	for( i = 0; i < iMax; ++i )
	{
		SAppBase::SAction action = m_mapActionRef[ m_arrAppInfoList.GetAt( i ).m_guid ];

		if( m_pNotifier )
			m_pNotifier->OnScan( ScannerId(), action.strName, NULL );

		size_t j, jMax = action.vThreatItems.size();

		for( j = 0; j < jMax; ++j )
		{
			switch( action.vThreatItems[j].second )
			{
				case IT_DIRECTORY:
					if( S_OK == IsPathExist( action.vThreatItems[j].first ) )
					{
						RecursivelyAddDirectory( action.vThreatItems[j].first, true );
					}
				break;

				case IT_FILE:
					if( S_OK == IsPathExist( action.vThreatItems[j].first ) )
					{
						CPath path( action.vThreatItems[j].first );

						int iPos = path.FindFileName();
						CString strDir = path.m_strPath.Left( iPos );
						CString strFile = path.m_strPath.Right( path.m_strPath.GetLength() - iPos );

						m_pEngine->AddItem(
							countable<IReportItem>(
								new SFileReportItem(
									fn::bind1st1( fn::mem_fun( &CAppScanner::CleanupObject ), this ),
									ScannerId(),
									IT_FILE,
									SEVERITY_MEDIUM,
									STATUS_FOUND,
									strFile,
									strDir ) ) );
					}
				break;

				case IT_REG_KEY:
				{
					HKEY hKey, hHive;
					LPCTSTR lpKey;
					if( SAppBase::Instance()->GetKeyFromSadsDataFormat( action.vThreatItems[j].first, hHive, lpKey ) )
						if( ERROR_SUCCESS == RegOpenKeyEx( hHive, lpKey, 0, KEY_READ | KEY_WRITE | DELETE, &hKey ) )
						{
							CString strHive = GetHKeyR( hHive );
							RegCloseKey( hKey );
							m_pEngine->AddItem(
								countable<IReportItem>(
									new SRegKReportItem(
										fn::bind1st1( fn::mem_fun( &CAppScanner::CleanupObject ), this ),
										ScannerId(),
										IT_REG_KEY,
										SEVERITY_MEDIUM,
										STATUS_FOUND,
										strHive + lpKey ) ) );
						}
				}
				break;

				case IT_REG_VALUE:
				{
					HKEY hKey, hHive;
					CString szKey, szValue;
					if( SAppBase::Instance()->GetRegValueFromSadsDataFormat( action.vThreatItems[j].first, hHive, szKey, szValue ) )
						if( ERROR_SUCCESS == RegOpenKeyEx( hHive, szKey.GetString(), 0, KEY_READ | KEY_WRITE | DELETE, &hKey ) )
						{
							DWORD dwType;
							if( ERROR_SUCCESS == RegQueryValueEx( hKey, szValue.GetString(), 0, &dwType, 0, 0 ) )
							{
								CString strHive = GetHKeyR( hHive );
								m_pEngine->AddItem(
									countable<IReportItem>(
										CREATE_REG_ITEM(
											CAppScanner::CleanupObject,
											ScannerId(),
											SEVERITY_MEDIUM,
											strHive + szKey,
											szValue,
											dwType ) ) );
							}
							RegCloseKey( hKey );
						}
				}
				break;

				default:
					ATLASSERT( FALSE );
			}
		}

		if( ABANDONNED == InterrogateMngr() )
		{
			break;
		}
	}

	StopJustMe();
}

/*************************************************************************
* Function Name:        CAppScanner::CleanupObject()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CAppScanner::CleanupObject( countable< IReportItem > item )
{
	if( item->m_Type == IT_FILE || item->m_Type == IT_DIRECTORY )
		CFileScanner::CleanupObject( item );
	else if( item->m_Type == IT_REG_KEY || item->m_Type == IT_REG_VALUE )
		CRegistryScanner::CleanupObject( item );
	else
		ATLASSERT( FALSE );
}
