#include "StdAfx.h"
#include ".\icomserverchecker.h"

IComServerChecker::IComServerChecker( ICLSIDChecker* parent ):
m_Parent(parent)
{
}

IComServerChecker::~IComServerChecker()
{
}

////////////////////////////////////////////////////////////////////

/************************************************************************
*
*
*
************************************************************************/
CInprocServerChecker::CInprocServerChecker( ICLSIDChecker * parent, CCleanEngine* pEngine ):
IComServerChecker(parent)
{

}

/************************************************************************
*
*
*
************************************************************************/
CInprocServerChecker::~CInprocServerChecker()
{

}

/************************************************************************
*
*
*
************************************************************************/
CHECK_RESULT CInprocServerChecker::Check( HKEY hKey, const TCHAR* szKeyName, HKEY hRootKey, const TCHAR * szRoot )
{

	HKEY hOpenKey = NULL;
	CString Server(szKeyName);
	Server += _T("\\InprocServer32");

	LOG_TEXT3( _T( "InprocServer check for " ), szKeyName, _T(" started"))

	LONG lResult = RegOpenKeyEx(
		hKey,
		Server.GetString(),
		0,
		KEY_READ|KEY_WRITE| DELETE,
		&hOpenKey );

	if( ERROR_SUCCESS != lResult )
	{
		LOG_TEXT3( _T( "RegOpenKeyEx for " ), Server, _T(" failed"))
		return C_FAILED;
	}

	countable< TCHAR > spDataBuffer;
	DWORD dwType, dwDataSize = 0;

	lResult = RegReadValue(
		hOpenKey,
		NULL,
		&dwType,
		spDataBuffer,
		dwDataSize );

	RegCloseKey( hOpenKey );

	if( ERROR_SUCCESS != lResult )
	{
		LOG_TEXT(_T("RegReadValue for default value was failed!"))

		return C_TREAT;
	}

	//if( m_Parent->m_pNotifier )
	//{
	//	m_Parent->m_pNotifier->CallOnScan( ScannerId(), NULL, spDataBuffer.get() );
	//}
	if ( !CheckPaths( spDataBuffer ) )
	{
		LOG_TEXT3(_T("CheckPaths for file "),spDataBuffer.get(),_T(" failed!"))

			return C_TREAT;

	}
	else
	{
		LOG_TEXT3(_T("File "),spDataBuffer.get(),_T(" present on the system"))

			if ( m_Parent->PathIsInReport( spDataBuffer.get() ) )
			{
				LOG_TEXT3(_T("File path "),spDataBuffer.get(),_T(" is in report as previously finded treat"))

					return C_TREAT;
			}
	}

	LOG_TEXT3 ( _T( "InprocServer check for" ), szKeyName, _T( "completed SUCCESSFULLY!" ) );
	return C_SUCCESS;
}

////////////////////////////////////////////////////////////////////

/************************************************************************
*
*
*
************************************************************************/
CLocalServerChecker::CLocalServerChecker( ICLSIDChecker * parent, CCleanEngine * pEngine ) :
IComServerChecker(parent)
{

}

/************************************************************************
*
*
*
************************************************************************/
CLocalServerChecker::~CLocalServerChecker()
{

}

/************************************************************************
*
*
*
************************************************************************/
CHECK_RESULT CLocalServerChecker::Check( HKEY hKey, const TCHAR* szKeyName, HKEY hRootKey, const TCHAR * szRoot )
{
	HKEY hOpenKey = NULL;
	CString Server(szKeyName);
	Server += _T("\\LocalServer32");

	LOG_TEXT3( _T( "LocalServer check for " ), szKeyName, _T(" started"))

	LONG lResult = RegOpenKeyEx(
		hKey,
		Server.GetString(),
		0,
		KEY_READ|KEY_WRITE| DELETE,
		&hOpenKey );

	if( ERROR_SUCCESS != lResult )
	{
		LOG_TEXT3( _T( "RegOpenKeyEx for " ), Server, _T(" failed"))
		return C_FAILED;
	}

	countable< TCHAR > spDataBuffer;
	DWORD dwType, dwDataSize = 0;

	lResult = RegReadValue(
		hOpenKey,
		NULL,
		&dwType,
		spDataBuffer,
		dwDataSize );

	RegCloseKey( hOpenKey );

	if( ERROR_SUCCESS != lResult )
	{
		LOG_TEXT(_T("RegReadValue for default value was failed!"))

		return C_TREAT;
	}

	//if( m_Parent->m_pNotifier )
	//{
	//	m_Parent->m_pNotifier->CallOnScan( m_Parent->ScannerId(), NULL, spDataBuffer.get() );
	//}

	if ( !CheckPaths( spDataBuffer ) )
	{
		LOG_TEXT3(_T("CheckPaths for file "),spDataBuffer.get(),_T(" failed!"))

			return C_TREAT;

	}
	else
	{
		LOG_TEXT3(_T("File "),spDataBuffer.get(),_T(" present on the system"))

			if ( m_Parent->PathIsInReport( spDataBuffer.get() ) )
			{
				LOG_TEXT3(_T("File path "),spDataBuffer.get(),_T(" is in report as previously finded treat"))

					return C_TREAT;
			}
	}

	LOG_TEXT3 ( _T( "LocalServer check for" ), szKeyName, _T( "completed SUCCESSFULLY!" ) );
	return C_SUCCESS;
}

