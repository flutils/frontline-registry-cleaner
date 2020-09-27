#include "StdAfx.h"
#include "FileAssocRscanner.h"
#include "ComObjectRScanner.h"

static const TCHAR * PersistentHandler      = _T( "PersistentHandler" );
static const TCHAR * MicrosoftExplorerExt   = _T( "Software\\Microsoft\\"
    "Windows\\CurrentVersion\\Explorer\\FileExts" );

/*************************************************************************
* Function Name:        CFileAssocRScanner::CFileAssocRScanner
* Function Purpose:     Constructor
*************************************************************************/
CFileAssocRScanner::CFileAssocRScanner( CCleanEngine * pEngine, INotifier * pNotifier /* = NULL */, IScanProfiler * pProfiler /* = NULL */ ) :
	CRegistryScanner( pEngine, pNotifier, pProfiler ),
	CCleanScanner( pEngine, pNotifier, pProfiler ),
    m_pfnAlgoSelector( NULL ),
    m_dwIndex( 0 ),
    m_hScanningKey( NULL )
{
}

/*************************************************************************
* Function Name:          CFileAssocRScanner::~CFileAssocRScanner
* Function Purpose:       Destructor
*************************************************************************/
CFileAssocRScanner::~CFileAssocRScanner( void )
{
}

/*************************************************************************
* Function Name:          CFileAssocRScanner::ReinitAlgorithm
* Function Purpose:       
* Function Description: 
*************************************************************************/
void CFileAssocRScanner::ReinitAlgorithm( void )
{
    m_pfnAlgoSelector = &CFileAssocRScanner::algoStep1;
    m_dwIndex = 0;
}

/*************************************************************************
* Function Name:          CFileAssocRScanner::ReinitAlgorithm
* Function Purpose:       
* Function Description: 
*************************************************************************/
void CFileAssocRScanner::algorithm( void )
{
    ( this->*m_pfnAlgoSelector )();
}

/*************************************************************************
* Function Name:    CFileAssocRScanner::postop()
*
* Function Purpose: Called by CRunnable after run() function completed. 
*                   Perform correctly thread completion (close opened 
*                   handles etc.)
*
* Arguments:        None
* Return value:     None
*                           
*************************************************************************/
void CFileAssocRScanner::postop( void )
{
    CRegistryScanner::postop();

    if( m_hScanningKey )
    {
        RegCloseKey( m_hScanningKey );
        m_hScanningKey = NULL;
    }
}

/*************************************************************************
* Function Name:            CFileAssocRScanner::CheckProgId
* Function Purpose: 
* Function Description:
* Arguments:        
* Return Value:
*************************************************************************/
CHECKITEM_RESULT CFileAssocRScanner::CheckProgId( HKEY hKey )
{
    _ASSERT( hKey );


	LOG_TEXT(_T("Start prog ID checking..."))

    DWORD dwDataSize = 0;
    DWORD dwType;
    LONG lResult;
    countable< TCHAR > spProgId( NULL );
	CCleanScanner * pScanner = NULL;
	CComObjectRScanner * pComScanner = NULL;
    
    lResult = RegReadValue(
                        hKey,
                        NULL,
                        &dwType,
                        spProgId,
                        dwDataSize );

    if( ERROR_SUCCESS != lResult )
    {
        if( ERROR_FILE_NOT_FOUND == lResult )
        {
			LOG_TEXT(_T("Default value not found!"))
            return ITEM_NOT_EXIST;
        }

        return CHECK_ERROR;
    }

    if( _tcscmp(_T( "" ), spProgId.get()) == 0 )
    {
		LOG_TEXT(_T("Default value is empty!"))
        return ITEM_EMPTY;
    }

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

    if( IsKeyExist( HKEY_CLASSES_ROOT, spProgId.get()) && ( (pComScanner == NULL) ? TRUE : !pComScanner->ProgIDIsInReport( spProgId.get(), _T( "HKEY_CLASSES_ROOT\\" ) ) ) ) 
    {
		LOG_TEXT3(_T("Key "),spProgId.get(),_T(" exist."))
        return ITEM_OK;
    }
	else
		LOG_TEXT3(_T("Key "),spProgId.get(),_T(" is not exist."))

    // Default value can be CLSID (Win9x).
    //

	LOG_TEXT(_T("Checking CLSID..."))

    if( _T('{') == spProgId.get()[0] )
    {
        ce_tstring strKeyName(_T( "CLSID\\" ));
        strKeyName += spProgId.get();

        if( IsKeyExist( HKEY_CLASSES_ROOT, const_cast<const LPTSTR>( strKeyName.GetString() ) ) && ( (pComScanner == NULL) ? TRUE : !pComScanner->CLSIDIsInReport( spProgId.get(), _T( "HKEY_CLASSES_ROOT\\" ) ) ) )
        {
			LOG_TEXT3(_T("Key "),const_cast<const LPTSTR>(strKeyName.GetString()),_T(" is exist!"))
            return ITEM_OK;
        }
		else
			LOG_TEXT3(_T("Key "),const_cast<const LPTSTR>(strKeyName.GetString()),_T(" is not exist!"))
    }

    // Do not delete keys that have NoOpen value
    //
    /*
    LONG lResult;
    DWORD dwType;
    DWORD dwDataSize;
    
    countable< TCHAR > spDataBuffer( NULL );
    
    lResult = RegReadValue( hKey, 
                            _T( "NoOpen" ), 
                            dwType, 
                            spDataBuffer, 
                            &dwDataSize );
    
    if( STATUS_SUCCESS == lResult )
    {
        return ITEM_OK;
    }
    */

	lResult = RegReadValue(
		hKey,
		_T( "PerceivedType" ),
		&dwType,
		spProgId,
		dwDataSize );

	if ( ERROR_SUCCESS == lResult)
	{
		if (_tcscmp(_T( "system" ), spProgId.get()) == 0)
		{
			return ITEM_OK;
		}
	}

    return ITEM_INCORRECT;
}

