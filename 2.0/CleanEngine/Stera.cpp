#include "StdAfx.h"
#include ".\stera.h"
#include <shlobj.h>

const wchar_t g_wszPrefix[]   = L"\\??\\";
const wchar_t g_wszStera[]	  = L"daila";
const wchar_t g_wszSteraExe[] = L"daila.exe";
const wchar_t g_wszSteraJob[] = L"daila.job";

const wchar_t g_wszRegSessionManager[] = L"SYSTEM\\CurrentControlSet\\Control\\Session Manager";
const wchar_t g_wszBootStera[] = L"BootDaila";
const wchar_t g_wszBootExecute[] = L"BootExecute";

//global
Stera g_csStera;

//shark [11/11/06]: требование не показывать среди найденого то, что помечено как CleanOnReboot
std::set<CString> g_setCleanOnReboot;

Stera::Stera(void)
: m_nInitializeStatus(0)
{
	m_nCount = 1;
	m_hJobFile = INVALID_HANDLE_VALUE;
}

Stera::~Stera(void)
{
	UnInitialize();
}

int Stera::Initialize(void)
{
	wchar_t wszPath[MAX_PATH];
	int nLastError = 0;
	OSVERSIONINFO stOSver;
	stOSver.dwOSVersionInfoSize = sizeof( stOSver );
	if( !GetVersionEx( &stOSver ) ||
		( stOSver.dwMajorVersion != 5 ) )
		return NOT_NT_OS;
	SHGetFolderPathW( 0, CSIDL_SYSTEM, NULL, SHGFP_TYPE_DEFAULT, m_wszSystem32 );
	lstrcatW( m_wszSystem32, L"\\" );
	lstrcpyW( wszPath, m_wszSystem32 );
	lstrcatW( wszPath, g_wszSteraExe );
	if( GetFileAttributesW( wszPath ) == INVALID_FILE_ATTRIBUTES )
	{
		m_nInitializeStatus = 0;
		nLastError = NO_STERA_IN_SYSTEM32;
	}
	else 
	{ 
		//stera.exe in %system32%
		nLastError = _Initialize();
		if( !nLastError )
			m_nInitializeStatus = isSteraInitilized;
	}
	return nLastError;
}

int Stera::UnInitialize()
{
	m_nInitializeStatus = isNone;
	if( m_hJobFile != INVALID_HANDLE_VALUE )
		CloseHandle( m_hJobFile );
	m_hJobFile = INVALID_HANDLE_VALUE;
	return 0;
}

int Stera::Delete_File(wchar_t* pPath, UINT /*nCount*/)
{
	if( !m_nInitializeStatus )
	{
		Initialize();
	}

	if( m_nInitializeStatus )
	{
		wchar_t buf[300];
		DWORD dw = 0;
		lstrcpyW( buf, L"<" );
		lstrcatW( buf, g_wszPrefix );
		lstrcatW( buf, pPath );
		lstrcatW( buf, L"\r\n" );
		WriteFile( m_hJobFile, buf, lstrlenW( buf ) << 1, &dw, 0 );
		return 0;
	}	
	else
		return NOT_INITIALIZED;
}

