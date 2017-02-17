#include "StdAfx.h"
#include "RegistryTools.h"


/*************************************************************************
* Function Name:        CRegistryTools::RegReadValue
* Function Description: The RegReadValue function retrieves the type 
*                       and data for a specified value name associated 
*                       with an open registry key.
*
* Arguments:            hKey - Handle to open key.
*
*                       lpValueName - Pointer to a null-terminated 
*                       string containing the name of the value 
*                       to query. If lpValueName is NULL or an empty z
*                       string, "", the function retrieves the type and 
*                       data for the key's unnamed or default value, 
*                       if any.
*                       
*                       lpType - [out] Pointer to a variable that 
*                       receives a code indicating the type of data stored
*                       in the specified value.
*                       
*                       rspDataBuffer - [out] Reference to smart-pointer
*                       that receives the value's data. Memory for data 
*                       buffer allocated by function and will be released
*                       automatically when container destroyed.
*                       
*                       rcbDataSize - Reference to variable that receives 
*                       size of data copied to rspDataBuffer.
*
* Return value:         If the function succeeds, the return value is 
*                       ERROR_SUCCESS. If the function fails, the return 
*                       value is a nonzero error code defined 
*                       in Winerror.h.
*
*************************************************************************/
LONG CRegistryTools::RegReadValue(
    HKEY                hKey,
    LPCTSTR             lpValueName,
    LPDWORD             lpType,
    countable< TCHAR >& rspDataBuffer,
    DWORD&              rcbDataSize )
{
    _ASSERT( hKey );
    
    TCHAR * pcData = NULL;
    rcbDataSize = 0;
    
    //
    // Determine required buffer size
    //
    LONG lResult = RegQueryValueEx( hKey,
                                    lpValueName, 
                                    NULL, 
                                    lpType, 
                                    NULL, 
                                    &rcbDataSize );
    if( ERROR_SUCCESS != lResult )
    {
        return lResult;
    }
    
    pcData = ( TCHAR* )new BYTE[ rcbDataSize ];

    if( !pcData )
    {
        return ENOMEM;
    }
    
    //
    // Read data
    //
    
    memset( pcData, 0, rcbDataSize );

    lResult = RegQueryValueEx(  hKey, 
                                lpValueName, 
                                NULL, 
                                lpType, 
                                ( LPBYTE )pcData, 
                                &rcbDataSize );

    if( ERROR_SUCCESS == lResult )
    {
        countable< TCHAR > pCountable( pcData );
        rspDataBuffer = pCountable;
    }

    return lResult;
}

/*************************************************************************
* Function Name:        CRegistryTools::IsKeyEmpty
* Function Description: Check is key contains any sub-keys or values.
*
* Arguments:            hKey - Opened key handle. Key must be opened 
*                       with the KEY_ENUMERATE_SUBKEYS access right.
*
* Return value:         True if key is empty, false otherwise.
*
*************************************************************************/
bool CRegistryTools::IsKeyEmpty( const HKEY hKey )
{
    _ASSERT( hKey );
    
    DWORD dwcSubKeys = 0;
    DWORD dwcValues  = 0;

    LONG lResult = RegQueryInfoKey( hKey, 
                                    NULL, 
                                    NULL, 
                                    NULL,
                                    &dwcSubKeys, 
                                    NULL, 
                                    NULL, 
                                    &dwcValues,  
                                    NULL, 
                                    NULL, 
                                    NULL, 
                                    NULL );
    if( ERROR_SUCCESS != lResult )
    {
        // Assume key is not empty if reading error occurred (we don't 
        // have enough info to make correct decision).
        //

        return false;
    }

    if( (!dwcSubKeys) && ((!dwcValues) || (1 == dwcValues)) )
    {
        // Assume key empty if it default value is empty and it doesn't 
        // contain any other values and sub-keys.
        //

        if( !dwcValues )
        {
            return true;
        }
        
        // if( 1 == dwcValues )
        countable< TCHAR > spDataBuffer;
        DWORD dwDataSize = 0;
        DWORD dwType = REG_SZ;

        LONG lResult = RegReadValue(
                                hKey,
                                NULL,
                                &dwType,
                                spDataBuffer,
                                dwDataSize );

        if( ERROR_SUCCESS == lResult )
        {
            if( _tcscmp( spDataBuffer.get(), _T("")) == 0 )
            {
                return true;
            }

            return false;
        }
    }

    return false;
}

