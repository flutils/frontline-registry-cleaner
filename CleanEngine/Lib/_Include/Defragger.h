#pragma once

const char kRegHives[] =  "SYSTEM\\CurrentControlSet\\Control\\hivelist";

class CDefragger
{
public:
	CDefragger(void);
	virtual ~CDefragger(void);

	BOOL LoadHivesInfo(CString pathToDefragDir);
	BOOL AnalyzeHives();
	BOOL CompactHives();

private:
	void GetPathFromDeviceName(char* pszFilename);
	BOOL SetPrivilege(LPCTSTR lpszPrivilege, BOOL bEnablePrivilege);

	CRITICAL_SECTION m_CriticalSection;

	struct HivesInfo
	{
		CString		csName;
		CString		csBackupPath;
		CString		csNewPath;
		HKEY		hKey;
		HivesInfo()
		{
			csName		= _T("");
			csBackupPath= _T("");
			csNewPath	= _T("");
			hKey		= NULL;
		}
	};

	typedef std::vector<HivesInfo> HIVESINFO;

	HIVESINFO m_HivesInfo;

	DWORD m_dwTotalOldSize;
	DWORD m_dwTotalNewSize;
};
