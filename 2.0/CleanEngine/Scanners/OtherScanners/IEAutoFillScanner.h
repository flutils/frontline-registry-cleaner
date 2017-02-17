#pragma once

#include "CleanScanner.h"
#include "PStoreWrapper.h"

class CIEAutoFillScanner : public CCleanScanner
{

protected:
    static const scanner_id m_nScannerId = IE_AUTOFILL_SCANNER_ID;

    virtual void algorithm( void );
    void ( CIEAutoFillScanner::*m_pfnAlgoSelector ) ( void );

    // algorithm implementation in essence
    void algoStep1( void );

	BOOL GetCMStoreInterface(CMStore **ppStore);

protected:
	HINSTANCE m_hPStoreLib;

protected:
    virtual void ReinitAlgorithm( void );

public:
    explicit CIEAutoFillScanner( CCleanEngine * pEngine, INotifier * pNotifier = NULL, IScanProfiler * pProfiler = NULL );

    virtual ~CIEAutoFillScanner( void );

    virtual scanner_id ScannerId( void ) const { return m_nScannerId; }

	virtual void CleanupObject( countable< IReportItem > item );

};
