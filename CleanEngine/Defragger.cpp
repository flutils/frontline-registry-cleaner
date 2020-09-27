#include "StdAfx.h"
#include "Defragger.h"
#include "shlobj.h"
#define BUFSIZE 512

CDefragger::CDefragger()
{
	if (!InitializeCriticalSectionAndSpinCount(&m_CriticalSection, 0x80000400))
	{

	}
}

CDefragger::~CDefragger()
{
	DeleteCriticalSection(&m_CriticalSection);
}


BOOL CDefragger::LoadHivesInfo(CString csDefragPath)
{
	BOOL res = TRUE;
	//initialize clsid list
	m_HivesInfo.clear();
	//m_dwTotalOldSize = 0;
	//m_dwTotalNewSize = 0;


	CString csSubKeyPath = kRegHives;
	HKEY hSubKeyResult = NULL;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, csSubKeyPath, NULL, KEY_READ, &hSubKeyResult) != ERROR_SUCCESS)
		return FALSE;
	
	int dwIndex = 0;
	DWORD dwType = 0;

	std::vector<TCHAR> szName(MAX_PATH);
	DWORD cchName = MAX_PATH;
	DWORD dwSize = 0;

	while (RegEnumValue(hSubKeyResult, dwIndex, &szName[0], &cchName, NULL, &dwType, NULL, &dwSize) == ERROR_SUCCESS )//&& m_bRunning)
	{
		if (dwSize <= 0)
		{
			// Reset back the variables and continue for the next item.
			cchName = MAX_PATH;
			dwSize = 0;
			dwIndex ++;
			continue;
		}

		//m_pStatus->UpdateProgress(eProgRunning);
		// The above method will return the size in bytes so we need to get a TCHAR below.
		// Add sizeof(TCHAR) to be sure we will be a NULL terminated.
		std::vector<TCHAR> data(dwSize / sizeof(TCHAR) + sizeof(TCHAR), 0);
		cchName = MAX_PATH;
		if (RegEnumValue(hSubKeyResult, dwIndex, &szName[0], &cchName, NULL, &dwType, (LPBYTE) &data[0], &dwSize) != ERROR_SUCCESS)
		{
			res = FALSE;
			//ASSERT(FALSE);
			break;
		}

		CString csCurPath = &data[0];
		CString csFullName = &szName[0];
		
		csCurPath.MakeLower();


		if (!csCurPath.IsEmpty() )
		{
			if(!PathFileExists(csCurPath))
			{
				char  acDevice[MAX_PATH];// = new char[csTemp.GetLength() + 1];
				strcpy_s((char*)acDevice,MAX_PATH,csCurPath);
				GetPathFromDeviceName(acDevice);
				csCurPath.Format("%s",acDevice);
			}
			if(PathFileExists(csCurPath))
			{
				int pos = csFullName.ReverseFind('\\') + 1;
				CString csName = csFullName.Right(csFullName.GetLength() - pos);
				CString csFileName;

				HKEY hKey = NULL;
				csFullName.MakeLower();
				if (csFullName.Find(_T("\\registry\\machine")) >= 0 )
				{
					csFileName.Format("LM_%s", csName);
					hKey = HKEY_LOCAL_MACHINE;
				}
				else if (csFullName.Find(_T("\\registry\\user")) >= 0 )
				{
					csFileName.Format("USERS_%s", csName);
					hKey = HKEY_USERS;
				}

				CString csNewPath = csDefragPath + "\\" + csFileName;// + ".reg";
				CString csBackupPath = csDefragPath + "\\Backup\\" + csFileName;// + ".bak";

				DeleteFile(csNewPath);
				DeleteFile(csBackupPath);


				HivesInfo hivesInfo;
				
				hivesInfo.csName    = csName;
				hivesInfo.csBackupPath = csBackupPath;
				hivesInfo.csNewPath = csNewPath;
				//m_dwTotalOldSize	= m_dwTotalOldSize + GetFileSize(csCurPath);
				hivesInfo.hKey		= hKey;

				m_HivesInfo.push_back(hivesInfo);
			}
		}

		// Reset back the variables
		cchName = MAX_PATH;
		dwSize = 0;
		dwIndex ++;
	}
	return res;
}