////////////////////////////////////////////////////////////////////

/************************************************************************
*
*
*
************************************************************************/
CInprocHandlerChecker::CInprocHandlerChecker( ICLSIDChecker * parent, CCleanEngine* pEngine ):
IComServerChecker(parent)
{

}

/************************************************************************
*
*
*
************************************************************************/
CInprocHandlerChecker::~CInprocHandlerChecker()
{

}

/************************************************************************
*
*
*
************************************************************************/
CHECK_RESULT CInprocHandlerChecker::Check( HKEY hKey, const TCHAR* szKeyName, HKEY hRootKey, const TCHAR * szRoot )
{

	HKEY hOpenKey = NULL;
	CString Server(szKeyName);
	Server += _T("\\InprocHandler32");

	LOG_TEXT3( _T( "InprocHandler check for " ), szKeyName, _T(" started"))

	LONG lResult = RegOpenKeyEx(
		hKey,
		Server.GetString(),
		0,
		KEY_READ|KEY_WRITE| DELETE,
		&hOpenKey );

	if( ERROR_SUCCESS != lResult )
	{
		LOG_TEXT3( _T( "RegOpenKeyEx for " ), Server, _T(" failed"))

		return C_FAILED;
	}

	countable< TCHAR > spDataBuffer;
	DWORD dwType, dwDataSize = 0;

	lResult = RegReadValue(
		hOpenKey,
		NULL,
		&dwType,
		spDataBuffer,
		dwDataSize );

	RegCloseKey( hOpenKey );

	if( ERROR_SUCCESS != lResult )
	{
		LOG_TEXT(_T("RegReadValue for default value was failed!"))

			return C_FAILED;
	}

	//if( m_Parent->m_pNotifier )
	//{
	//	m_Parent->m_pNotifier->CallOnScan( ScannerId(), NULL, spDataBuffer.get() );
	//}
	if ( !CheckPaths( spDataBuffer ) )
	{
		LOG_TEXT3(_T("CheckPaths for file "),spDataBuffer.get(),_T(" failed!"))

			return C_TREAT;

	}
	else
	{
		LOG_TEXT3(_T("File "),spDataBuffer.get(),_T(" present on the system"))

			if ( m_Parent->PathIsInReport( spDataBuffer.get() ) )
			{
				LOG_TEXT3(_T("File path "),spDataBuffer.get(),_T(" is in report as previously finded treat"))

					return C_TREAT;
			}
	}

	LOG_TEXT3 ( _T( "InprocHandler check for" ), szKeyName, _T( "completed SUCCESSFULLY!" ) );
	return C_SUCCESS;
}

////////////////////////////////////////////////////////////////////

/************************************************************************
*
*
*
************************************************************************/
CProgIDChecker::CProgIDChecker( ICLSIDChecker * parent, CCleanEngine * pEngine ) :
IComServerChecker(parent)
{

}

/************************************************************************
*
*
*
************************************************************************/
CProgIDChecker::~CProgIDChecker()
{

}

