// Profile.h: interface for the CProfile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(MENEES_PROFILE_H_INCLUDED)
#define MENEES_PROFILE_H_INCLUDED

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <atlstr.h>
#include <atlcoll.h>
#include <map>
// #include <afx.h>

/*
**********************************************************************
	CProfile - 12/27/97 - Bill Menees
**********************************************************************
	CProfile encapsulates reading and writing to Ini files and the 
	Windows Registry.

	The constructor takes three parameters:  
	* The "Name" paramter can either be the name of an Ini file 
	(with or without path), or it can be the path to a registry key.
	This parameter is required.
	* The "UseRegistry" parameter controls whether all reads and writes 
	are directed to the registry (instead of to an Ini file).  It
	defaults to true.
	* The "RegistryRootKey" is only used if "UseRegistry"
	is true.  "RegistryRootKey" determines whether the registry key
	specified by the "Name" parameter is under HKEY_CURRENT_USER or 
	HKEY_LOCAL_MACHINE.  It defaults to CProfile::hkeyCurrentUser.

	For the most part, using CProfile is very similar to using the
	PrivateProfile APIs and CWinApp's Profile methods.  However,
	two terms that I've used may need some clarification.  "Section" 
	means SECTION when you are dealing with Ini files, but it means 
	KEY when you are dealing with the registry.  Also, "Item" means 
	KEY when you are dealing with Ini files, but it means VALUE when 
	you are dealing	with the registry.  (This is a lot clearer if
	you're familiar with the Ini and Registry APIs.)

	A final interesting note: Be careful how you use empty strings!  
	* When you are working with Ini files, blank value names and data
	will delete sections and values	(because that's what the 
	underlying APIs do).
    * However, when you are working with the registry, blank section
	names and item names allow you to read and write from the 
	"(Default)" keys and values that appear in RegEdit (i.e. the ones
	that have blank names).  By writing to the section named "", you
	can write values directly under the key you passed to the 
	constructor.

**********************************************************************
*/

class CUserProfile 
{
public:
	enum CRegistryRootKey { hkeyCurrentUser, hkeyLocalMachine };

	//Constructor
	CUserProfile(const CString& Name, 
		const bool& UseRegistry = true, 
		const CRegistryRootKey& RegistryRootKey = hkeyCurrentUser);
	//Destructor
	virtual ~CUserProfile();

	//Strings
	CString GetLastSection(){ return m_sLastSection;};
    CString GetString(const CString& Section, const CString& Ident, const CString& Default);
    bool WriteString(const CString& Section, const CString& Ident, const CString& Value);
	
	//Integers
    int GetInt(const CString& Section, const CString& Ident, const int& Default);
    bool WriteInt(const CString& Section, const CString& Ident, const int& Value);
	
	//Booleans
    bool GetBool(const CString& Section, const CString& Ident, const bool& Default);
    bool WriteBool(const CString& Section, const CString& Ident, const bool& Value);

	//Doubles
	double GetDouble(const CString& Section, const CString& Ident, const double& Default);
	bool WriteDouble(const CString& Section, const CString& Ident, const double& Value);

	//Binary: Caller must delete memory allocated by GetBinary.  See GetDouble for example.
	bool GetBinary(const CString& Section, const CString& Ident, BYTE** ppData, UINT* pBytes);
	bool WriteBinary(const CString& Section, const CString& Ident, LPBYTE pData, UINT nBytes);

	//Sections and Values
    bool GetSections(CAtlArray<CString>* const Sections);
    bool GetSectionItems(const CString& Section, CAtlArray<CString>* Items);
	
	//Deletions
    bool DeleteSection(const CString& Section); //Won't recursively delete under WinNT!
    bool DeleteItem(const CString& Section, const CString& Ident);
	
	//Properties: FileName, UseRegistry, RegistryRootKey
    CString GetName();
	bool GetUseRegistry();
	CRegistryRootKey GetRegistryRootKey();
	bool IsRegistryValueSpecifyed(const CString& szSection, const CString& szIdent);
	static CString GetExecutableModulePathName();
	static CString GetExecutableModuleDir();
	static CString GetExecutableDir(const CString& szExecPath);
	static bool IsAppIdRegistered(const CString& szInstalledGUID);
	static CString CorrectSlesh(CString str);
	static BOOL IsFolderEmpty(const CString& szFolderPath);
	static BOOL DeleteRegistryKey(const CString& szRegValue, CUserProfile::CRegistryRootKey regKey);
	static BOOL DeleteRegistryKeyValue(const CString& szRegValue, CUserProfile::CRegistryRootKey regKey);
	static BOOL DeleteFileEx(const CString& szFilePath);
private:
	//Members set by constructor
	CString m_sName;
	bool m_bUseRegistry;
	CRegistryRootKey m_eRegistryRootKey;
	
	//Other members used to keep the last registry key open.
	CString m_sLastSection;
	HKEY m_hKey;

protected:
	HKEY OpenRegistryHKey(const CString& Section = _T(""));
	bool CloseRegistryHKey();
};


typedef std::multimap<CString,CString> XMLfilePart;
typedef std::pair<CString,CString> strPair;
typedef std::multimap<CString,CString>::iterator XMLfilePart_iterator;

class CUserXMLParser
{

public:
	CUserXMLParser(){}

	void InitFromFile( const CString& XMLPath );
	void InitFromBuffer( const CString& strData );

	size_t GetFilesCount(){ return m_arrXMLFiles.GetCount(); }

private:
	CString GetValue( int &pos, const CString &str );

public:
	CAtlArray<XMLfilePart> m_arrXMLFiles;

};

#endif // !defined(MENEES_PROFILE_H_INCLUDED)
