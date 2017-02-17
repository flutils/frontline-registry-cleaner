#include "StdAfx.h"
#include "Exception.h"

CGenException::CGenException(
    LONG            errcode,
    const TCHAR*    message,
    const TCHAR*    where,
    UINT            line ) :
    
    m_szMessage( NULL ), m_szWhere( NULL ), m_nErrorCode( errcode )
{
    if( message )
    {
	    m_szMessage = _tcsdup( message );
    }

    if( where )
    {
	    m_szWhere = new TCHAR[ _tcslen(where)+10 ];

	    if( m_szWhere )
        {
            _stprintf( m_szWhere, _T( "%s:%d" ), where, line );
        }
    }
}

CGenException::~CGenException() throw()
{
    free( m_szMessage );
    delete [] m_szWhere;
}

const TCHAR* CGenException::what() const throw()
{
    return( m_szMessage ? m_szMessage : _tcserror(m_nErrorCode) );
}

const TCHAR* CGenException::where() const throw()
{
    return m_szWhere;
}

LONG CGenException::code() const throw()
{
    return m_nErrorCode;
}
