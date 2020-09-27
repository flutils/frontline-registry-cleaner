#include "StdAfx.h"
#include "GeneralTempFScanner.h"
#include "CleanEngine.h"

/*************************************************************************
* Function Name:        CGeneralTempFScanner::CGeneralTempFScanner()
* Function Purpose:     Constructor
*************************************************************************/
CGeneralTempFScanner::CGeneralTempFScanner( CCleanEngine * pEngine, INotifier * pNotifier /* = NULL */, IScanProfiler * pProfiler /* = NULL */ ) :
    CFileScanner( pEngine, pNotifier, pProfiler ),
	CCleanScanner( pEngine, pNotifier, pProfiler ),
    m_pfnAlgoSelector( NULL ),
    m_vTempList( 0 )
{
    AddTempPath( _T( "%TEMP%" ));
    AddTempPath( _T( "%TMP%"  ));    
}

/*************************************************************************
* Function Name:        CGeneralTempFScanner::~CGeneralTempFScanner()
* Function Purpose:     Destructor
*************************************************************************/
CGeneralTempFScanner::~CGeneralTempFScanner()
{
}

/*************************************************************************
* Function Name:        CGeneralTempFScanner::AddTempPath()
* Function Purpose:     
* Arguments:            Path where temporary files located. Path may be
*                       expandable string (i.e. wrapped by %%)
*
* Return Value:         True if path successfully added; false if not
*                       ( e.g. such path already exists in list ).
*************************************************************************/
bool CGeneralTempFScanner::AddTempPath( const TCHAR * szTempPath )
{
    const UINT INFO_BUFFER_SIZE = 32768;
    TCHAR infoBuf[ INFO_BUFFER_SIZE ];

    DWORD bufCharCount = ExpandEnvironmentStrings(  szTempPath, 
                                                    infoBuf, 
                                                    INFO_BUFFER_SIZE );

    if(( bufCharCount > INFO_BUFFER_SIZE ) || ( !bufCharCount ))
    {
        return false;
    }

    if( IsAdded( infoBuf ))
    {
        return false;
    }

    countable< ce_tstring > spTempPath( new ce_tstring( infoBuf ));

    if( !spTempPath.get() )
    {
        return false;
    }

    m_vTempList.push_back( spTempPath );
    return true;
}

/*************************************************************************
* Function Name:        CGeneralTempFScanner::IsAdded()
* Function Purpose:     Checks is such path is already added to list.
*
* Arguments:            Path where temporary files located. Must be full 
*                       path, not expandable variable.
*
* Return Value:         True if path is already added to list,
*                       false if not.
*************************************************************************/
bool CGeneralTempFScanner::IsAdded( const TCHAR * szTempPath )
{
    std::vector< countable< ce_tstring > >::iterator it;
    it = m_vTempList.begin();

    for( ; it != m_vTempList.end(); it++ )
    {
        if( _tcsicmp( it->get()->GetString(), szTempPath ) == 0 )
        {
            return true;
        }
    }

    return false;
}

/*************************************************************************
* Function Name:        CGeneralTempFScanner::ReinitAlgorithm()
* Function Purpose:       
* Function Description: 
*************************************************************************/
void CGeneralTempFScanner::ReinitAlgorithm()
{
    m_pfnAlgoSelector = &CGeneralTempFScanner::algoStep1;
    m_theIterator = m_vTempList.begin();
}

/*************************************************************************
* Function Name:        CGeneralTempFScanner::algorithm()
* Function Purpose:       
* Function Description: 
*************************************************************************/
void CGeneralTempFScanner::algorithm()
{
    ( this->*m_pfnAlgoSelector )();
}

/*************************************************************************
* Function Name:        CGeneralTempFScanner::postop()
* Function Purpose:       
* Function Description: 
*************************************************************************/
void CGeneralTempFScanner::postop()
{
    CFileScanner::postop();
}

/*************************************************************************
* Function Name:        CGeneralTempFScanner::algoStep1()
* Function Purpose:     
*
* Arguments:            None
* Return value:         None
*************************************************************************/
void CGeneralTempFScanner::algoStep1()
{
    if( m_theIterator != m_vTempList.end() )
    {
        //
        // Notify user about current scanning object
        //
        if( m_pNotifier )
        {
            m_pNotifier->CallOnScan( 
                ScannerId(), m_theIterator->get()->GetString() );
        }

        m_pfnAlgoSelector = &CGeneralTempFScanner::algoStep2;
        return;
    }

    StopJustMe();
}

/*************************************************************************
* Function Name:        CGeneralTempFScanner::algoStep2()
* Function Purpose:     
*
* Arguments:            None
* Return value:         None
*************************************************************************/
void CGeneralTempFScanner::algoStep2()
{
    WIN32_FIND_DATA sFD;
    HANDLE hTempSearch;

	int size = m_theIterator->get()->GetLength();
	*m_theIterator->get() += _T( "\\*.*" );
    
    hTempSearch = FindFirstFile( m_theIterator->get()->GetString(), &sFD );
	m_theIterator->get()->GetBufferSetLength( size );

	if( INVALID_HANDLE_VALUE == hTempSearch )
	{
        m_theIterator++;
        m_pfnAlgoSelector = &CGeneralTempFScanner::algoStep1;

		return;
	}
    
    m_theIterator->get()->GetBufferSetLength( size );

    do
    {
        //
        // TODO: Implement FileScanner configuration that 
        // it can be abble to skip separate files or file types 
        // ( such as desktop.ini etc. ) Does this need :-?
        //
        
        //
        // TODO: Possible, files selection by last modification 
        // time needs, e.g. to protect Visual Studio temp files.
        //

        if( /*
            !( sFD.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN ) && 
            !( sFD.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM ) && 
            */
            !( sFD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ))
        {
			m_pEngine->AddItem(
				countable<IReportItem>(
					new SFileReportItem(
						fn::bind1st1( fn::mem_fun( &CFileScanner::CleanupObject ), this ),
						ScannerId(),
						IT_FILE,
						SEVERITY_MEDIUM,
						STATUS_FOUND,
						sFD.cFileName,
						m_theIterator->get()->GetString() ) ) );
        }
        else
        {
            if( (_tcscmp( _T( "."  ), sFD.cFileName ) != 0 ) && 
                (_tcscmp( _T( ".." ), sFD.cFileName ) != 0 ))
            {
                int size = m_theIterator->get()->GetLength();
                *m_theIterator->get() += _T( "\\" );
                *m_theIterator->get() += sFD.cFileName;
                RecursivelyAddDirectory( *m_theIterator->get() );
                m_theIterator->get()->GetBufferSetLength( size );
            }
        }
    }
    while( FindNextFile( hTempSearch, &sFD ));

    FindClose( hTempSearch );
    m_theIterator++;

    m_pfnAlgoSelector = &CGeneralTempFScanner::algoStep1;
}