int Stera::_Initialize()
{
	int nRet = SOME_ERROR;
	HKEY hKey = 0;
	RegOpenKeyExW( HKEY_LOCAL_MACHINE, g_wszRegSessionManager, 0, KEY_ALL_ACCESS, &hKey );
	if( hKey )
	{
		DWORD dw = sizeof( m_wszJobFilePath );
		if( RegQueryValueExW( hKey, g_wszBootStera, 0, 0, (LPBYTE)m_wszJobFilePath, &dw ) == ERROR_SUCCESS )
		{
			m_hJobFile = CreateFileW( m_wszJobFilePath + 4, GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0 );
			if( m_hJobFile == INVALID_HANDLE_VALUE )
			{
NOBootStera:
				//lstrcpyW( m_wszJobFilePath, g_wszPrefix );
				lstrcpyW( m_wszJobFilePath, m_wszSystem32 );
				lstrcatW( m_wszJobFilePath, g_wszSteraJob );
				m_hJobFile = CreateFileW( m_wszJobFilePath, GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0 );
				if( m_hJobFile == INVALID_HANDLE_VALUE )
					nRet = COULDNT_OPEN_JOB_FILE;
				else
				{
					wchar_t buf[300];
					buf[0] = 0xFEFF;
					dw = 0;
					WriteFile( m_hJobFile, buf, sizeof( wchar_t), &dw, 0 );
					lstrcpyW( buf, g_wszPrefix );
					lstrcatW( buf, m_wszJobFilePath );
					if( RegSetValueExW( hKey, g_wszBootStera, 0, REG_SZ, (LPBYTE)buf, lstrlenW( buf ) << 1 ) == ERROR_SUCCESS )
						nRet = 0;
					SetFilePointer( m_hJobFile, 0, 0, FILE_END );
				}
			} 
			else 
			{
				nRet = 0;
				SetFilePointer( m_hJobFile, 0, 0, FILE_END );
			}
			if( !nRet )
			{ //check if we in BootExecute
				wchar_t buf [1000],	//must be enough
						*p = buf;
				dw = sizeof( buf );
				if( RegQueryValueExW( hKey, g_wszBootExecute, 0, 0, (LPBYTE)buf, &dw ) == ERROR_SUCCESS )
				{
					dw = lstrlenW( p );
					while( dw )
					{
						if( lstrcmpW( p, g_wszStera ) == 0 )
							break;						
						p += dw + 1;
						dw = lstrlenW( p );
					} 
				}
				if( !dw ) //this mean no stera in BootExecute
				{
					lstrcpyW( p, g_wszStera );
					p += 6; //lstrlen( g_wszStera ) + 1;
					*p = 0;
					dw = ((DWORD)(p - buf) + 1) << 1;
					if( RegSetValueExW( hKey, g_wszBootExecute, 0, REG_MULTI_SZ, (LPBYTE)buf, dw ) != ERROR_SUCCESS )
						nRet = COULDNT_SET_BOOTEXEC;
				} 
			}
		} 
			else
				goto NOBootStera;
		RegCloseKey( hKey );
	} else
		nRet = COULDNT_OPEN_REG_SM;
	return nRet;
}

void LoadCleanOnRebootItems()
{
	wchar_t wszPath[MAX_PATH];

	SHGetFolderPathW( 0, CSIDL_SYSTEM, NULL, SHGFP_TYPE_DEFAULT, wszPath );
	lstrcatW( wszPath, L"\\" );
	lstrcatW( wszPath, g_wszSteraJob );

	if( GetFileAttributesW( wszPath ) == INVALID_FILE_ATTRIBUTES )
		return;

	HANDLE hFile = CreateFileW( wszPath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0 );
	if( INVALID_HANDLE_VALUE != hFile && 0 != hFile )
	{

		DWORD dwSize = GetFileSize( hFile, 0 );
		if( dwSize )
		{
			DWORD dw = 0;
			WCHAR *wszBuf = new WCHAR[dwSize];

			if(!wszBuf)
				return;

			if( ReadFile( hFile, (LPVOID)wszBuf, dwSize, &dw, 0 ) )
			{
				CString strBuffer( wszBuf );
				strBuffer.Trim( "\r\n?" );// обрезаем всякую херню, которую JG записал в конец...

				CString strLine;
				int nPos = 0;
				strLine = strBuffer.Tokenize( _T("\r\n"), nPos );
				while( !strLine.IsEmpty() )
				{
					strLine.Trim(_T("\r\n"));

					int n = strLine.Find(_T("??\\"));
					if( -1 != n )
						strLine.Delete( 0, n+3 );// удаляем лишнее в начале строки
										
					strLine.MakeLower();
					g_setCleanOnReboot.insert( strLine );
					strLine = strBuffer.Tokenize( _T("\r\n"), nPos );
				}
			}

			delete[] wszBuf;
		}
	}
	
}