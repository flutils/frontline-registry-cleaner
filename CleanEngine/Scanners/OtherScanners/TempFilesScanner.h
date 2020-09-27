#pragma once
#include "FileScanner.h"

class CTempFilesScanner : public CFileScanner
{
protected:
    static const scanner_id m_nScannerId = TEMP_FILES_SCANNER_ID;

    virtual void algorithm( void );
    void ( CTempFilesScanner::*m_pfnAlgoSelector )( void );

    // algorithm implementation in essence
    void algoStep1( void );

	void DriveTraverse( std::string strDrive, const std::list<std::string> & lstExtMask );

protected:
    virtual void ReinitAlgorithm( void );

public:
    explicit CTempFilesScanner( CCleanEngine * pEngine, INotifier * pNotifier = NULL, IScanProfiler * pProfiler = NULL );

    virtual ~CTempFilesScanner( void );
    virtual scanner_id ScannerId( void ) const { return m_nScannerId; }
};