/*************************************************************************
* Function Name:        CRegistryTools::IsKeyExist
* Function Description: Check existing registry key with given name.
*
* Arguments:            
*                       
*
* Return value:         True if key exist, false otherwise.
*
*************************************************************************/
bool CRegistryTools::IsKeyExist( const LPTSTR lpKeyFullPath )
{
	CString strKeyFullPath( lpKeyFullPath );
	HKEY hKey = GetHKey2( strKeyFullPath );

	return IsKeyExist( hKey, const_cast<const LPTSTR>( strKeyFullPath.GetString() ) );
}

/*************************************************************************
* Function Name:        CRegistryTools::IsKeyExist
* Function Description: Check existing registry key with given name.
*
* Arguments:            hKey - Opened key handle.
*                       lpKeyName - Name of sub-key is need to be checked.
*
* Return value:         True if key exist, false otherwise.
*
*************************************************************************/
bool CRegistryTools::IsKeyExist( 
    const HKEY      hKey, 
    const LPTSTR    lpKeyName )
{
    _ASSERT( hKey );
    
    bool bRetval    = true;
    HKEY hOpenKey   = NULL;
    LONG lResult    = RegOpenKey( hKey, lpKeyName, &hOpenKey );

    if( ERROR_FILE_NOT_FOUND == lResult )
    {
        bRetval = false;
    }

    if( hOpenKey )
    {
        RegCloseKey( hOpenKey );
    }
    
    return bRetval;
}

/*************************************************************************
* Function Name:        CRegistryTools::IsValueExist
* Function Description: Check existing registry key with given name.
*
* Arguments:            
*                       
*
* Return value:         True if key exist, false otherwise.
*
*************************************************************************/
bool CRegistryTools::IsValueExist( const LPTSTR lpKeyFullPath, const LPTSTR lpValueName )
{
	CString strKeyFullPath( lpKeyFullPath );
	HKEY hKey = GetHKey2( strKeyFullPath );

    _ASSERT( hKey );
    
    bool bRetval    = true;
    HKEY hOpenKey   = NULL;
	LONG lResult    = RegOpenKey( hKey, strKeyFullPath.GetString(), &hOpenKey );

    if( ERROR_FILE_NOT_FOUND == lResult )
    {
        bRetval = false;
    }

    if( hOpenKey )
    {
		if( bRetval )
		{
			DWORD dwDataSize    = 0;
			LONG lResult        = RegQueryValueEx(  hOpenKey,
													lpValueName, 
													NULL, 
													NULL, 
													NULL, 
													&dwDataSize );

			bRetval = ( ERROR_SUCCESS == lResult ) ? true : false;
		}

		RegCloseKey( hOpenKey );
    }

    return bRetval;
}

/*************************************************************************
* Function Name:          CRegistryTools::IsValueExist()
* Function Description:   Check existing registry value with given name.
*
* Arguments:              hKey - Opened key handle.
*                         lpValueName - Name of value is need 
*                         to be checked.
*
* Return value:           True if value exist, false otherwise.
*
*************************************************************************/
bool CRegistryTools::IsValueExist( 
    const HKEY      hKey,
    const LPTSTR    lpValueName )
{
    _ASSERT( hKey );
    
    DWORD dwDataSize    = 0;
    LONG lResult        = RegQueryValueEx(  hKey,
                                            lpValueName, 
                                            NULL, 
                                            NULL, 
                                            NULL, 
                                            &dwDataSize );

    return ( ERROR_SUCCESS == lResult ) ? true : false;
}

/*************************************************************************
* Function Name:          CRegistryTools::IsContainPath()
* Function Description:   Verify is given string path or not.
*
* Arguments:              strCheckString - String to be analyzed
*
* Return value:           True if incoming string is path, 
*                         false otherwise.
*
*************************************************************************/
bool CRegistryTools::IsContainPath( 
    const LPTSTR strCheckString )
{
    const UINT INFO_BUFFER_SIZE = 32768;
    TCHAR infoBuf[ INFO_BUFFER_SIZE ];
    
    DWORD bufCharCount = ExpandEnvironmentStrings(  strCheckString, 
                                                    infoBuf, 
                                                    INFO_BUFFER_SIZE ); 
    
    if(( bufCharCount > INFO_BUFFER_SIZE ) || ( !bufCharCount ))
    {
        return false;
    }
    
    if( !_tcsstr( infoBuf, _T( ":\\" )) )
    {
        return false;
    }
    
    return true;
}

