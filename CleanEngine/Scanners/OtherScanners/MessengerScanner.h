#pragma once
#include "FileScanner.h"

class CMessengerScanner : public CFileScanner
{
protected:
    static const scanner_id m_nScannerId = MESSENGER_SCANNER_ID;

    virtual void algorithm( void );
    void ( CMessengerScanner::*m_pfnAlgoSelector )( void );

    // algorithm implementation in essence
    void algoStep1( void );
    void algoStep2( void );
    void algoStep3( void );
    void algoStep4( void );

	void GetICQUINs( CAtlArray< CString > & arrUINList );
	void GetICQInstallPath( LPTSTR szShortPath );

	void GetTrillianInstallPath( CString & szShortPath );
	void GetTrillianProfiles( CAtlArray< CString > & arrUINList );

protected:
    virtual void ReinitAlgorithm( void );

public:
    explicit CMessengerScanner( CCleanEngine * pEngine, INotifier * pNotifier = NULL, IScanProfiler * pProfiler = NULL );

    virtual ~CMessengerScanner( void );
    virtual scanner_id ScannerId( void ) const { return m_nScannerId; }
};