BOOL CDefragger::AnalyzeHives()
{
	BOOL res = FALSE;

	//m_dwTotalNewSize = 0;

	if ((int)m_HivesInfo.size() < 0)
	{
		return FALSE;
	}

	SetPrivilege(SE_BACKUP_NAME,TRUE);

	for ( int nIndex = 0; nIndex < (int)m_HivesInfo.size(); nIndex ++)
	{
		HKEY hKeyResult = NULL;
		if (RegOpenKeyEx(m_HivesInfo.at(nIndex).hKey, m_HivesInfo.at(nIndex).csName, NULL, KEY_READ, &hKeyResult) != ERROR_SUCCESS)
			continue;
		

		//m_pStatus->UpdateProgress(eProgRunning);
		// Request ownership of the critical section.
		EnterCriticalSection(&m_CriticalSection);

		RegFlushKey(hKeyResult);
		//alaqwer
		DeleteFile(m_HivesInfo.at(nIndex).csNewPath);
		
		//::MessageBox(0,m_HivesInfo.at(nIndex).csNewPath, m_HivesInfo.at(nIndex).csName,0);
		if( RegSaveKey(hKeyResult, m_HivesInfo.at(nIndex).csNewPath, NULL) == ERROR_SUCCESS)
		{
			//m_dwTotalNewSize = m_dwTotalNewSize + GetFileSize(m_HivesInfo.at(nIndex).csNewPath);
			res = TRUE;
		}
		else
		{
			DeleteFile(m_HivesInfo.at(nIndex).csNewPath);
		}

		// Release ownership of the critical section.
		LeaveCriticalSection(&m_CriticalSection);
	}

	SetPrivilege(SE_BACKUP_NAME,FALSE);

	return res;
}

BOOL CDefragger::CompactHives()
{
	BOOL res = FALSE;

	if ((int)m_HivesInfo.size() < 0)
	{
		return FALSE;
	}

	//if(m_dwTotalNewSize < m_dwTotalOldSize)
	{
		SetPrivilege(SE_RESTORE_NAME,TRUE);
		for ( int nIndex = 0; nIndex < (int)m_HivesInfo.size(); nIndex ++)
		{
			//HKEY hKeyResult = NULL;
			//if (RegOpenKeyEx(m_HivesInfo.at(nIndex).hKey, m_HivesInfo.at(nIndex).csName, NULL, KEY_READ | KEY_WRITE, &hKeyResult) != ERROR_SUCCESS)
			//	continue;


			//m_pStatus->UpdateProgress(eProgRunning);

			if(PathFileExists(m_HivesInfo.at(nIndex).csNewPath))
			{
				// Request ownership of the critical section.
				EnterCriticalSection(&m_CriticalSection);

				//RegFlushKey(hKeyResult);

				DeleteFile(m_HivesInfo.at(nIndex).csBackupPath);

				//LONG err = RegReplaceKey(m_HivesInfo.at(nIndex).hKey, m_HivesInfo.at(nIndex).csName, m_HivesInfo.at(nIndex).csNewPath, m_HivesInfo.at(nIndex).csBackupPath);
				if(RegReplaceKey(m_HivesInfo.at(nIndex).hKey, m_HivesInfo.at(nIndex).csName, m_HivesInfo.at(nIndex).csNewPath, m_HivesInfo.at(nIndex).csBackupPath) == ERROR_SUCCESS)
				{
					res = TRUE;
				}
				else
				{
					DeleteFile(m_HivesInfo.at(nIndex).csBackupPath);
				}

				// Release ownership of the critical section.
				LeaveCriticalSection(&m_CriticalSection);
			}
		}
		SetPrivilege(SE_RESTORE_NAME,FALSE);
		return TRUE;
	}
	/*else
	{
		res = FALSE;
	}*/

	return FALSE;
}


void CDefragger::GetPathFromDeviceName(char* pszFilename)
{

	// Translate path with device name to drive letters.
	TCHAR szTemp[BUFSIZE];
	szTemp[0] = '\0';

	//printf("Device name is %s\n", pszFilename);

	if (GetLogicalDriveStrings(BUFSIZE-1, szTemp))
	{
		TCHAR szName[MAX_PATH];
		TCHAR szDrive[3] = TEXT(" :");
		BOOL bFound = FALSE;
		TCHAR* p = szTemp;

		do
		{
			// Copy the drive letter to the template string
			*szDrive = *p;

			// Look up each device name
			if (QueryDosDevice(szDrive, szName, BUFSIZE))
			{
				UINT uNameLen = _tcslen(szName);

				if (uNameLen < MAX_PATH)
				{
					bFound = _tcsnicmp(pszFilename, szName,
						uNameLen) == 0;

					if (bFound)
					{
						TCHAR szTempFile[MAX_PATH];
						_stprintf(szTempFile,
							TEXT("%s%s"),
							szDrive,
							pszFilename+uNameLen);
						_tcsncpy(pszFilename, szTempFile, MAX_PATH);
					}
				}
			}

			// Go to the next NULL character.
			while (*p++);
		} while (!bFound && *p); // end of string
	}
}


BOOL CDefragger::SetPrivilege(LPCTSTR lpszPrivilege, BOOL bEnablePrivilege)
{
	TOKEN_PRIVILEGES tp;
	LUID luid;
	HANDLE hToken; 

	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
	if ( !LookupPrivilegeValue(NULL, lpszPrivilege, &luid) )    
		return FALSE; 

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;

	if (bEnablePrivilege)
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	else
		tp.Privileges[0].Attributes = 0;

	AdjustTokenPrivileges(hToken, FALSE, &tp, 0, (PTOKEN_PRIVILEGES) NULL, 0); 

	return ( (GetLastError()!=ERROR_SUCCESS)?FALSE:TRUE);
}