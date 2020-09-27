#include "StdAfx.h"
#include ".\\RegistryScanner.h"
#include "RegistryScanner.h"
#include <atlpath.h>
#include "CleanEngine.h"
#include "helpers.h"

CRegistryScanner::CRegistryScanner( CCleanEngine * pEngine, INotifier * pNotifier /* = NULL */, IScanProfiler * pProfiler /* = NULL */ ) :
	CCleanScanner( pEngine, pNotifier, pProfiler )
{
}

/*************************************************************************
* Function Name:          CRegistryScanner::CleanupObject()
* Function Purpose:       Erase item in list and all sub-items by it Id.
*
* Arguments:              itemId - Identifier of item that is need 
*                         to be erased.
*
* Return value:           None.
*************************************************************************/
void CRegistryScanner::CleanupObject( countable< IReportItem > item )
{
	switch( item->m_Type )
	{
	case IT_REG_VALUE:
		{
			SRegVReportItem * pItem;
			pItem = item->GetAsRegVItem();

			if( false == IsValueExist(
				const_cast<const LPTSTR>( pItem->m_strKeyPath.GetString() ),
				const_cast<const LPTSTR>( pItem->m_strSubkeyName.GetString() ) ) )
			{
				item->m_Status = STATUS_CLEANED;
			}
			else
			{
				S_OK == DeleteRegValue( pItem->m_strKeyPath + '|' + pItem->m_strSubkeyName ) ?
					item->m_Status = STATUS_CLEANED:
					item->m_Status = STATUS_CLEAN_FAILED;
			}
		} break;

	case IT_REG_KEY:
		{
			SRegKReportItem * pItem;
			pItem = item->GetAsRegKItem();

			if( false == IsKeyExist( const_cast<const LPTSTR>( pItem->m_strKeyPath.GetString() ) ) )
			{
				item->m_Status = STATUS_CLEANED;
			}
			else
			{
				S_OK == DeleteRegKey( pItem->m_strKeyPath ) ?
					item->m_Status = STATUS_CLEANED:
					item->m_Status = STATUS_CLEAN_FAILED;
			}
		} break;

	default:

		// Not RegistryScanner threat
		ATLASSERT( FALSE );
	}

	if( m_pNotifier )
		m_pNotifier->OnItemFix( item );
}

