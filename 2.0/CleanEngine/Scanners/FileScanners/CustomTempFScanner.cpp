#include "StdAfx.h"
#include "CustomTempFScanner.h"

CCustomTempFScanner::CCustomTempFScanner( CCleanEngine * pEngine, INotifier * pNotifier /* = NULL */, IScanProfiler * pProfiler /* = NULL */ ) :
    CFileScanner( pEngine, pNotifier, pProfiler ),
	CCleanScanner( pEngine, pNotifier, pProfiler ),
    m_pfnAlgoSelector( NULL )
{
}

CCustomTempFScanner::~CCustomTempFScanner()
{
}

void CCustomTempFScanner::ReinitAlgorithm()
{
    m_pfnAlgoSelector = &CCustomTempFScanner::algoStep1;
}

void CCustomTempFScanner::algorithm()
{
    ( this->*m_pfnAlgoSelector )();
}

void CCustomTempFScanner::algoStep1()
{
#ifdef _DEBUG
    //MessageBox( NULL, "CCustomTempFScanner::algoStep1()", NULL, MB_OK );
#endif
    m_pfnAlgoSelector = &CCustomTempFScanner::algoStep2;
}

void CCustomTempFScanner::algoStep2()
{
#ifdef _DEBUG
    //MessageBox( NULL, "CCustomTempFScanner::algoStep2()", NULL, MB_OK );
#endif
    m_pfnAlgoSelector = &CCustomTempFScanner::algoStep3;
}

void CCustomTempFScanner::algoStep3()
{
#ifdef _DEBUG
    //MessageBox( NULL, "CCustomTempFScanner::algoStep3() : STOP", NULL, MB_OK );
#endif
    StopJustMe();
}
