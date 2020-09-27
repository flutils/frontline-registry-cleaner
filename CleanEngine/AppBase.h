#pragma once

#include <atlcoll.h>
#include <atlstr.h>
#include <CETypes.h>

struct SAppBase
{

	struct SAction
	{
		std::vector< std::pair< CString, ITEM_TYPE > > vThreatItems;
		CString strName;
		CString strIsInstallHDD;
		CString strIsInstallReg;
	};

	CString m_strDbFolder;
	CAtlMap< GUID, SAction > m_mapAction;

	~SAppBase(void);

	void Init( LPCTSTR lpDbFolder, BASE_SOURCE uiLoadFrom, HINSTANCE hResInstance );
	void ReadBaseFiles( UINT uiFrom );
	void ReadXml( CString &szFile, BOOL bFromRes );
	void GetApplications( CString &szFile );

	void GetAppList( CAtlArray<SAppInfo> & arrAppInfoList ) const;

	BOOL GetKeyFromSadsDataFormat( LPCTSTR lpSource, HKEY & hHive, LPCTSTR & lpKey ) const;
	BOOL GetRegValueFromSadsDataFormat( LPCTSTR lpSource, HKEY & hHive, CString & szKey, CString & szValue );

	static SAppBase * Instance();

protected:

	SAppBase(void);

};
