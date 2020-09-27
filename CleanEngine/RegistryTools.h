#pragma once

#include <sys/stat.h>
#include "countable.h"
#include "Logger.h"
// Size limit of registry key name.
//
#ifndef MAX_KEY_SIZE
#define MAX_KEY_SIZE    255
#endif

// Size limit of registry value name.
//
#ifndef MAX_VALUE_SIZE

#define MAX_VALUE_SIZE      16384
#endif

class CRegistryTools
{
    size_t GetNextPathChunk(
        const TCHAR* &  rszSourceBuffer, 
        TCHAR* &        rszDestBuffer, 
        unsigned        nDestBufferSize );

    size_t GetNextPathChunk2(
        const TCHAR* &  rszSourceBuffer, 
        TCHAR* &        rszDestBuffer, 
        unsigned        nDestBufferSize );

    bool CheckFileExistence(
        const TCHAR* &  rpSourceBuffer, 
        TCHAR* &        rpDestBuffer, 
        unsigned        nDestBufferSize );

protected:
    bool IsValueExist( const LPTSTR lpKeyFullPath, const LPTSTR lpValueName );
    bool IsValueExist( const HKEY hKey, const LPTSTR lpValueName );
	bool IsKeyExist( const LPTSTR lpKeyFullPath );
    bool IsKeyExist( const HKEY hKey, const LPTSTR lpKeyName );
    bool IsKeyEmpty( const HKEY hKey );
    bool IsContainPath( const LPTSTR strCheckString );
    bool IsStringGuid( const TCHAR * szGuid );

    bool CheckPaths( const TCHAR * szDataBuffer, BOOL bCheckOnlyFullPath = TRUE );
    bool CheckPaths( countable< TCHAR >& rspDataBuffer, BOOL bCheckOnlyFullPath = TRUE );

    bool CheckPathExistence( const TCHAR * szPathString );

	HRESULT DeleteRegKey(LPCTSTR lpKeyName);
	HRESULT DeleteRegValue(LPCTSTR lpValueName);
	HKEY GetHKey(CString & strKey);
	HKEY GetHKey2(CString & strKey);
	CString GetHKeyR( HKEY hKey );

	
	LONG RegReadValue(
        HKEY                hKey,
        LPCTSTR             lpValueName,
        LPDWORD             lpType,
        countable< TCHAR >& rspDataBuffer,
        DWORD&              rcbDataSize );
	
};