/*************************************************************************
* Function Name:          CFileAssocRScanner::CheckPersistentHandler()
*
* Function Purpose:       Verify does key contain 'PersistentHandler' 
*                         sub-key and is it valid or not.
*
* Function Description:   
* Arguments:              hKey  - handle to an open key.
*
* Return value:           ITEM_OK if 'PersistentHandler' key exist 
*                         and valid, or some error code defined in 
*                         CRegistryScanner::CHECK_RESULT enum.
*
*************************************************************************/
CHECKITEM_RESULT CFileAssocRScanner::CheckPersistentHandler( HKEY hKey )
{
    _ASSERT( hKey );

	LOG_TEXT(_T("CheckPersistentHandler...."))
		
    ce_tstring strCheckString = PersistentHandler;

	if( !IsKeyExist( hKey, const_cast<const LPTSTR>( strCheckString.GetString() ) ) )
    {
		LOG_TEXT3(_T("Key "), const_cast<const LPTSTR>( strCheckString.GetString()),_T(" is not exist!"))

        return ITEM_NOT_EXIST;
    }

    HKEY hOpenKey;
    LONG lResult = RegOpenKeyEx(
                            hKey,
                            PersistentHandler,
                            0,
                            KEY_READ|KEY_WRITE| DELETE,
                            &hOpenKey );
    
    if( ERROR_SUCCESS != lResult )
    {
		LOG_TEXT3(_T("Key "), const_cast<const LPTSTR>( strCheckString.GetString()),_T(" is not exist!"))
		return CHECK_ERROR;
    }

    DWORD dwType = REG_SZ;
    DWORD dwDataSize = 0;
    countable< TCHAR > spDataBuffer( NULL );

    lResult = RegReadValue( hOpenKey,
                            NULL,
                            &dwType,
                            spDataBuffer,
                            dwDataSize );

    RegCloseKey( hOpenKey );

    if( ERROR_SUCCESS != lResult )
    {
        return ITEM_INCORRECT;	//CHECK_ERROR;
    }
    
    strCheckString = _T( "CLSID\\" );
    strCheckString += spDataBuffer.get();

	if( !IsKeyExist( HKEY_CLASSES_ROOT, const_cast<const LPTSTR>( strCheckString.GetString() ) ) )
    {
		LOG_TEXT3(_T("Key "), const_cast<const LPTSTR>( strCheckString.GetString()),_T(" is not exist!"))
        return ITEM_INCORRECT;
    }

	LOG_TEXT3(_T("Key "), const_cast<const LPTSTR>( strCheckString.GetString()),_T(" is good!"))

    return ITEM_OK;
}