/*************************************************************************
* Function Name:          CRegistryTools::IsStringGuid()
* Function Description:   Verify is given string (can be) guid or not.
*
* Arguments:              szGuid - Zero-terminating string to be checked.
*
* Return value:           True if incoming string is guid, 
*                         false otherwise.
*************************************************************************/
bool CRegistryTools::IsStringGuid( const TCHAR * szGuid )
{
    _ASSERT( szGuid != NULL );

    if( _T('{') != szGuid[ 0 ] )
    {
        return false;
    }
    
    if( _tcslen( szGuid ) != 38 )
    {
        return false;
    }

    if( _T('}') != szGuid[ 37 ] )
    {
        return false;
    }

    if( _T('-') != szGuid[ 9 ] )
    {
        return false;
    }

    if( _T('-') != szGuid[ 14 ] )
    {
        return false;
    }

    if( _T('-') != szGuid[ 19 ] )
    {
        return false;
    }

    if( _T('-') != szGuid[ 24 ] )
    {
        return false;
    }
    
    //
    // TODO: Check that all other symbols are hex digits?
    //

    return true;
}

/*************************************************************************
* Function Name:        CRegistryTools::GetNextPathChunk()
* Function Purpose:     Copy source string to destination buffer until
*                       delimiter found or terminating NULL reached.
*                       Zero symbol is added to end of copied data.
*
* Arguments:            rpSourceBuffer - Reference to pointer to  
*                       zero-terminating string from where data will
*                       be copied.
*
*                       rpDestBuffer - Reference to pointer to buffer 
*                       where data will be stored. Terminating zero will
*                       be added to the end of the buffer after copying
*                       next data chunk.
*
*                       nDestBufferSize - Size of free destination buffer 
*                       space. Must not be equal zero.
*
* Return value:         Number of characters saved in destination buffer,
*                       termination zero is not included.
*
*************************************************************************/
size_t CRegistryTools::GetNextPathChunk(
    const TCHAR* &  rpSourceBuffer, 
    TCHAR* &        rpDestBuffer, 
    unsigned        nDestBufferSize )
{
    _ASSERT( nDestBufferSize != 0 );
    unsigned i;

    for( i = 0; i < ( nDestBufferSize - 1 ); i++ )
    {
        //
        // Is the current symbol delimiter?
        //

        switch( *rpDestBuffer = *rpSourceBuffer )
        {
        case _T('\\'): 
        case _T(' '):
        case _T(','):
        case _T('/'):
        case _T('%'):
        case _T('<'):
        case _T('>'):
        case _T('|'):
        case _T('"'):
        case _T(';'):
            
        //
        // Possible the following delimiters are need to be added:
        // 

        /*
        case _T('$'):
        case _T('\''):
        case _T('`'):
        case _T('-'):
        case _T('_'):
        case _T('@'):
        case _T('~'):
        case _T('!'):
        case _T('('):
        case _T(')'):
        case _T('{'):
        case _T('}'):
        case _T('^'):
        case _T('#'):
        case _T('&'):
        case _T('+'):
        case _T('['):
        case _T(']'):
        case _T('='):
        */
        {
            if( !i )
            {
                rpSourceBuffer ++;
                rpDestBuffer ++;
                continue;
            }

            goto end_for_;
        }
        
        //
        // Does end of line reached?
        //

        case _T( '\0' ):

            return i;
        }
        
        rpSourceBuffer ++;
        rpDestBuffer ++;
    }

end_for_:
    *rpDestBuffer = _T('\0');
    return i;
}

