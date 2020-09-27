#pragma once
#include "FileScanner.h"
#include "RegistryScanner.h"
#include "AppBase.h"

class CAppScanner : public CFileScanner, public CRegistryScanner
{
protected:
    static const scanner_id m_nScannerId = APP_SCANNER_ID;

	CAtlArray< SAppInfo > m_arrAppInfoList;
	CAtlMap< GUID, SAppBase::SAction > & m_mapActionRef;

    virtual void algorithm( void );
    void ( CAppScanner::*m_pfnAlgoSelector )( void );

    // algorithm implementation in essence
    void algoStep1( void );

protected:
	virtual void prerun( void );
	virtual void ReinitAlgorithm( void );

public:
    explicit CAppScanner( CCleanEngine * pEngine, INotifier * pNotifier = NULL, IScanProfiler * pProfiler = NULL );

    virtual ~CAppScanner( void );
    virtual scanner_id ScannerId( void ) const { return m_nScannerId; }

    virtual void CleanupObject( countable< IReportItem > item );
};
