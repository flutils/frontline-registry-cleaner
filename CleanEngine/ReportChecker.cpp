#include "StdAfx.h"
#include ".\reportchecker.h"

CReportChecker::CReportChecker(CCleanEngine * pEngine):
m_pCEngine( pEngine )
{
}

CReportChecker::~CReportChecker(void)
{
}

/*
*
*
*
*/
BOOL CReportChecker::CLSIDIsInReport( const TCHAR * szCLSID )
{
	countable< IReportItem > item;
	size_t ItemCount = m_pCEngine->GetReportObject()->GetItemCount();
	for( unsigned int i = 0; i < ItemCount ; i++)
	{
		item = m_pCEngine->GetReportObject()->GetItem( i );
		if(	/*COMCLASS_SCANNER_ID/**/COMOBJECT_SCANNER_ID == item->m_ScannerID )
		{
			SRegKReportItem* regitem = item->GetAsRegKItem();

			CString strCurrentCLSID;
			GetCLSIDFromPath(CString(szCLSID), strCurrentCLSID);

			CString strFoundCLSID;
			GetCLSIDFromPath(CString(regitem->m_strKeyPath), strFoundCLSID);

			if ( strFoundCLSID.CompareNoCase(strCurrentCLSID) == 0 )
			{
				LOG_TEXT3(_T("IsFakeCLSID for "),szCLSID,_T( " was failed!"));
				return TRUE;
			}

		}
	}

	return FALSE;
}

/*
*
*
*
*/
BOOL CReportChecker::PathIsInReport( const TCHAR * szPath )
{
	countable< IReportItem > item;
	size_t ItemCount = m_pCEngine->GetReportObject()->GetItemCount();
	for( unsigned int i = 0; i < ItemCount ; i++)
	{
		item = m_pCEngine->GetReportObject()->GetItem( i );
		if(	DOWNLOAD_SCANNER_ID == item->m_ScannerID )
		{
			SFileReportItem * pItem = item->GetAsFileItem();

			CPath CurrentPath(szPath);
			CPath FoundPath( pItem->m_strDirectory );
			FoundPath.Append(pItem->m_strFile);

			CString strCurrentPath, strFoundPath;

			GetShortPath(CurrentPath,strCurrentPath);
			GetShortPath(FoundPath,strFoundPath);


			if ( strFoundPath == strCurrentPath)
			{
				LOG_TEXT3( _T("IsFakePath for "),szPath,_T( " was failed!"))
					return TRUE;
			}

		}
	}
	return FALSE;
}

/*
*
*
*
*/
BOOL CReportChecker::TypeLibIsInReport( const TCHAR * szTypeLib )
{
	countable< IReportItem > item;
	size_t ItemCount = m_pCEngine->GetReportObject()->GetItemCount();
	for( unsigned int i = 0; i < ItemCount ; i++)
	{
		item = m_pCEngine->GetReportObject()->GetItem( i );

		if ( COMOBJECT_SCANNER_ID == item->m_ScannerID )
		{
			SRegKReportItem* regitem = item->GetAsRegKItem();
			CString strCurrentItem, strFoundItem;

			GetCLSIDFromPath(CString(szTypeLib),strCurrentItem);
			GetCLSIDFromPath(CString(regitem->m_strKeyPath),strFoundItem);

			if ( strCurrentItem.CompareNoCase(strFoundItem) == 0 )
			{
				LOG_TEXT3(_T("IsFakeTypeLib for "),szTypeLib,_T( " was failed"))

					return TRUE;
			}
		}
	}

	return FALSE;
}

/*
*
*
*
*/

BOOL CReportChecker::UninstallIsInReport( const TCHAR * szUninstall)
{
	countable< IReportItem > item;
	size_t ItemCount = m_pCEngine->GetReportObject()->GetItemCount();
	for( unsigned int i = 0; i < ItemCount ; i++)
	{
		item = m_pCEngine->GetReportObject()->GetItem( i );
		if(	UNINSTALL_SCANNER_ID == item->m_ScannerID )
		{
			SRegKReportItem* regitem = item->GetAsRegKItem();
			CString str( szUninstall );

			if ( 0 == str.CompareNoCase( regitem->m_strKeyPath ) )
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}