/*************************************************************************
* Function Name:        CRegistryTools::GetNextPathChunk2()
* Function Purpose:     The same as GetNextPathChunk()
*                       Difference: '\' delimiter is excluded.
*
* Arguments:            The same as for the GetNextPathChunk()
* Return value:         See GetNextPathChunk() description.
*************************************************************************/
size_t CRegistryTools::GetNextPathChunk2(
    const TCHAR* &  rpSourceBuffer, 
    TCHAR* &        rpDestBuffer, 
    unsigned        nDestBufferSize )
{
    _ASSERT( nDestBufferSize != 0 );
    unsigned i;

    for( i = 0; i < ( nDestBufferSize - 1 ); i++ )
    {
        //
        // Is the current symbol delimiter?
        //

        switch( *rpDestBuffer = *rpSourceBuffer )
        {
        //case _T('\\'): 
        case _T(' '):
        case _T(','):
        case _T('/'):
        case _T('%'):
        case _T('<'):
        case _T('>'):
        case _T('|'):
        case _T('"'):
        case _T(';'):
            
        //
        // Possible the following delimiters are need to be added:
        // 

        /*
        case _T('$'):
        case _T('\''):
        case _T('`'):
        case _T('-'):
        case _T('_'):
        case _T('@'):
        case _T('~'):
        case _T('!'):
        case _T('('):
        case _T(')'):
        case _T('{'):
        case _T('}'):
        case _T('^'):
        case _T('#'):
        case _T('&'):
        case _T('+'):
        case _T('['):
        case _T(']'):
        case _T('='):
        */
        {
            if( !i )
            {
                rpSourceBuffer ++;
                rpDestBuffer ++;
                continue;
            }

            goto end_for2_;
        }
        
        //
        // Does end of line reached?
        //

        case _T( '\0' ):

            return i;
        }
        
        rpSourceBuffer ++;
        rpDestBuffer ++;
    }

end_for2_:
    *rpDestBuffer = _T('\0');
    return i;
}

/*************************************************************************
* Function Name:        CRegistryTools::CheckFileExistence()
* Function Purpose:     Try find path from the beginning of the string 
*                       and check if file under this path exist.
*
* Arguments:            rpSourceBuffer - Reference to pointer to  
*                       zero-terminating string from where data will
*                       be copied.
*
*                       rpDestBuffer - Reference to pointer to buffer 
*                       where data will be stored. Terminating zero will
*                       be added to the end of the buffer after copying
*                       next data chunk.
*
*                       nDestBufferSize - Size of free destination buffer 
*                       space. Must not be equal zero.
*
* Return value:         True if path found, false if not.
*
*************************************************************************/
bool CRegistryTools::CheckFileExistence(
                                        const TCHAR* &  rpSourceBuffer, 
                                        TCHAR* &        rpDestBuffer, 
                                        unsigned        nDestBufferSize )
{
    _ASSERT( nDestBufferSize );
    
    TCHAR * pOutBuffer = rpDestBuffer;
    size_t n, count = 0;
    bool bFound = false;

    while( 
        n = GetNextPathChunk(
                        rpSourceBuffer, 
                        rpDestBuffer, 
                        ( unsigned )( nDestBufferSize - count ))
        )
    {
        count += n;

        // String can contain either single file name or full path.
        //

        if( _tcsstr( pOutBuffer, _T( ":\\" )) )
        {
            struct _stat st;

            if( _tstat( pOutBuffer, &st ) == 0 )
            {
                if( st.st_mode & _S_IFREG )
                {
					LOG_TEXT3(_T("CheckFileExistence for file "),pOutBuffer,_T(" succeeded"))

					bFound = true;
                    break;
                }
            }
            else
            {
                _tcscat( pOutBuffer, _T( ".exe" ));

                if( _tstat( pOutBuffer, &st ) == 0 )
                {
                    if( st.st_mode & _S_IFREG )
                    {
						LOG_TEXT3(_T("CheckFileExistence for file "),pOutBuffer,_T(" succeeded"))

                        bFound = true;
                        break;
                    }                    
                }
                else
                {
                    // Remove '.exe' and continue
                    //

                    *rpDestBuffer = _T( '\0' );
                }
            }
        }
        else
        {
            TCHAR szBuffer[ MAX_PATH + 5 ]; // Reserved for '.exe'

            _tcsncpy( szBuffer, pOutBuffer, MAX_PATH );
            szBuffer[ MAX_PATH - 1 ] = _T('\0');

            if( PathFindOnPath( szBuffer, NULL ))
            {
				LOG_TEXT3(_T("PathFindOnPath for file "),szBuffer,_T(" succeeded"))

			    bFound = true;
                break;
            }
            else
            {
                _tcscat( szBuffer, _T( ".exe" ));

                if( PathFindOnPath( szBuffer, NULL ))
                {
					LOG_TEXT3(_T("PathFindOnPath for file "),szBuffer,_T(" succeeded"))

				    bFound = true;
                    break;
                }
            }
        }
    }

    return bFound;
}

