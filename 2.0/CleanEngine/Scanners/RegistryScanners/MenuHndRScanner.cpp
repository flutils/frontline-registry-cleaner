#include "StdAfx.h"
#include "MenuHndRScanner.h"
#include "ComObjectRScanner.h"

/*************************************************************************
* Function Name:        CMenuHndRScanner::CMenuHndRScanner()
* Function Purpose:     Constructor  
*************************************************************************/
CMenuHndRScanner::CMenuHndRScanner( CCleanEngine * pEngine, INotifier * pNotifier /* = NULL */, IScanProfiler * pProfiler /* = NULL */ ) :
	CRegistryScanner( pEngine, pNotifier, pProfiler ),
	CCleanScanner( pEngine, pNotifier, pProfiler ),
    m_pfnAlgoSelector( NULL ),
    m_dwIndex( 0 )
{
}

/*************************************************************************
* Function Name:        CMenuHndRScanner::~CMenuHndRScanner()
* Function Purpose:     Destructor
*************************************************************************/
CMenuHndRScanner::~CMenuHndRScanner( void )
{
}

/*************************************************************************
* Function Name:        CMenuHndRScanner::ReinitAlgorithm()
* Function Purpose:       
* Function Description: 
*************************************************************************/
void CMenuHndRScanner::ReinitAlgorithm( void )
{
    m_pfnAlgoSelector = &CMenuHndRScanner::algoStep1;
    m_dwIndex = 0;
}

/*************************************************************************
* Function Name:        CMenuHndRScanner::algorithm()
* Function Purpose:       
* Function Description: 
*************************************************************************/
void CMenuHndRScanner::algorithm( void )
{
    ( this->*m_pfnAlgoSelector )();
}

/*************************************************************************
* Function Name:        CMenuHndRScanner::algoStep1()
*
* Function Purpose:     Searching shell object’s context menu handlers, 
*                       shortcut menu handlers etc. that has no 
*                       correspondent CLSIDs.
*
* Arguments:            None
* Return value:         None
*
*************************************************************************/
void CMenuHndRScanner::algoStep1( void )
{
    TCHAR  szKeyName[ MAX_KEY_SIZE ];
    DWORD  dwSize = sizeof( szKeyName ) / sizeof( TCHAR );

    LONG lResult = RegEnumKeyEx(
                            HKEY_CLASSES_ROOT,
                            m_dwIndex++,
                            szKeyName,
                            &dwSize,
                            NULL,
                            NULL,
                            NULL,
                            NULL );

    if( ERROR_SUCCESS == lResult )
    {
        // Notify user about current scanning object
        //
        if( m_pNotifier )
        {
            m_pNotifier->CallOnScan( ScannerId(), _T("HKEY_CLASSES_ROOT\\"), szKeyName );
        }

        CheckMenuHandler( szKeyName );
        return;
    }

    // Enumeration completed or error occurred. Stop scanning.
    //

    StopJustMe();
}

/*************************************************************************
* Function Name:        CMenuHndRScanner::CheckMenuHandler()
* Function Purpose:     Checking keys for incorrect menu handler CLSIDs.
*
* Arguments:            szKeyName - Zero-terminating string containing 
* Return value:         None
*
*************************************************************************/
void CMenuHndRScanner::CheckMenuHandler( const TCHAR * szKeyName )
{
	HKEY hOpenKey;
	LONG lResult;

	ce_tstring strShellEx( szKeyName );
	strShellEx += _T("\\");
	strShellEx += _T("ShellEx");

	lResult = RegOpenKeyEx( HKEY_CLASSES_ROOT,
							strShellEx.GetString(),
							0,
							KEY_READ|KEY_WRITE| DELETE,
							&hOpenKey );

	if( ERROR_SUCCESS != lResult )
	{
		return ;
	}

	ScanKeyForGuids( hOpenKey, strShellEx, strShellEx.GetLength() );
	RegCloseKey( hOpenKey );
}

