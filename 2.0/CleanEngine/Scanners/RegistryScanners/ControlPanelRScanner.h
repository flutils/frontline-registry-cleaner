#pragma once
#include "RegistryScanner.h"

class CControlPanelRScanner : public CRegistryScanner
{
protected:
	static const scanner_id m_nScannerId = CTRLPANEL_SCANNER_ID;
	HKEY m_hScanningKey;

	virtual void algorithm( void );
	void ( CControlPanelRScanner::*m_pfnAlgoSelector )( void );

	// algorithm implementation in essence
	void algoStep1( void );

	void CControlPanelRScanner::CheckCtrlPanel( 
		HKEY            hKey, 
		const TCHAR *   szParentKey, 
		const TCHAR *   szFullKeyName );

protected:
	virtual void ReinitAlgorithm( void );

public:
	explicit CControlPanelRScanner( CCleanEngine * pEngine, INotifier * pNotifier = NULL, IScanProfiler * pProfiler = NULL );

	virtual ~CControlPanelRScanner( void );
	virtual scanner_id ScannerId( void ) const { return m_nScannerId; }
};
