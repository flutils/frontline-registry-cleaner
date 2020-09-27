#pragma once
#include "CleanEngine.h"
#include "Helpers.h"
#include "Logger.h"

class CReportChecker
{
protected:
	CCleanEngine* m_pCEngine;
public:
	CReportChecker(CCleanEngine* pEngine);
	~CReportChecker(void);

	BOOL CLSIDIsInReport( const TCHAR * szCLSID);
	BOOL PathIsInReport( const TCHAR * szPath);
	BOOL TypeLibIsInReport( const TCHAR * szTypeLib);
	BOOL UninstallIsInReport( const TCHAR * szUninstall);
};