/*************************************************************************
* Function Name:          CFileAssocRScanner::CheckContentType()
*
* Function Purpose:       Verify does key contain 'Content Type' value.
*
* Function Description:   
* Arguments:              hKey  - handle to an open key.
*
* Return value:           ITEM_OK if 'Content Type' value found,
*                         or ITEM_NOT_EXIST otherwise.
*
*************************************************************************/
CHECKITEM_RESULT CFileAssocRScanner::CheckContentType( HKEY hKey )
{
	LOG_TEXT(_T("CheckContentType...."))

    CHECKITEM_RESULT chkResult = ITEM_OK;
    if( !IsValueExist( hKey, _T( "Content Type" )) )
    {
        chkResult = ITEM_NOT_EXIST;
		LOG_TEXT(_T("Key \"Content Type\" is not exist!"))
    }
	else
	  LOG_TEXT(_T("Key \"Content Type\" is good!"))
    return chkResult;
}

/*************************************************************************
* Function Name:          CFileAssocRScanner::CheckRegisteredType
*
* Function Purpose:       Check one separate registry key from HKCR that 
*                         describes registered file type, add invalid 
*                         keys to item list.
*
* Function Description:   
* Arguments:              hKey  - handle to an open key.
*
*                         lpValueName - pointer to zero-terminated key 
*                         name that is need to be checked.
*
* Return value:           None
* Revision History:
*      comments:          created
*                           
*************************************************************************/
void CFileAssocRScanner::CheckRegisteredType(
    HKEY        hKey, 
    LPTSTR      lpKeyName,
    LPTSTR      lpSubkeyName )
{
    _ASSERT( hKey );

	LOG_TEXT3(_T("Checking registered type "),lpKeyName,lpSubkeyName)

    HKEY hOpenKey;
    LONG lResult = RegOpenKeyEx(
                            hKey, 
                            lpSubkeyName, 
                            0, 
                            KEY_READ|KEY_WRITE| DELETE, 
                            &hOpenKey );

    if( ERROR_SUCCESS != lResult )
    {
        return;
    }

    switch( CheckProgId( hOpenKey ) )
    {
    case ITEM_INCORRECT:
        {
            ce_tstring strFullName( lpKeyName );
            strFullName += lpSubkeyName;
			LOG_TEXT3(_T("Key "),strFullName,_T(" is TREAT!"))
            RecursivelyAddKey( hOpenKey, strFullName );
            break;
        }

    case ITEM_OK:
        {
            // TODO: check paths recursively
            //
			LOG_TEXT3(lpKeyName,lpSubkeyName,_T(" is OK!"))
            break;
        }

    case ITEM_EMPTY:
    case ITEM_NOT_EXIST:
        {
            // Check Persistent Handler, Content Type 
            // TODO: check NoOpen, paths ?
            //

			LOG_TEXT(_T("Checking Persistent Handler, Content Type"))

            CHECKITEM_RESULT chkPersistHandler;
            CHECKITEM_RESULT chkContentType;

            chkPersistHandler = CheckPersistentHandler( hOpenKey );
            chkContentType = CheckContentType( hOpenKey );
            
            if(((ITEM_INCORRECT == chkPersistHandler ) ||
                (ITEM_NOT_EXIST == chkPersistHandler ))
             
             && (ITEM_NOT_EXIST == chkContentType ))
            {
                CHECKITEM_RESULT chkOWL;
				CHECKITEM_RESULT chkOWP;

				chkOWL = CheckOpenWithList1( hOpenKey );
				chkOWP = CheckOpenWithProgids1( hOpenKey );

				if ( ( ITEM_INCORRECT == chkOWL ) && ( ITEM_INCORRECT == chkOWP ) )
				{
					ce_tstring strFullName( lpKeyName );
					strFullName += lpSubkeyName;
					LOG_TEXT2(strFullName, _T(" is TREAT!"))
					RecursivelyAddKey( hOpenKey, strFullName );
					break;
				}
            }

            if( ITEM_INCORRECT == chkPersistHandler )
            {
                ce_tstring strFullName( lpKeyName );
                strFullName += lpSubkeyName;
                strFullName += _T( "\\" );
                strFullName += PersistentHandler;
            }

            break;
        }

    case CHECK_ERROR:
        {
            break;
        }

    // We should never reach this code.
    //
    default:
        EXCEPTION(_T( "Function return unhandled value" ));
        break;
    }

    RegCloseKey( hOpenKey );
}

