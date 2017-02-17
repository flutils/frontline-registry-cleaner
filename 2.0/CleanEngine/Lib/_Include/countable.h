#pragma once

#include <assert.h>

template < typename T > class countable 
{
    T    * m_pObj;  // storage object pointer
    long * m_pRef;  // reference counter

public:
    typedef T element_type;
	
    //
    // Default constructor
    //
    explicit countable( T * p = 0 ) throw() : m_pObj( p ), m_pRef( NULL )
    {
        m_pRef = new long( 0L );
        increment();
    }
	
    //
    // Copying constructor
    //
    countable( const countable< T >& Y ) throw() : 
        m_pObj( Y.m_pObj ), 
        m_pRef( Y.m_pRef )
    {
        increment();
    }
    
    //
    // Assignment operators
    //
    countable< T >& operator=( const countable< T >& Y ) throw()
    {
        if( this != &Y )
        {
            if( m_pObj != Y.get() )
            {
				decrement();

				m_pObj = Y.get();
				m_pRef = Y.m_pRef;

				increment();
            }
            else if( m_pRef != Y.m_pRef )
            {
                // Usage counter (i.e. value of *m_pRef) must be equal to 1 always here;
                // otherwise we have unsolvable problem caused by incorrect countable usage.
                //
                assert( 1 == *m_pRef );
                if( 1 == *m_pRef ) delete m_pRef;
                m_pRef = Y.m_pRef;
                increment();
            }
        }

        return( *this );
    }
	
    countable< T >& operator=( typename T * pT ) throw()
    {
        if( m_pObj != pT )
        {
            decrement();
            m_pRef = new long( 0L );
            m_pObj = pT;
            increment();
        }

        return( *this );
    }

    // Destructor
    //
    virtual ~countable()
    {
        decrement();
    }
	
    T& operator*() const throw()
    {
        return( *get() );
    }

    T *operator->() const throw()
    {
        return( get() );
    }

    T *get() const throw() 
    {
        return m_pObj;
    }

    long increment()
    {
        return ++( *m_pRef );
    }

    long decrement()
    {
        if( 0 != --( *m_pRef )) return( *m_pRef );

        // Do something useful when counter reaches 0 (e.g. destroy object)
        //
        delete m_pRef;
        delete m_pObj;

        return 0;
    }
};
