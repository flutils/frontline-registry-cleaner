//------------------------------------------------------------------------
// File:        Exception.h
// Contents:    
//------------------------------------------------------------------------

#include <exception>
#include <stdexcept>

#define EXCEPTION( a ) throw new \
    CGenException( errno, a, __FILE__, __LINE__ )

#define SAFEEXEC( a ) if( !( a )) throw new \
    CGenException( errno, NULL, __FILE__, __LINE__ )

#define OUTOFRANGE( ) throw new \
    COutOfRangeException( __FILE__, __LINE__ )

/**
* Class Name: CGenException
* See file: 'Exception.cpp' for the implementation of this class
*/
class CGenException : public std::exception
{
    LONG        m_nErrorCode;
    TCHAR*      m_szMessage;
    TCHAR*      m_szWhere;
    UINT        m_uLineNumber;

public:
    CGenException(
        LONG            errcode, 
        const TCHAR*    message, 
        const TCHAR*    where, 
        UINT            line);

    virtual ~CGenException() throw();

    const TCHAR* what() const throw();
    const TCHAR* where() const throw();
    
    LONG code() const throw();
};

/**
* Class Name: COutOfRangeException
*/
class COutOfRangeException : public CGenException
{
public:
    COutOfRangeException( const TCHAR * where, int line ) :
        CGenException( -1, _T( "Out of range" ), where, line ){};
};