/*************************************************************************
* Function Name:        CRegistryTools::CheckPathExistence()
* Function Purpose:     Check is path containing in the string exist 
*                       or not. Assume the string contains full path.
*
* Arguments:            szPathString - Zero-terminating string containing 
*                       path to file or folder. Checked string must begins 
*                       with symbols [D:\], where D is the drive letter.
*
* Return value:         True if file found, false otherwise.
*
*************************************************************************/
bool CRegistryTools::CheckPathExistence( const TCHAR * szPathString )
{
    _ASSERT( szPathString );

    const UINT DATA_BUFFER_SIZE = 32768;

    TCHAR accumulativeBuf[ DATA_BUFFER_SIZE + 5 ];  // reserved for '.exe'
    TCHAR * pOutBuffer = accumulativeBuf;
    
    size_t n, count = 0;
    bool bFound = false;

    while( n = GetNextPathChunk2(
                            szPathString, 
                            pOutBuffer, 
                            ( unsigned )( DATA_BUFFER_SIZE - count ))
         )
    {
        count += n;

        if( PathFileExists( accumulativeBuf ))
        {
            bFound = true;
            break;
        }
        else
        {
            _tcscat( accumulativeBuf, _T( ".exe" ));

            if( PathFileExists( accumulativeBuf ))
            {
                bFound = true;
                break;
            }
            else
            {
                //
                // Remove '.exe' and continue
                //

                *pOutBuffer = _T( '\0' );

                //
                // Cut off the end of the string from the last back slash:
                // this can be the resource section number 
                // (e.g. C:\Program Files\System32\mydll.dll\2 )
                // 
                
                TCHAR * pchSlash = _tcsrchr( accumulativeBuf, _T( '\\' ));

                if( pchSlash )
                {
                    struct _stat st;
                    *pchSlash = _T( '\0' );

                    if( _tstat( accumulativeBuf, &st ) == 0 )
                    {
                        if( st.st_mode & _S_IFREG )
                        {
                            bFound = true;
                            break;
                        }
                    }
                    else
                    {
                        TCHAR tailBuf[ DATA_BUFFER_SIZE ];  // for tail saving
                        
                        pchSlash++;
                        
                        _tcsncpy( tailBuf, pchSlash, sizeof( tailBuf ));
                        tailBuf[ sizeof( tailBuf ) - 1 ] = _T( '\0' );
                        _tcscat( accumulativeBuf, _T( ".exe" ));

                        pchSlash--;

                        if( _tstat( accumulativeBuf, &st ) == 0 )
                        {
                            if( st.st_mode & _S_IFREG )
                            {
                                bFound = true;
                                break;
                            }                    
                        }
                        else
                        {
                            //
                            // Remove '.exe' and continue
                            //

                            *pchSlash = _T( '\\' );
                            pchSlash++;
                            *pchSlash = _T( '\0' );

                            size_t count = sizeof( accumulativeBuf ) -
                                          _tcslen( accumulativeBuf );

                            _tcsncat( accumulativeBuf, tailBuf, count );

                            count = sizeof( accumulativeBuf ) - 1;
                            accumulativeBuf[ count ] = _T( '\0' );

                            continue;
                        }
                    }

                    *pchSlash = _T( '\\' );
                }
            }
        }
    }

    return bFound;
}

/*************************************************************************
* Function Name:        CRegistryTools::CheckPaths()
* Function Purpose:     Wrapper for CheckPath( const TCHAR * ).
*
* Arguments:            rspDataBuffer - Smart-pointer to string containing 
*                       file name(s).
*
* Return value:         True if file found, false otherwise.
*
*************************************************************************/
bool CRegistryTools::CheckPaths( countable< TCHAR >& rspDataBuffer, BOOL bCheckOnlyFullPath /* = TRUE /**/ )
{
    return CheckPaths( rspDataBuffer.get(), bCheckOnlyFullPath );
}

