#pragma once
#include "CleanScanner.h"

// disable warning C4290: C++ exception specification ignored 
// except to indicate a function is not __declspec(nothrow)
#pragma warning( push )
#pragma warning( disable : 4290 )

class CCleanEngine;

class CUnknownScanner : public CGenException
{
public:
    CUnknownScanner( const TCHAR* where, int line):
	CGenException( -1, _T( "Unknown scanner Id" ), where, line ) {};
};

class CScannerFactory
{
public:
    CScannerFactory( void );
    virtual ~CScannerFactory( void );

    CCleanScanner * CreateScanner( scanner_id Id, CCleanEngine * pEngine, INotifier * pNotifier = NULL, IScanProfiler * pProfiler = NULL )
		throw( CUnknownScanner, CGenException );
};

#pragma warning( pop )
