#pragma once

/*
	wrapper for stera.exe functional
	used for creation and add job to job file
	which can be created by several process
	and read data from log file.

	job file open between Initialize and UnInialize
*/

#define SOME_ERROR				-1
#define NOT_INITIALIZED			-2
#define NO_STERA_IN_SYSTEM32	-3
#define COULDNT_OPEN_REG_SM		-4
#define COULDNT_OPEN_JOB_FILE	-5
#define NO_BOOTEXECUTIVE		-6
#define COULDNT_SET_BOOTEXEC	-7
#define NOT_NT_OS				-8

#include <set>

class Stera
{
	enum InitializeStatus
	{
		isNone = 0,
		isSteraInitilized = 1,
		isLogFileRead = 2
	};

	UINT m_nCount;
public:
	Stera(void);
	~Stera(void);
	/*
		used to initialize class
		and return 0 if succeeded
		or error code otherwise
	*/
	int Initialize(void);
	/*
		Add to job file entry for file postpone deletion
	*/
	int Delete_File(wchar_t* pPath, UINT nCount );
	
	/*
		close job file (if it is opened)
	*/
	int UnInitialize();

protected:
	HANDLE m_hJobFile;
	int m_nInitializeStatus;
	wchar_t	m_wszJobFilePath[MAX_PATH];
	wchar_t m_wszSystem32[MAX_PATH];

	int _Initialize();
};

// These were origicnally declared in the source
// Moved to here based on StackOverflow recommendation
extern Stera g_csStera;
extern std::set<CString> g_setCleanOnReboot;

//removed void LoadCleanOnRebootItems();