/*************************************************************************
* Function Name:          CRegistryScanner::RecursivelyAddKey()
*
* Function Description:   Recursively add registry key and all it 
*                         sub-keys and values to scan result list. 
*
* Arguments:              hKey - Handle to an open key. The key must have 
*                         been opened with KEY_ENUMERATE_SUB_KEYS access 
*                         rights.
*
*                         rstrFullKeyName - Full path to opened key.
*                         nParentId - Id of parent item in result list.
*                         nDepth - Recurse level.
*
* Return value:           None
*************************************************************************/
void CRegistryScanner::RecursivelyAddKey(
	const HKEY          hKey,
	const ce_tstring&   rstrFullKeyName,
	const SEVERITY		severity,
	const unsigned      nDepth,
	bool bEmtyKey)
{
	_ASSERT( hKey );

	if( RegKeysWhiteList->IsKeyWhite( rstrFullKeyName ) )
		return;

	if( nDepth > MaxRecurseLevel )
	{
		return;
	}

	if( ABANDONNED == InterrogateMngr() )
	{
		return;
	}
	if(bEmtyKey)
	{
		//::MessageBox(0, rstrFullKeyName, _T("Empty KEy comes"), 0);
	}
	if( m_pNotifier )
	{
		m_pNotifier->CallOnScan( (bEmtyKey == true) ? EMPTY_KEYS_SCANNER_ID : ScannerId() , rstrFullKeyName );
	}

	// Add key to list
	//
	m_pEngine->AddItem(
		countable<IReportItem>(
			new SRegKReportItem(
				fn::bind1st1( fn::mem_fun( &CRegistryScanner::CleanupObject ), this ),
				(bEmtyKey == true) ? EMPTY_KEYS_SCANNER_ID : ScannerId(),
				IT_REG_KEY,
				severity,
				STATUS_FOUND,
				rstrFullKeyName
				)
				),
				!bEmtyKey
	);

	// Enumerate values
	//
	TCHAR szValueName[ MAX_VALUE_SIZE ];
	DWORD dwBufferSize  = 0;
	DWORD dwIndex       = 0;
	DWORD dwType;
	LONG lResult;
	countable<BYTE> pData;

	while( 1 )
	{
		dwBufferSize = MAX_VALUE_SIZE / sizeof( TCHAR );

		lResult = RegEnumValue( hKey,
								dwIndex++,
								szValueName,
								&dwBufferSize,
								NULL,
								&dwType,
								NULL,
								NULL );

		if( ERROR_SUCCESS != lResult )
		{
			if( lResult != 259L )
			{
				int q = 0;
			}

			break;
		}

		if( ABANDONNED == InterrogateMngr() )
		{
			break;
		}

		if( m_pNotifier )
		{
			m_pNotifier->CallOnScan( ScannerId(), rstrFullKeyName, szValueName );
		}

		if(_tcscmp(_T( "" ), szValueName) == 0 )
		{
			continue;
		}

		// Add value to list
		//
		m_pEngine->AddItem(
			countable<IReportItem>(
				CREATE_REG_ITEM(
					CRegistryScanner::CleanupObject,
					ScannerId(),
					severity,
					rstrFullKeyName,
					szValueName,
					dwType
				)
			)
		);
	}

	// Enumerate sub-keys
	//
	TCHAR szKeyName[ MAX_KEY_SIZE ];

	dwIndex = 0;
	lResult = ERROR_SUCCESS;

	while( 1 )
	{
		dwBufferSize = sizeof( szKeyName ) / sizeof( TCHAR );

		lResult = RegEnumKeyEx( hKey, 
								dwIndex ++, 
								szKeyName, 
								&dwBufferSize, 
								NULL, 
								NULL, 
								NULL, 
								NULL );

		if( ERROR_SUCCESS != lResult)
		{
			break;
		}

		ce_tstring strSubkeyFullName( rstrFullKeyName );
		strSubkeyFullName += _T( "\\" );
		strSubkeyFullName += szKeyName;

		HKEY hNewKey;
		lResult = RegOpenKeyEx( hKey, 
								szKeyName, 
								0, 
								KEY_READ|KEY_WRITE| DELETE, 
								&hNewKey );

		if( ERROR_SUCCESS == lResult )
		{
			RecursivelyAddKey(  hNewKey, 
								strSubkeyFullName, 
								severity,
								nDepth + 1 );

			RegCloseKey( hNewKey );
		}
	}
}
//
///*************************************************************************
//*
//*
//*
//*
//*
//*
//*
//*
//*
//*************************************************************************/
//BOOL CRegistryScanner::IsFakeCLSID( const TCHAR * szCLSID )
//{
//	countable< IReportItem > item;
//	size_t ItemCount = m_pEngine->GetReportObject()->GetItemCount();
//	for( unsigned int i = 0; i < ItemCount ; i++)
//	{
//		item = m_pEngine->GetReportObject()->GetItem( i );
//		if(	COMCLASS_SCANNER_ID == item->m_ScannerID )
//		{
//			SRegKReportItem* regitem = item->GetAsRegKItem();
//
//			CString strCurrentCLSID;
//			GetCLSIDFromPath(CString(szCLSID), strCurrentCLSID);
//
//			CString strFoundCLSID;
//			GetCLSIDFromPath(CString(regitem->m_strKeyPath), strFoundCLSID);
//			
//			if ( strFoundCLSID.CompareNoCase(strCurrentCLSID) == 0 )
//			{
//				LOG_TEXT3(_T("IsFakeCLSID for "),szCLSID,_T( " was failed!"));
//				return TRUE;
//			}
//
//		}
//	}
//
//	return FALSE;
//}
//
///*************************************************************************
//*
//*
//*
//*
//*
//*
//*
//*
//*
//*************************************************************************/
//BOOL CRegistryScanner::IsFakePath( const TCHAR * szPath )
//{
//	countable< IReportItem > item;
//	size_t ItemCount = m_pEngine->GetReportObject()->GetItemCount();
//	for( unsigned int i = 0; i < ItemCount ; i++)
//	{
//		item = m_pEngine->GetReportObject()->GetItem( i );
//		if(	DOWNLOAD_SCANNER_ID == item->m_ScannerID )
//		{
//			SFileReportItem * pItem = item->GetAsFileItem();
//
//			CPath CurrentPath(szPath);
//			CPath FoundPath( pItem->m_strDirectory );
//			FoundPath.Append(pItem->m_strFile);
//
//			CString strCurrentPath, strFoundPath;
//
//			GetShortPath(CurrentPath,strCurrentPath);
//			GetShortPath(FoundPath,strFoundPath);
//			
//		
//			if ( strFoundPath == strCurrentPath)
//			{
//				LOG_TEXT3( _T("IsFakePath for "),szPath,_T( " was failed!"))
//				return TRUE;
//			}
//
//		}
//	}
//	return FALSE;
//}
