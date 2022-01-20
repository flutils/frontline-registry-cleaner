#pragma once

#include "DCTypes.h"

#define ABANDONNED	0x0200
#define RESUMED		0x0201
#define PAUSED		0x0202
#define NONE		0x0203

typedef HANDLE runnable_id;
typedef LONG item_id;
typedef LONG param_id;

//class CString_like_std_string : public CString
//{
//	typedef int size_type;
//
//	inline size_type size() { return GetLength(); }
//	inline void resize( size_type _Newsize ) { GetBufferSetLength( _Newsize ); }
//};

typedef CString ce_tstring;

//typedef std::basic_string< TCHAR > ce_tstring;
//typedef std::basic_stringstream< TCHAR > ce_tstringstream;

enum RNSTATUS
{
	ST_CREATED = 0,
	ST_STARTING,
	ST_RUNNING,
	ST_SLEEPING,
	ST_STOPPED,
	ST_ERROR
};

enum RNRESULT
{
	ERR_SUCCESS = 0,
	
	ERR_FAILED,
	ERR_ALREADY_RUNNING,
	ERR_NOT_RUNNING,
	ERR_ID_NOT_FOUND
};

enum RunnableState
{
	Sleeping,
	Working,
	Abandoned
};

enum COMPLETION_STATUS
{
	ST_WASNOTRUNNED = 1,
	ST_COMPLETED,
	ST_ABORTED
};

enum FIX_RESULT
{
	FR_FIXED = 1,
	FR_NOT_FIXED,
	FR_NOT_FOUND
};

enum ITEM_TYPE
{
	IT_NONE = 0,
	IT_REG_KEY,
	IT_REG_VALUE,
	IT_FILE,
	IT_DIRECTORY,
	IT_OTHER
};

enum SEVERITY
{
	SEVERITY_NONE = 0,
	SEVERITY_LOW,
	SEVERITY_MEDIUM,
	SEVERITY_DANGEROUS,
	SEVERITY_CRITICAL,
	SEVERITY_SEVERE
};

enum THREAT_STATUS
{
	STATUS_FOUND,
	STATUS_SKIPPED,
	STATUS_CLEANING,
	STATUS_CLEANED,
	STATUS_CLEAN_ON_REBOOT,
	STATUS_CLEAN_FAILED
};

enum CHECKITEM_RESULT
{
    CHECK_ERROR         = -1,
    ITEM_OK             = 1, 
    ITEM_EMPTY          = 2, 
    ITEM_INCORRECT      = 3,
    ITEM_NOT_EXIST      = 4,
};

enum CHECKPATH_RESULT
{
	NO_PATH             = 0,
	CORRECT_PATH        = 1,
	INVALID_PATH        = 2,
};

typedef struct
{
    //************************************************
    // Must be one of the following predefined keys:
    //      HKEY_CLASSES_ROOT
    //      HKEY_CURRENT_USER
    //      HKEY_LOCAL_MACHINE
    //      HKEY_USERS
    //************************************************
    HKEY        hKey;
    ce_tstring     strKeyName;

} MruKey;

#define DEFINE_CRITICAL_UNIT(name)\
	CRITICAL_SECTION		m_cs##name;\
	inline void Lock##name() { EnterCriticalSection( &m_cs##name ); }\
	inline void Unlock##name() { LeaveCriticalSection( &m_cs##name ); }

#define INIT_CRITICAL_UNIT(name)\
	InitializeCriticalSection( &m_cs##name );

#define UNINIT_CRITICAL_UNIT(name)\
	DeleteCriticalSection( &m_cs##name );