/*************************************************************************
* Function Name:        CRegistryTools::CheckPaths()
* Function Purpose:     Find path(s) in string and check are they exist 
*                       or not.
*
* Arguments:            szPathString - Zero-terminating string containing 
*                       file(s) name. String consist of path (optionally), 
*                       file name, possible arguments. Path may be Win32 
*                       or DOS path. String may be quoted or not. If 
*                       string contains file name only, file will be 
*                       searched in all paths enumerated in the 
*                       environment variable. String also can contain more 
*                       than one file name.
*
* Return value:         True if file found, false otherwise.
*
*************************************************************************/
bool CRegistryTools::CheckPaths( const TCHAR * szPathString, BOOL bCheckOnlyFullPath /* = TRUE /**/ )
{
    _ASSERT( szPathString );

    const UINT DATA_BUFFER_SIZE = 32768;

    TCHAR infoBuf[ DATA_BUFFER_SIZE ];
    const TCHAR * szInfoPtr = infoBuf;

    TCHAR accumulativeBuf[ DATA_BUFFER_SIZE + 5 ]; // reserved for '.exe'
    TCHAR * szAccumulativePtr = accumulativeBuf;

    infoBuf[ 0 ] = _T('\0');

    DWORD bufCharCount = ExpandEnvironmentStrings(
                                            szPathString, 
                                            infoBuf, 
                                            DATA_BUFFER_SIZE ); 

    if(( bufCharCount > DATA_BUFFER_SIZE ) || ( !bufCharCount ))
    {
        return false;
    }

	
	// Unquote initial string
    //
    while( _T('\"') == *szInfoPtr )
    {
         szInfoPtr ++;
    }
        

	TCHAR * pcQuote = const_cast<TCHAR *>(_tcsstr( szInfoPtr, _T( "\"" ) ));

    if( pcQuote )
    {
        *pcQuote = _T('\0');
    }

	// Do not check network paths - server can be turned off 
    // or network unavailable at scanning time.
    //
    if(( _T( '\\' ) == szInfoPtr[ 0 ] ) && 
       ( _T( '\\' ) == szInfoPtr[ 1 ] ))
    {
		LOG_TEXT3(_T("File "),szInfoPtr,_T(" is network path"))
		
        return true;
    }

	// If buffer contain full path, do not perform further parsing.
	if(bCheckOnlyFullPath)
	{
		if(!IsContainPath((const LPTSTR)szInfoPtr))
		{
		  LOG_TEXT3(_T("Skip not full path flag is set and file "),szInfoPtr,_T(" not contains :\\"))
    	  return true;
		}
	}

	bool bFound = CheckFileExistence(
                                szInfoPtr, 
                                szAccumulativePtr, 
                                DATA_BUFFER_SIZE );

    // If first chunk of string contained file name only, assume 
    // it was rundll32.exe, IsUninst.exe or other standard utility 
    // and continue parsing.
    //

    // Trim leading spaces in the rest of the incoming string.
    //
    if( *szInfoPtr )
    {
        while( _T(' ') == *szInfoPtr ++ ) ;
        szInfoPtr--;
    }

	if((NULL == *szInfoPtr) || (*szInfoPtr=='\\'))
		return bFound;

	// Unquote initial string
    //
    while( _T('\"') == *szInfoPtr )
    {
         szInfoPtr ++;
    }
        

	pcQuote = const_cast<TCHAR *>(_tcsstr( szInfoPtr, _T( "\"" ) ));

    if( pcQuote )
    {
        *pcQuote = _T('\0');
    }

	
	// If buffer contain full path, do not perform further parsing.
	if(bCheckOnlyFullPath)
	{
		if(!IsContainPath((const LPTSTR)szInfoPtr))
		{
  		  LOG_TEXT3(_T("Skip not full path flag is set and file "),szInfoPtr,_T(" after proccessing quotes not contains :\\"))
  	      return true;
		}
	}

			
    // Reset outgoing buffer.
    //
    szAccumulativePtr = accumulativeBuf;

    bFound = CheckFileExistence(
                            szInfoPtr, 
                            szAccumulativePtr, 
                            DATA_BUFFER_SIZE );
    return bFound;
}

/*************************************************************************
* Function Name:		CRegistryTools::GetHKey
* Function Description:	
*
* Arguments:			
*						
*						
* Return value:			
*
*************************************************************************/
HKEY CRegistryTools::GetHKey(CString & strKey)
{
	HKEY hKey = NULL;

	int nPos = strKey.Find(_T(':'));
	if (0 <= nPos)
	{
		CString strRootKey = strKey.Left(nPos);
		strKey.Delete(0, nPos + 1);

		if (0 == strRootKey.Compare(_T("HKLM")))
			hKey = HKEY_LOCAL_MACHINE;
		else if (0 == strRootKey.Compare(_T("HKCU")))
			hKey = HKEY_CURRENT_USER;
		else if (0 == strRootKey.Compare(_T("HKCR")))
			hKey = HKEY_CLASSES_ROOT;
	}

	return hKey;
}