/************************************************************************
*
*
*
************************************************************************/
CHECK_RESULT CProgIDChecker::Check( HKEY hKey, const TCHAR* szKeyName, HKEY hRootKey, const TCHAR * szRoot )
{
	HKEY hOpenKey = NULL;
	CString Server(szKeyName);
	Server += _T("\\ProgID");
	CHECK_RESULT CLSIDCheckSuccess = C_FAILED;

	LOG_TEXT3( _T( "ProgID check for " ), szKeyName, _T( " started" ) )

	LONG lResult = RegOpenKeyEx(
		hKey,
		Server.GetString(),
		0,
		KEY_READ|KEY_WRITE| DELETE,
		&hOpenKey );

	if( ERROR_SUCCESS != lResult )
	{
		LOG_TEXT3( _T( "RegOpenKeyEx for " ), Server, _T( " failed" ) )
		return C_FAILED;
	}

	countable< TCHAR > spDataBuffer;
	DWORD dwType, dwDataSize = 0;

	lResult = RegReadValue(
		hOpenKey,
		NULL,
		&dwType,
		spDataBuffer,
		dwDataSize );

	RegCloseKey( hOpenKey );

	if( ERROR_SUCCESS != lResult )
	{
		LOG_TEXT(_T("RegReadValue for default value was failed!"))

		return C_TREAT;
	}

	//if( m_Parent->m_pNotifier )
	//{
	//	m_Parent->m_pNotifier->CallOnScan( m_Parent->ScannerId(), NULL, spDataBuffer.get() );
	//}	
	CString regkey( spDataBuffer.get() );
	lResult = RegOpenKeyEx(
		hRootKey,
		regkey,
		0,
		KEY_READ|KEY_WRITE| DELETE,
		&hOpenKey );

	if( ERROR_SUCCESS != lResult )
	{
		CString str( szRoot );
		str += spDataBuffer.get(); 
		LOG_TEXT3( _T( "RegOpenKeyEx for " ), str , _T( " failed" ) )
		return C_TREAT;
	}

	CString strLog( szRoot );
	strLog += spDataBuffer.get();

	LOG_TEXT2( strLog, _T( " opened successfully" ) )

	HKEY hSubKey = NULL;

	lResult = RegOpenKeyEx(
		hOpenKey,
		_T( "CLSID" ),
		0,
		KEY_READ|KEY_WRITE| DELETE,
		&hSubKey );

	if( ERROR_SUCCESS != lResult )
	{
		CString str( strLog );
		str += _T( "\\CLSID");
		LOG_TEXT3( _T( "RegOpenKeyEx for " ), str , _T( " failed" ) )
		CLSIDCheckSuccess = C_FAILED;
	}
	else
	{
		LOG_TEXT2(  strLog + _T( "\\CLSID"), _T( " opened successfully" ) )
		countable< TCHAR > spDataBuffer;
		DWORD dwType, dwDataSize = 0;

		lResult = RegReadValue(
			hSubKey,
			NULL,
			&dwType,
			spDataBuffer,
			dwDataSize );

		//RegCloseKey( hOpenKey );

		if( ERROR_SUCCESS != lResult )
		{
			LOG_TEXT(_T("RegReadValue for default value was failed!"))

			lResult = RegReadValue(
				hSubKey,
				_T( "CLSID" ),
				&dwType,
				spDataBuffer,
				dwDataSize );

			if ( ERROR_SUCCESS != lResult )
			{
				RegCloseKey( hSubKey );
				RegCloseKey( hOpenKey );
				
				return C_TREAT;
			}
			
		}

		if ( m_Parent->CLSIDIsInReport( spDataBuffer.get(), szRoot))
		{
			LOG_TEXT3( _T( "CLSID " ), spDataBuffer.get(), _T( " is in report as previously finded treat" ) )
			return C_TREAT;
		}


		CString CheckedCLSID(szKeyName), RetreivedCLSID(spDataBuffer.get());

		if ( !CheckedCLSID.CompareNoCase(RetreivedCLSID) )
		{
			CLSIDCheckSuccess = C_SKIP;
		}
		else
		{
			if( m_Parent->CheckCLSID( RetreivedCLSID.GetString(), szRoot, FALSE ) )
			{
				CLSIDCheckSuccess = C_SUCCESS;
			}
			else
			{
				RegCloseKey( hSubKey );
				RegCloseKey( hOpenKey );
				
				return C_TREAT;
			}
		}
		RegCloseKey( hSubKey );
	}

	lResult = RegOpenKeyEx(
		hOpenKey,
		_T( "protocol\\StdFileEditing\\server" ),
		0,
		KEY_READ|KEY_WRITE| DELETE,
		&hSubKey );

	if ( ERROR_SUCCESS != lResult )
	{
		LOG_TEXT3( _T( "RegOpenKeyEx for " ), strLog + _T( "\\protocol\\StdFileEditing\\server"), _T( " failed" ) )

		if ( C_SKIP == CLSIDCheckSuccess)
		{
			RegCloseKey( hOpenKey );

			return C_SUCCESS;
		}

		if ( C_SUCCESS == CLSIDCheckSuccess)
		{
			RegCloseKey( hOpenKey );

			return C_SUCCESS;
		}
		else
		{
			RegCloseKey( hOpenKey );

			return C_TREAT;
		}
		
	}

	LOG_TEXT2( strLog + _T( "\\protocol\\StdFileEditing\\server"), _T( " opened succesfully" ) )

	lResult = RegReadValue(
		hSubKey,
		NULL,
		&dwType,
		spDataBuffer,
		dwDataSize );

	RegCloseKey( hSubKey );
	RegCloseKey( hOpenKey );

	if( ERROR_SUCCESS != lResult )
	{
		LOG_TEXT(_T("RegReadValue for default value was failed!"))

			return C_TREAT;
	}

	if ( !CheckPaths( spDataBuffer ) )
	{
		LOG_TEXT3(_T("CheckPaths for file "),spDataBuffer.get(),_T(" failed!"))

		return C_TREAT;
	}
	else
	{
		if ( m_Parent->PathIsInReport( spDataBuffer.get() ) )
		{
			LOG_TEXT3(_T("File path "),spDataBuffer.get(),_T(" is in report as previously finded treat"))

			return C_TREAT;
		}
	}

	LOG_TEXT3( _T( "ProgID check for" ), szKeyName, _T( " completed SUCCESSFULLY!") )

	return C_SUCCESS;
}
////////////////////////////////////////////////////////////////////

