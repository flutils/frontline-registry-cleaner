#pragma once
#include "registryscanner.h"

class CEmtyKeysRScanner :
	public CRegistryScanner
{
public:
	explicit CEmtyKeysRScanner(CCleanEngine * pEngine, INotifier * pNotifier = NULL, IScanProfiler * pProfiler = NULL );
	virtual ~CEmtyKeysRScanner(void);
	virtual scanner_id ScannerId( void ) const { return m_nScannerId; }

protected:
	virtual void ReinitAlgorithm( void );
	static const scanner_id m_nScannerId = EMPTY_KEYS_SCANNER_ID;
	DWORD m_dwIndex;

	void ( CEmtyKeysRScanner::*m_pfnAlgoSelector ) ( void );

	virtual void postop( void );
	virtual void algorithm( void );

	void algoStep1( void );

};