/*************************************************************************
* Function Name:            CFileAssocRScanner::algoStep1
* Function Purpose:         Check registered file types in HKCR
* Function Description:     Examines entries under HKCR starting with '.'
* 
* Arguments:                None
*
* Return value:             None
* Revision History:
*      comments:            Created
*                           
*************************************************************************/
void CFileAssocRScanner::algoStep1( void )
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
        if( _T( '.' ) == szKeyName[ 0 ] )
        {
            // Notify user about current scanning object
            //
            if( m_pNotifier )
            {
                m_pNotifier->CallOnScan( ScannerId(), _T( "HKEY_CLASSES_ROOT\\" ), szKeyName );
            }

            CheckRegisteredType( HKEY_CLASSES_ROOT, _T( "HKEY_CLASSES_ROOT\\" ), szKeyName );
        }
		LOG_TEXT(_T("----------------------------------------------------------------------------"))

        return;
    }

    // Enumeration completed or error occurred. Switch to step 2.
    //
    m_pfnAlgoSelector = &CFileAssocRScanner::algoStep2;
}

/*************************************************************************
* Function Name:            CFileAssocRScanner::algoStep2()
* Function Purpose:         Open  HKEY_CURRENT_USER\Software\Microsoft\
*                           Windows\CurrentVersion\Explorer\FileExts
*                           registry key for scanning.
* Function Description:     
* 
* Arguments:                None
* Return value:             None
*                           
*************************************************************************/
void CFileAssocRScanner::algoStep2( void )
{
    LONG lResult = RegOpenKeyEx( 
                            HKEY_CURRENT_USER, 
                            MicrosoftExplorerExt,
                            0, 
                            KEY_READ|KEY_WRITE| DELETE,
                            &m_hScanningKey);

    if( ERROR_SUCCESS != lResult )
    {
        // Can't open required key, stop scanning
        //

        StopJustMe();   
        return;
    }

    m_pfnAlgoSelector = &CFileAssocRScanner::algoStep3;
    m_dwIndex = 0;
}

/*************************************************************************
* Function Name:            CFileAssocRScanner::algoStep3
* Function Purpose:         Check registered file types in 
*                           HKEY_CURRENT_USER\Software\Microsoft\
*                            Windows\CurrentVersion\Explorer\FileExts
*
* Function Description:     All sub keys that have no associated ProgIDs 
*                           and empty OpenWithProgids or OpenWithList
*                           child keys must be marked as unused file 
*                           extension and deleted.
* 
* Arguments:                None
*
* Return value:             None
*                           
*************************************************************************/
void CFileAssocRScanner::algoStep3( void )
{
	LOG_TEXT(_T("Checking registered file types in \"HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\""))

    TCHAR       szKeyName[ MAX_KEY_SIZE ];
    DWORD       dwSize = sizeof( szKeyName ) / sizeof( TCHAR );

    LONG lResult = RegEnumKeyEx(
                            m_hScanningKey,
                            m_dwIndex++,
                            szKeyName,
                            &dwSize,
                            NULL,
                            NULL,
                            NULL,
                            NULL );

    if( ERROR_SUCCESS == lResult )
    {
        if( _T('.') == szKeyName[ 0 ] )
        {
            // Notify user about current scanning object
            //
            if( m_pNotifier )
            {
                ce_tstring strScanObject( _T("HKEY_CURRENT_USER\\") );
                strScanObject += MicrosoftExplorerExt;

				m_pNotifier->CallOnScan( ScannerId(), strScanObject.GetString(), szKeyName );
            }

            ce_tstring strProgIds( szKeyName );
            ce_tstring strWithList( szKeyName );
            
            strProgIds  += _T( "\\OpenWithProgids" );
            strWithList += _T( "\\OpenWithList" );

			if( !IsKeyExist( m_hScanningKey, const_cast<const LPTSTR>( strProgIds.GetString() ) ) &&
				!IsKeyExist( m_hScanningKey, const_cast<const LPTSTR>( strWithList.GetString() ) ) )
            {
                HKEY hOpenKey;

				LOG_TEXT3(_T("OpenWithProgids of "),szKeyName,_T(" not exist"))
				LOG_TEXT3(_T("OpenWithList of "),szKeyName,_T(" not exist"))

                lResult = RegOpenKeyEx( m_hScanningKey, 
                                        szKeyName, 
                                        0, 
                                        KEY_READ|KEY_WRITE| DELETE,
                                        &hOpenKey );

                if( ERROR_SUCCESS == lResult )
                {
                    ce_tstring strKeyName( _T("HKEY_CURRENT_USER\\") );
					strKeyName += MicrosoftExplorerExt;
					strKeyName += _T('\\');
                    strKeyName += szKeyName;
					LOG_TEXT3(_T("OpenWithProgids or OpenWithList are not present for "), szKeyName,_T(" This is TREAT!"))
                    RecursivelyAddKey( hOpenKey, strKeyName );
                    RegCloseKey( hOpenKey );
                }
            }
			else
			{
				HKEY hOpenKey;
				lResult = RegOpenKeyEx(
					m_hScanningKey,
					szKeyName,
					0,
					KEY_READ|KEY_WRITE| DELETE,
					&hOpenKey
					);
				
				CHECKITEM_RESULT chkOWL;
				CHECKITEM_RESULT chkOWP;

				chkOWL = CheckOpenWithList2( hOpenKey );
				chkOWP = CheckOpenWithProgids2( hOpenKey );

				if ( ( ITEM_INCORRECT == chkOWL ) && ( ITEM_INCORRECT == chkOWP ) )
				{
					ce_tstring strKeyName( _T("HKEY_CURRENT_USER\\") );
					strKeyName += MicrosoftExplorerExt;
					strKeyName += _T('\\');
					strKeyName += szKeyName;
					LOG_TEXT3(_T("OpenWithProgids or OpenWithList are not present for "), szKeyName,_T(" This is TREAT!"))
					RecursivelyAddKey( hOpenKey, strKeyName );
					RegCloseKey( hOpenKey );
				}
				
				LOG_TEXT(_T("OpenWithProgids or OpenWithList are present"))

			}
        }

		LOG_TEXT(_T("----------------------------------------------------------------------------"))
        return;
    }

	// Any error occurred or enum completed.
    //
	//RegCloseKey( m_hScanningKey );
    StopJustMe();
}