/*************************************************************************
* Function Name:        CMenuHndRScanner::CheckMenuHandler()
* Function Purpose:     Check key for incorrect CLSIDs.
*
* Arguments:            hKey - Handle to an open key;
*                       
*                       rstrKeyName - Reference to a string containing
*                       full path to the key to be scanned.
*                       
*                       length - String size;
*                       
*                       nRecurseLevel - Function call depth counter.
*                       
* Return value:         None
*************************************************************************/
void CMenuHndRScanner::ScanKeyForGuids( 
    const HKEY              hKey,
    ce_tstring &               rstrKeyName,
    int						length,
    unsigned                nRecurseLevel )
{
    if( nRecurseLevel > MaxRecurseLevel )
    {
        return;
    }

    TCHAR szKeyName[ MAX_KEY_SIZE ];
    DWORD dwSize; 
    DWORD dwIndex = 0;

    while( 1 )
    {
        dwSize = sizeof( szKeyName ) / sizeof( TCHAR );

        LONG lResult = RegEnumKeyEx(
                                hKey,
                                dwIndex++,
                                szKeyName,
                                &dwSize,
                                NULL,
                                NULL,
                                NULL,
                                NULL );

        if( ERROR_SUCCESS != lResult )
        {
            break;
        }

        HKEY hOpenKey;

        if( ERROR_SUCCESS == RegOpenKeyEx(
                                        hKey,
                                        szKeyName,
                                        0,
                                        KEY_READ| DELETE|KEY_WRITE,
                                        &hOpenKey ) )
        {
            DWORD dwType;
            DWORD dwDataSize = 0;
            countable< TCHAR > spDataBuffer;

            // The key value has higher priority than the key name.
            //
            if( ERROR_SUCCESS == RegReadValue(
                                            hOpenKey,
                                            NULL,
                                            &dwType,
                                            spDataBuffer,
                                            dwDataSize ) )
            {
                if( IsStringGuid( spDataBuffer.get() ))
                {
                    ce_tstring strClsidKey( _T("CLSID\\") );
                    strClsidKey += spDataBuffer.get();
					CCleanScanner * pScanner = NULL;
					CComObjectRScanner * pComScanner = NULL;

					try
					{
						pScanner = m_pEngine->GetPointerToScanner( COMOBJECT_SCANNER_ID );
						if ( pScanner->IsScannerEnabled() )
						{
							pComScanner = dynamic_cast< CComObjectRScanner* >(pScanner);
						}
						else
						{
							pComScanner = NULL;
						}
					}
					catch (...)
					{
						pComScanner = NULL;
					}

					if( !IsKeyExist( HKEY_CLASSES_ROOT, const_cast<const LPTSTR>( strClsidKey.GetString() ) ) || 
						( (pComScanner == NULL) ? FALSE : pComScanner->CLSIDIsInReport( spDataBuffer.get(), _T( "HKEY_CLASSES_ROOT" ) ) ) )
                    {
                        ce_tstring strFullKeyName( _T("HKEY_CLASSES_ROOT\\") );
                        strFullKeyName += rstrKeyName;
                        strFullKeyName += _T("\\");
                        strFullKeyName += szKeyName;
                        RecursivelyAddKey( hOpenKey, strFullKeyName );
                    }
                    
                    RegCloseKey( hOpenKey );
                    continue;
                }
            }

			// Key name can be GUID also.
            //
            if( IsStringGuid( szKeyName ))
            {
                ce_tstring strClsidKey(_T( "CLSID\\" ));
                strClsidKey += szKeyName;

				if( !IsKeyExist( HKEY_CLASSES_ROOT, const_cast<const LPTSTR>( strClsidKey.GetString() ) ) )
                {
                    ce_tstring strFullKeyName( "HKEY_CLASSES_ROOT\\" );
                    strFullKeyName += rstrKeyName;
                    strFullKeyName += _T( "\\" );
                    strFullKeyName += szKeyName;
                    RecursivelyAddKey( hOpenKey, strFullKeyName );
                    RegCloseKey( hOpenKey );
                    continue;
                }
            }

            rstrKeyName += _T( "\\" );
            rstrKeyName += szKeyName;

            ScanKeyForGuids(
                        hOpenKey,
                        rstrKeyName,
						rstrKeyName.GetLength(),
                        nRecurseLevel + 1 );

            rstrKeyName.GetBufferSetLength( length );
            RegCloseKey( hOpenKey );
        }
    }
}