/************************************************************************
*
*
*
************************************************************************/
CAppIDChecker::CAppIDChecker( ICLSIDChecker * parent, CCleanEngine * pEngine ) :
IComServerChecker(parent)
{

}

/************************************************************************
*
*
*
************************************************************************/
CAppIDChecker::~CAppIDChecker()
{

}

/************************************************************************
*
*
*
************************************************************************/
CHECK_RESULT CAppIDChecker::Check( HKEY hKey, const TCHAR* szKeyName, HKEY hRootKey, const TCHAR * szRoot )
{
	HKEY hOpenKey = NULL;
	CString Server(szKeyName);
	//Server += _T("\\AppID");

	LOG_TEXT3( _T( "AppID check for " ), szKeyName, _T( " started" ))

	LONG lResult = RegOpenKeyEx(
		hKey,
		Server.GetString(),
		0,
		KEY_READ|KEY_WRITE| DELETE,
		&hOpenKey );

	if( ERROR_SUCCESS != lResult )
	{
		LOG_TEXT3( _T( "RegOpenKeyEx for " ), Server, _T( " failed!" ) )

		return C_FAILED;
	}

	countable< TCHAR > spDataBuffer;
	DWORD dwType, dwDataSize = 0;

	lResult = RegReadValue(
		hOpenKey,
		_T("AppID"),
		&dwType,
		spDataBuffer,
		dwDataSize );

	RegCloseKey( hOpenKey );

	if( ERROR_SUCCESS != lResult )
	{
		LOG_TEXT(_T("RegReadValue for default value was failed!"))

		return C_FAILED;
	}

	Server = _T( "AppID\\" );
	Server += spDataBuffer.get();

	lResult = RegOpenKeyEx(
		hRootKey,
		Server,
		0,
		KEY_READ|KEY_WRITE| DELETE,
		&hOpenKey );

	if( ERROR_SUCCESS != lResult )
	{
		CString str( szRoot );
		str += _T( "AppID\\" );
		str += spDataBuffer.get();
		LOG_TEXT3( _T( "RegOpenKeyEx for " ), str, _T( " failed" ) )
		return C_TREAT;
	}

	RegCloseKey( hOpenKey );

	LOG_TEXT3( _T( "AppID check for " ), szKeyName, _T( " completed SUCCESSFULLY" ))
	return C_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////

/************************************************************************
*
*
*
************************************************************************/
CTypeLibChecker::CTypeLibChecker( ICLSIDChecker * parent, CCleanEngine * pEngine ) :
IComServerChecker(parent)
{

}

/************************************************************************
*
*
*
************************************************************************/
CTypeLibChecker::~CTypeLibChecker()
{

}

/************************************************************************
*
*
*
************************************************************************/
CHECK_RESULT CTypeLibChecker::Check( HKEY hKey, const TCHAR* szKeyName, HKEY hRootKey, const TCHAR * szRoot )
{
	HKEY hOpenKey = NULL;
	CString Server(szKeyName);
	Server += _T("\\TypeLib");

	LOG_TEXT3( _T( "TypeLib check for " ), szKeyName, _T( " started" ))

		LONG lResult = RegOpenKeyEx(
		hKey,
		Server.GetString(),
		0,
		KEY_READ|KEY_WRITE| DELETE,
		&hOpenKey );

	if( ERROR_SUCCESS != lResult )
	{
		LOG_TEXT3( _T( "RegOpenKeyEx for " ), Server, _T( " failed!" ) )

			return C_FAILED;
	}

	countable< TCHAR > spDataBuffer;
	DWORD dwType, dwDataSize = 0;

	lResult = RegReadValue(
		hOpenKey,
		NULL,
		&dwType,
		spDataBuffer,
		dwDataSize );

	RegCloseKey( hOpenKey );

	if( ERROR_SUCCESS != lResult )
	{
		LOG_TEXT(_T("RegReadValue for default value was failed!"))

			return C_TREAT;
	}

	Server = _T( "TypeLib\\" );
	Server += spDataBuffer.get();

	ce_tstring str(szRoot);
	str += Server;

	if ( m_Parent->TypeLibIsInReport( str ) )
	{
		return C_TREAT;
	}

	lResult = RegOpenKeyEx(
		hRootKey,
		Server.GetString(),
		0,
		KEY_READ|KEY_WRITE| DELETE,
		&hOpenKey );

	if( ERROR_SUCCESS != lResult )
	{
		CString str( szRoot );
		str += _T( "TypeLib\\" );
		str += spDataBuffer.get();
		LOG_TEXT3( _T( "RegOpenKeyEx for " ), str, _T( " failed" ) )
			return C_TREAT;
	}

	RegCloseKey( hOpenKey );

	LOG_TEXT3( _T( "TypeLib check for " ), szKeyName, _T( " completed SUCCESSFULLY" ))
		return C_SUCCESS;
}

////////////////////////////////////////////////////////////////////

/************************************************************************
*
*
*
************************************************************************/
CAutoConvertToChecker::CAutoConvertToChecker( ICLSIDChecker * parent, CCleanEngine * pEngine ) :
IComServerChecker( parent )
{

}

/************************************************************************
*
*
*
************************************************************************/
CAutoConvertToChecker::~CAutoConvertToChecker()
{

}

/************************************************************************
*
*
*
************************************************************************/
CHECK_RESULT CAutoConvertToChecker::Check( HKEY hKey, const TCHAR* szKeyName, HKEY hRootKey, const TCHAR * szRoot )
{
	HKEY hOpenKey = NULL;
	CString Server(szKeyName);
	Server += _T("\\AutoConvertTo");

	LOG_TEXT3( _T( "AutoConvertTo check for " ), szKeyName, _T( " started" ))

	LONG lResult = RegOpenKeyEx(
		hKey,
		Server.GetString(),
		0,
		KEY_READ|KEY_WRITE| DELETE,
		&hOpenKey );

	if( ERROR_SUCCESS != lResult )
	{
		LOG_TEXT3( _T( "RegOpenKeyEx for " ), Server, _T( " failed!" ) )
		return C_FAILED;
	}

	countable< TCHAR > spDataBuffer;
	DWORD dwType, dwDataSize = 0;

	lResult = RegReadValue(
		hOpenKey,
		NULL,
		&dwType,
		spDataBuffer,
		dwDataSize );

	RegCloseKey( hOpenKey );

	if( ERROR_SUCCESS != lResult )
	{
		LOG_TEXT(_T("RegReadValue for default value was failed!"))

			return C_TREAT;
	}

	if( m_Parent->CLSIDIsInReport( spDataBuffer.get(), szRoot ) )
	{
		LOG_TEXT3( _T( "CLSID " ), spDataBuffer.get(), _T(" is in report as previously finded treat") )
		return C_TREAT;
	}

	if ( ! m_Parent->CheckCLSID( spDataBuffer.get(), szRoot, FALSE ) )
	{
		return C_TREAT;
	}

	LOG_TEXT3( _T( "AutoConvertTo check for " ), szKeyName, _T( " completed SUCCESSFULLY" ))
	return C_SUCCESS;
}

////////////////////////////////////////////////////////////////////

/************************************************************************
*
*
*
************************************************************************/
CAutoTreatAsChecker::CAutoTreatAsChecker( ICLSIDChecker * parent, CCleanEngine * pEngine ) :
IComServerChecker( parent )
{

}

/************************************************************************
*
*
*
************************************************************************/
CAutoTreatAsChecker::~CAutoTreatAsChecker()
{

}

/************************************************************************
*
*
*
************************************************************************/
CHECK_RESULT CAutoTreatAsChecker::Check( HKEY hKey, const TCHAR* szKeyName, HKEY hRootKey, const TCHAR * szRoot )
{
	HKEY hOpenKey = NULL;
	CString Server(szKeyName);
	Server += _T("\\AutoTreatAs");
	
	LOG_TEXT3( _T( "AutoTreatAs check for " ), szKeyName, _T( " started" ))

	LONG lResult = RegOpenKeyEx(
		hKey,
		Server.GetString(),
		0,
		KEY_READ|KEY_WRITE| DELETE,
		&hOpenKey );

	if( ERROR_SUCCESS != lResult )
	{
		LOG_TEXT3( _T( "RegOpenKeyEx for " ), Server, _T( " failed!" ) )
		return C_FAILED;
	}

	countable< TCHAR > spDataBuffer;
	DWORD dwType, dwDataSize = 0;

	lResult = RegReadValue(
		hOpenKey,
		NULL,
		&dwType,
		spDataBuffer,
		dwDataSize );

	RegCloseKey( hOpenKey );

	if( ERROR_SUCCESS != lResult )
	{
		LOG_TEXT(_T("RegReadValue for default value was failed!"))

		return C_TREAT;
	}

	if( m_Parent->CLSIDIsInReport( spDataBuffer.get(), szRoot ) )
	{
		LOG_TEXT3( _T( "CLSID " ), spDataBuffer.get(), _T(" is in report as previously finded treat") )

		return C_TREAT;
	}

	if ( ! m_Parent->CheckCLSID( spDataBuffer.get(), szRoot, FALSE ) )
	{
		return C_TREAT;
	}

	LOG_TEXT3( _T( "AutoTreatAs check for " ), szKeyName, _T( " completed SUCCESSFULLY" ))
	return C_SUCCESS;
}

////////////////////////////////////////////////////////////////////

/************************************************************************
*
*
*
************************************************************************/
CTreatAsChecker::CTreatAsChecker( ICLSIDChecker * parent, CCleanEngine * pEngine ) :
IComServerChecker( parent )
{

}

/************************************************************************
*
*
*
************************************************************************/
CTreatAsChecker::~CTreatAsChecker()
{

}

/************************************************************************
*
*
*
************************************************************************/
CHECK_RESULT CTreatAsChecker::Check( HKEY hKey, const TCHAR* szKeyName, HKEY hRootKey, const TCHAR * szRoot )
{
	HKEY hOpenKey = NULL;
	CString Server(szKeyName);
	Server += _T("\\TreatAs");
	
	LOG_TEXT3( _T( "TreatAs check for " ), szKeyName, _T( " started" ))

	LONG lResult = RegOpenKeyEx(
		hKey,
		Server.GetString(),
		0,
		KEY_READ|KEY_WRITE| DELETE,
		&hOpenKey );

	if( ERROR_SUCCESS != lResult )
	{
		return C_FAILED;
	}

	countable< TCHAR > spDataBuffer;
	DWORD dwType, dwDataSize = 0;

	lResult = RegReadValue(
		hOpenKey,
		NULL,
		&dwType,
		spDataBuffer,
		dwDataSize );

	RegCloseKey( hOpenKey );

	if( ERROR_SUCCESS != lResult )
	{
		LOG_TEXT(_T("RegReadValue for default value was failed!"))

		return C_TREAT;
	}

	if( m_Parent->CLSIDIsInReport( spDataBuffer.get(), szRoot ) )
	{
		LOG_TEXT3( _T( "CLSID " ), spDataBuffer.get(), _T(" is in report as previously finded treat") )

		return C_TREAT;
	}

	if ( ! m_Parent->CheckCLSID( spDataBuffer.get(), szRoot, FALSE ) )
	{
		return C_TREAT;
	}

	LOG_TEXT3( _T( "TreatAs check for " ), szKeyName, _T( " completed SUCCESSFULLY" ))

	return C_SUCCESS;
}

////////////////////////////////////////////////////////////////////

/************************************************************************
*
*
*
************************************************************************/
CPersistentHandlerChecker::CPersistentHandlerChecker( ICLSIDChecker * parent, CCleanEngine * pEngine ) :
IComServerChecker( parent )
{

}

/************************************************************************
*
*
*
************************************************************************/
CPersistentHandlerChecker::~CPersistentHandlerChecker()
{

}

/************************************************************************
*
*
*
************************************************************************/
CHECK_RESULT CPersistentHandlerChecker::Check( HKEY hKey, const TCHAR* szKeyName, HKEY hRootKey, const TCHAR * szRoot )
{
	HKEY hOpenKey = NULL;
	CString Server(szKeyName);
	Server += _T("\\PersistentHandler");

	LOG_TEXT3( _T( "PersistentHandler check for " ), szKeyName, _T( " started" ))

		LONG lResult = RegOpenKeyEx(
		hKey,
		Server.GetString(),
		0,
		KEY_READ|KEY_WRITE| DELETE,
		&hOpenKey );

	if( ERROR_SUCCESS != lResult )
	{
		return C_FAILED;
	}

	countable< TCHAR > spDataBuffer;
	DWORD dwType, dwDataSize = 0;

	lResult = RegReadValue(
		hOpenKey,
		NULL,
		&dwType,
		spDataBuffer,
		dwDataSize );

	RegCloseKey( hOpenKey );

	if( ERROR_SUCCESS != lResult )
	{
		LOG_TEXT(_T("RegReadValue for default value was failed!"))

			return C_TREAT;
	}

	if( m_Parent->CLSIDIsInReport( spDataBuffer.get(), szRoot ) )
	{
		LOG_TEXT3( _T( "CLSID " ), spDataBuffer.get(), _T(" is in report as previously finded treat") )

			return C_TREAT;
	}

	if ( ! m_Parent->CheckCLSID( spDataBuffer.get(), szRoot, FALSE ) )
	{
		return C_TREAT;
	}

	LOG_TEXT3( _T( "PersistentHandler check for " ), szKeyName, _T( " completed SUCCESSFULLY" ))

	return C_SUCCESS;
}

////////////////////////////////////////////////////////////////////

/************************************************************************
*
*
*
************************************************************************/
CPersistentAddinsRegisteredChecker::CPersistentAddinsRegisteredChecker( ICLSIDChecker * parent, CCleanEngine * pEngine ) :
IComServerChecker( parent )
{

}

/************************************************************************
*
*
*
************************************************************************/
CPersistentAddinsRegisteredChecker::~CPersistentAddinsRegisteredChecker()
{

}

/************************************************************************
*
*
*
************************************************************************/
CHECK_RESULT CPersistentAddinsRegisteredChecker::Check( HKEY hKey, const TCHAR* szKeyName, HKEY hRootKey, const TCHAR * szRoot )
{
	HKEY hOpenKey = NULL;
	HKEY hSubKey = NULL;
	CString Server(szKeyName);
	Server += _T("\\PersistentAdinsRegistered");

	LOG_TEXT3( _T( "PersistentAddinsRegistered check for " ), szKeyName, _T( " started" ))

	LONG lResult = RegOpenKeyEx(
		hKey,
		Server.GetString(),
		0,
		KEY_READ|KEY_WRITE| DELETE,
		&hOpenKey );

	if( ERROR_SUCCESS != lResult )
	{
		return C_FAILED;
	}

	lResult = RegOpenKeyEx(
		hOpenKey,
		_T( "{89BCB740-6119-101A-BCB7-00DD010655AF}" ),
		0,
		KEY_READ|KEY_WRITE| DELETE,
		&hSubKey );

	if( ERROR_SUCCESS != lResult )
	{
		RegCloseKey( hOpenKey );
		return C_TREAT;
	}

	countable< TCHAR > spDataBuffer;
	DWORD dwType, dwDataSize = 0;

	lResult = RegReadValue(
		hSubKey,
		NULL,
		&dwType,
		spDataBuffer,
		dwDataSize );

	RegCloseKey( hSubKey );
	RegCloseKey( hOpenKey );

	if( ERROR_SUCCESS != lResult )
	{
		LOG_TEXT(_T("RegReadValue for default value was failed!"))

			return C_TREAT;
	}

	if( m_Parent->CLSIDIsInReport( spDataBuffer.get(), szRoot ) )
	{
		LOG_TEXT3( _T( "CLSID " ), spDataBuffer.get(), _T(" is in report as previously finded treat") )

			return C_TREAT;
	}

	if ( ! m_Parent->CheckCLSID( spDataBuffer.get(), szRoot, FALSE ) )
	{
		return C_TREAT;
	}

	LOG_TEXT3( _T( "PersistentAddinsRegistered check for " ), szKeyName, _T( " completed SUCCESSFULLY" ))

		return C_SUCCESS;
}

////////////////////////////////////////////////////////////////////

/************************************************************************
*
*
*
************************************************************************/
CImplementedCategoriesChecker::CImplementedCategoriesChecker( ICLSIDChecker * parent, CCleanEngine * pEngine ) :
IComServerChecker( parent )
{

}

/************************************************************************
*
*
*
************************************************************************/
CImplementedCategoriesChecker::~CImplementedCategoriesChecker()
{

}

/************************************************************************
*
*
*
************************************************************************/
CHECK_RESULT CImplementedCategoriesChecker::Check( HKEY hKey, const TCHAR* szKeyName, HKEY hRootKey, const TCHAR * szRoot )
{
	HKEY hOpenKey = NULL;
	DWORD dwIndex = 0;
	LONG lResult = 0;
	CString Server(szKeyName);
	Server += _T("\\ImplementedCategories");
	CHECK_RESULT Res = C_SUCCESS;

	LOG_TEXT3( _T( "ImplementedCategories check for " ), szKeyName, _T( " started" ))

	lResult = RegOpenKeyEx(
		hKey,
		Server.GetString(),
		0,
		KEY_READ|KEY_WRITE| DELETE,
		&hOpenKey );

	if( ERROR_SUCCESS != lResult )
	{
		return C_FAILED;
	}

	TCHAR szSubKeyName[ MAX_KEY_SIZE ];
	DWORD dwSize = sizeof( szKeyName ) / sizeof( TCHAR );

	while( ERROR_SUCCESS == lResult )
	{
		lResult = RegEnumKeyEx(
			hOpenKey,
			dwIndex++,
			szSubKeyName,
			&dwSize,
			NULL,
			NULL,
			NULL,
			NULL );

		if ( ERROR_SUCCESS != lResult )
		{
			break;
		}

		if ( !m_Parent->CheckCLSID( szSubKeyName, szRoot, FALSE ) )
		{
			Res = C_FAILED;
		}
	}
	
	RegCloseKey( hOpenKey );

	return Res;
}

////////////////////////////////////////////////////////////////////

/************************************************************************
*
*
*
************************************************************************/
CNetSkipper::CNetSkipper( ICLSIDChecker * parent, CCleanEngine * pEngine ) :
IComServerChecker(parent)
{

}

/************************************************************************
*
*
*
************************************************************************/
CNetSkipper::~CNetSkipper()
{

}

/************************************************************************
*
*
*
************************************************************************/
CHECK_RESULT CNetSkipper::Check( HKEY hKey, const TCHAR* szKeyName, HKEY hRootKey, const TCHAR * szRoot )
{
	HKEY hOpenKey = NULL;
	CString Server(szKeyName);
	Server += _T("\\InprocServer32");

	BOOL AssemblyExists = FALSE;
	BOOL ClassExists = FALSE;
	BOOL RuntimeVersionExists = FALSE;

	LOG_TEXT3( _T( ".Net skipper check for " ), szKeyName, _T(" started"))

		LONG lResult = RegOpenKeyEx(
		hKey,
		Server.GetString(),
		0,
		KEY_READ|KEY_WRITE| DELETE,
		&hOpenKey );

	if( ERROR_SUCCESS != lResult )
	{
		LOG_TEXT3( _T( "RegOpenKeyEx for " ), Server, _T(" failed"))
			return C_FAILED;
	}

	countable< TCHAR > spDataBuffer;
	DWORD dwType, dwDataSize = 0;

	lResult = RegReadValue(
		hOpenKey,
		_T("Assembly"),
		&dwType,
		spDataBuffer,
		dwDataSize );

	if( ERROR_SUCCESS != lResult )
	{
		LOG_TEXT(_T("RegReadValue for Assembly value was failed!"))

		AssemblyExists = FALSE;
	}
	else
	{
		AssemblyExists = TRUE;
	}

	lResult = RegReadValue(
		hOpenKey,
		_T("Class"),
		&dwType,
		spDataBuffer,
		dwDataSize );

	if( ERROR_SUCCESS != lResult )
	{
		LOG_TEXT(_T("RegReadValue for Class value was failed!"))

		ClassExists = FALSE;
	}
	else
	{
		ClassExists = TRUE;
	}

	lResult = RegReadValue(
		hOpenKey,
		_T("RuntimeVersion"),
		&dwType,
		spDataBuffer,
		dwDataSize );

	RegCloseKey( hOpenKey );

	if( ERROR_SUCCESS != lResult )
	{
		LOG_TEXT(_T("RegReadValue for RuntimeVersion value was failed!"))

		RuntimeVersionExists = FALSE;
	}
	else
	{
		RuntimeVersionExists = TRUE;
	}

	//if ( AssemblyExists && ClassExists && RuntimeVersionExists )
	//{
	//	return C_SKIP;
	//}
	//else
	//{
	if ( AssemblyExists || ClassExists || RuntimeVersionExists )
	{
		LOG_TEXT3( _T( ".Net skipper check for " ), szKeyName, _T(" completed SUCESSFULLY"))
		return C_SKIP;
	}
	else
	{
		LOG_TEXT3( _T( ".Net skipper check for " ), szKeyName, _T(" FAILED!"))
		return C_FAILED;
	}
	//}

	LOG_TEXT3( _T( ".Net skipper check for " ), szKeyName, _T(" completed SUCESSFULLY"))
	return C_SKIP;
}

////////////////////////////////////////////////////////////////////

/************************************************************************
*
*
*
************************************************************************/
CEmptyKeyChecker::CEmptyKeyChecker( ICLSIDChecker * parent, CCleanEngine * pEngine ):
IComServerChecker( parent )
{

}

/************************************************************************
*
*
*
************************************************************************/
CEmptyKeyChecker::~CEmptyKeyChecker()
{

}

/************************************************************************
*
*
*
************************************************************************/
CHECK_RESULT CEmptyKeyChecker::Check( HKEY hKey, const TCHAR* szKeyName, HKEY hRootKey, const TCHAR * szRoot )
{
	HKEY hOpenKey = NULL;
	CString Server(szKeyName);

	LOG_TEXT3( _T( "Empty key check for " ), szKeyName, _T(" started"))

	LONG lResult = RegOpenKeyEx(
		hKey,
		Server.GetString(),
		0,
		KEY_READ|KEY_WRITE| DELETE,
		&hOpenKey );

	if( ERROR_SUCCESS != lResult )
	{
		LOG_TEXT3( _T( "RegOpenKeyEx for " ), Server, _T(" failed"))
		return C_TREAT;
	}

	//TCHAR       szSubKeyName[ MAX_KEY_SIZE ];
	//DWORD       dwSize = sizeof( szKeyName ) / sizeof( TCHAR );

	//lResult = RegEnumKeyEx(
	//	m_hScanningKey,
	//	0,
	//	szSubKeyName,
	//	&dwSize,
	//	NULL,
	//	NULL,
	//	NULL,
	//	NULL );

	if( IsKeyEmpty( hOpenKey ))
	{
		LOG_TEXT3( _T( "Empty key check for " ), Server, _T(" FAILED"))
		RegCloseKey(hOpenKey);
		return C_TREAT;
	}
	else
	{
		LOG_TEXT3( _T( "Empty key check for " ), Server, _T(" completed SUCCESSFULLY"))
		RegCloseKey(hOpenKey);
		return C_FAILED;
	}
}


