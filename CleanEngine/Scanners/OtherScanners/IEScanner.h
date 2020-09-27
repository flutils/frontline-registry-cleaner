#pragma once
#include "FileScanner.h"
#include "RegistryScanner.h"
#include "IETools.h"

#include <UrlHist.h>

class CIEScanner : public CFileScanner, public CRegistryScanner, protected CIETools
{
protected:
	static const scanner_id m_nScannerId = IE_SCANNER_ID;

    virtual void algorithm( void );
    void ( CIEScanner::*m_pfnAlgoSelector )( void );

    // algorithm implementation in essence
    void algoStep1( void );
    void algoStep2( void );
    void algoStep3( void );
    void algoStep4( void );

	void EnumValues( HKEY hKey, const CString & strKeyName );
	HRESULT SafeEnumNext( IEnumSTATURL * pEnum, ULONG celt, LPSTATURL rgelt, ULONG *pceltFetched );

protected:
    virtual void ReinitAlgorithm( void );

public:
    explicit CIEScanner( CCleanEngine * pEngine, INotifier * pNotifier = NULL, IScanProfiler * pProfiler = NULL );

    virtual ~CIEScanner( void );
    virtual scanner_id ScannerId( void ) const { return m_nScannerId; }

    virtual void CleanupObject( countable< IReportItem > item );
};
