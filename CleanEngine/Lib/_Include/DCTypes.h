#pragma once

struct SAppInfo
{
	SAppInfo(void)
	{
	}

	SAppInfo(CString const & strName, GUID const & guid)
		: m_strName(strName), m_guid(guid)
	{
	}

	CString m_strName;
	GUID m_guid;
};

enum BASE_SOURCE
{
	LOAD_FROM_DAT = 0,
	LOAD_FROM_XML,
	LOAD_FROM_RES
};

#define CUSTOM_LIST			0x00000000
#define EXT_MASK			0x00000001
#define DRIVES_MASK			0x00000002
#define ID_SECURE_LEVEL		0x00000004
#define ID_LINK_SCANNER_TYPE 0x00000008
#define ID_REG_SCANNER_TYPE 0x00000010