CHECKITEM_RESULT CFileAssocRScanner::CheckOpenWithList1( HKEY hKey )
{
	DWORD lResult = ERROR_SUCCESS;
	HKEY hOpenKey = NULL;
	TCHAR       szKeyName[ MAX_KEY_SIZE ];
	DWORD       dwSize = sizeof( szKeyName ) / sizeof( TCHAR );
	DWORD dwIndex = 0;
	bool CheckSuccess = false;

	lResult = RegOpenKeyEx(
				hKey,
				_T("OpenWithList"),
				0,
				KEY_READ|KEY_WRITE| DELETE,
				&hOpenKey
				);
	if ( ERROR_SUCCESS != lResult)
	{
		return ITEM_NOT_EXIST;
	}

	lResult = RegEnumKeyEx(
		hOpenKey,
		dwIndex++,
		szKeyName,
		&dwSize,
		NULL,
		NULL,
		NULL,
		NULL );

	while ( ERROR_SUCCESS == lResult )
	{
		ce_tstring strKey( szKeyName );
	
		if ( IsKeyExist( HKEY_CLASSES_ROOT, const_cast<const LPTSTR>( strKey.GetString() ) ) )
		{
			CheckSuccess = true;
		}

		lResult = RegEnumKeyEx(
			hOpenKey,
			dwIndex++,
			szKeyName,
			&dwSize,
			NULL,
			NULL,
			NULL,
			NULL );
	}

	RegCloseKey( hOpenKey );

	if ( CheckSuccess )
	{
		return ITEM_OK;
	}
	else
	{
		return ITEM_INCORRECT;
	}
}