/*************************************************************************
* Function Name:		CRegistryTools::GetHKey2()
* Function Description:	
*
* Arguments:			
*						
*						
* Return value:			
*
*************************************************************************/
HKEY CRegistryTools::GetHKey2(CString & strKey)
{
	HKEY hKey = NULL;

	int iPos = strKey.Find( _T("HKEY_LOCAL_MACHINE\\") );
	if( 0 != iPos )
		iPos = strKey.Find( _T("HKEY_CURRENT_USER\\") );
	if( 0 != iPos )
		iPos = strKey.Find( _T("HKEY_CLASSES_ROOT\\") );

	if( 0 != iPos )
		return hKey;

	iPos = strKey.Find( _T('\\') );

	CString strRootKey = strKey.Left( iPos );
	strKey.Delete( 0, iPos + 1 );

	if (0 == strRootKey.Compare(_T("HKEY_LOCAL_MACHINE")))
		hKey = HKEY_LOCAL_MACHINE;
	else if (0 == strRootKey.Compare(_T("HKEY_CURRENT_USER")))
		hKey = HKEY_CURRENT_USER;
	else if (0 == strRootKey.Compare(_T("HKEY_CLASSES_ROOT")))
		hKey = HKEY_CLASSES_ROOT;

	return hKey;
}

CString CRegistryTools::GetHKeyR( HKEY hKey )
{
	if( HKEY_LOCAL_MACHINE == hKey )
		return _T("HKEY_LOCAL_MACHINE\\");
	else if( HKEY_CURRENT_USER == hKey )
		return _T("HKEY_CURRENT_USER\\");
	else if( HKEY_CLASSES_ROOT == hKey )
		return _T("HKEY_CLASSES_ROOT\\");
	else if( HKEY_USERS == hKey )
		return _T("HKEY_USERS\\");
	else if( HKEY_CURRENT_CONFIG == hKey )
		return _T("HKEY_CURRENT_CONFIG\\");
	else if( HKEY_DYN_DATA == hKey )
		return _T("HKEY_DYN_DATA\\");
	else if( HKEY_PERFORMANCE_DATA == hKey )
		return _T("HKEY_PERFORMANCE_DATA\\");
	else if( HKEY_PERFORMANCE_TEXT == hKey )
		return _T("HKEY_PERFORMANCE_TEXT\\");
	else if( HKEY_PERFORMANCE_NLSTEXT == hKey )
		return _T("HKEY_PERFORMANCE_NLSTEXT\\");

	ATLASSERT( FALSE );

	return _T("");
}

/*************************************************************************
* Function Name:		CRegistryTools::DeleteRegKey
* Function Description:	
*
* Arguments:			
*						
*						
* Return value:			
*
*************************************************************************/
HRESULT CRegistryTools::DeleteRegKey(LPCTSTR lpKeyName)
{
	CString strTmp(lpKeyName);

	HKEY hKey = GetHKey2(strTmp);
	DWORD dwRes = ::SHDeleteKey(hKey, strTmp);

	if (ERROR_FILE_NOT_FOUND == dwRes)
		dwRes = ERROR_SUCCESS;

	return HRESULT_FROM_WIN32(dwRes);
}

/*************************************************************************
* Function Name:		CRegistryTools::DeleteRegValue
* Function Description:	
*
* Arguments:			
*						
*						
* Return value:			
*
*************************************************************************/
HRESULT CRegistryTools::DeleteRegValue(LPCTSTR lpValueName)
{
	CString strTmp(lpValueName);
	CString strName;

	HKEY hKey = GetHKey2(strTmp);

	int nPos = strTmp.ReverseFind(_T('|'));
	if (0 <= nPos)
	{
		strName = strTmp.Mid(nPos + 1);
		strTmp.Delete(nPos, strTmp.GetLength());
	}

	DWORD dwRes = ::SHDeleteValue(hKey, strTmp, strName);

	if (ERROR_FILE_NOT_FOUND == dwRes)
		dwRes = ERROR_SUCCESS;

	return HRESULT_FROM_WIN32(dwRes);
}

