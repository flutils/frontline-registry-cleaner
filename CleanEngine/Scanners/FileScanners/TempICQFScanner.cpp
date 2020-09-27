#include "StdAfx.h"
#include "TempICQFScanner.h"


/*************************************************************************
* Function Name:        CTempICQFScanner::CTempICQFScanner()
* Function Purpose:     Constructor
*************************************************************************/
CTempICQFScanner::CTempICQFScanner( CCleanEngine * pEngine, INotifier * pNotifier /* = NULL */, IScanProfiler * pProfiler /* = NULL */ ) :
    CFileScanner( pEngine, pNotifier, pProfiler ),
	CCleanScanner( pEngine, pNotifier, pProfiler ),
    m_pfnAlgoSelector( NULL )
{
}

/*************************************************************************
* Function Name:        CTempICQFScanner::CTempICQFScanner()
* Function Purpose:     Destructor
*************************************************************************/
CTempICQFScanner::~CTempICQFScanner()
{
}

/*************************************************************************
* Function Name:        CTempICQFScanner::ReinitAlgorithm()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CTempICQFScanner::ReinitAlgorithm()
{
    m_pfnAlgoSelector = &CTempICQFScanner::algoStep1;
}

/*************************************************************************
* Function Name:        CTempICQFScanner::algorithm()
* Function Purpose:     
* Arguments:            None
* Return value:         None
*************************************************************************/
void CTempICQFScanner::algorithm()
{
    ( this->*m_pfnAlgoSelector )();
}

void CTempICQFScanner::algoStep1()
{
#ifdef _DEBUG
    //MessageBox( NULL, "CTempICQFScanner::algoStep1()", NULL, MB_OK );
#endif
    m_pfnAlgoSelector = &CTempICQFScanner::algoStep2;
}

void CTempICQFScanner::algoStep2()
{
#ifdef _DEBUG
    //MessageBox( NULL, "CTempICQFScanner::algoStep2()", NULL, MB_OK );
#endif
    m_pfnAlgoSelector = &CTempICQFScanner::algoStep3;
}

void CTempICQFScanner::algoStep3()
{
#ifdef _DEBUG
    //MessageBox( NULL, "CTempICQFScanner::algoStep3() : STOP", NULL, MB_OK );
#endif
    StopJustMe();
}