CHECKITEM_RESULT CFileAssocRScanner::CheckOpenWithProgids1( HKEY hKey )
{
	DWORD lResult = ERROR_SUCCESS;
	HKEY hOpenKey = NULL;
	TCHAR       szKeyName[ MAX_KEY_SIZE ];
	DWORD       dwSize = sizeof( szKeyName ) / sizeof( TCHAR );
	DWORD dwIndex = 0;
	bool CheckSuccess = false;

	lResult = RegOpenKeyEx(
		hKey,
		_T("OpenWithProgids"),
		0,
		KEY_READ|KEY_WRITE| DELETE,
		&hOpenKey
		);
	if ( ERROR_SUCCESS != lResult)
	{
		return ITEM_NOT_EXIST;
	}

	lResult = RegEnumKeyEx(
		hOpenKey,
		dwIndex++,
		szKeyName,
		&dwSize,
		NULL,
		NULL,
		NULL,
		NULL );

	while ( ERROR_SUCCESS == lResult )
	{
		ce_tstring strKey( szKeyName );

		if ( IsKeyExist( HKEY_CLASSES_ROOT, const_cast<const LPTSTR>( strKey.GetString() ) ) )
		{
			CheckSuccess = true;
		}

		lResult = RegEnumKeyEx(
			hOpenKey,
			dwIndex++,
			szKeyName,
			&dwSize,
			NULL,
			NULL,
			NULL,
			NULL );
	}

	RegCloseKey( hOpenKey );

	if ( CheckSuccess )
	{
		return ITEM_OK;
	}
	else
	{
		return ITEM_INCORRECT;
	}
}

CHECKITEM_RESULT CFileAssocRScanner::CheckOpenWithList2( HKEY hKey )
{
	DWORD lResult = ERROR_SUCCESS;
	HKEY hOpenKey = NULL;
	TCHAR       szKeyName[ MAX_KEY_SIZE ];
	DWORD       dwSize = sizeof( szKeyName ) / sizeof( TCHAR );
	DWORD dwIndex = 0;
	bool CheckSuccess = false;

	lResult = RegOpenKeyEx(
		hKey,
		_T("OpenWithList"),
		0,
		KEY_READ|KEY_WRITE| DELETE,
		&hOpenKey
		);
	if ( ERROR_SUCCESS != lResult)
	{
		return ITEM_NOT_EXIST;
	}

	lResult = RegEnumValue(
		hOpenKey,
		dwIndex++,
		szKeyName,
		&dwSize,
		NULL,
		NULL,
		NULL,
		NULL );

	while ( ERROR_SUCCESS == lResult )
	{
		countable< TCHAR > spDataBuffer;
		DWORD dwType, dwDataSize = 0;

		RegReadValue(
			hOpenKey,
			szKeyName,
			&dwType,
			spDataBuffer,
			dwDataSize);

		if ( CheckPaths( spDataBuffer ) )
		{
			CheckSuccess = true;
		}

		lResult = RegEnumValue(
			hOpenKey,
			dwIndex++,
			szKeyName,
			&dwSize,
			NULL,
			NULL,
			NULL,
			NULL );
	}

	RegCloseKey( hOpenKey );

	if ( CheckSuccess )
	{
		return ITEM_OK;
	}
	else
	{
		return ITEM_INCORRECT;
	}
}

CHECKITEM_RESULT CFileAssocRScanner::CheckOpenWithProgids2( HKEY hKey )
{
	DWORD lResult = ERROR_SUCCESS;
	HKEY hOpenKey = NULL;
	TCHAR       szKeyName[ MAX_KEY_SIZE ];
	DWORD       dwSize = sizeof( szKeyName ) / sizeof( TCHAR );
	DWORD dwIndex = 0;
	bool CheckSuccess = false;

	lResult = RegOpenKeyEx(
		hKey,
		_T("OpenWithProgids"),
		0,
		KEY_READ|KEY_WRITE| DELETE,
		&hOpenKey
		);
	if ( ERROR_SUCCESS != lResult)
	{
		return ITEM_NOT_EXIST;
	}

	lResult = RegEnumValue(
		hOpenKey,
		dwIndex++,
		szKeyName,
		&dwSize,
		NULL,
		NULL,
		NULL,
		NULL );

	while ( ERROR_SUCCESS == lResult )
	{
		
		ce_tstring strKey( szKeyName );

		if ( IsKeyExist( HKEY_CLASSES_ROOT, const_cast<const LPTSTR>( strKey.GetString() ) ) )
		{
			CheckSuccess = true;
		}

		lResult = RegEnumValue(
			hOpenKey,
			dwIndex++,
			szKeyName,
			&dwSize,
			NULL,
			NULL,
			NULL,
			NULL );
	}

	RegCloseKey( hOpenKey );

	if ( CheckSuccess )
	{
		return ITEM_OK;
	}
	else
	{
		return ITEM_INCORRECT;
	}
}