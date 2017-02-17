#pragma once

#include <map>

const LPSTR g_szWords		= "words";
const LPSTR g_szFiles		= "files";
const LPSTR g_szDirectories	= "directories";
const LPSTR g_szKeys		= "keys";
const LPSTR g_szSubkeys		= "subkeys";

typedef std::multimap< CStringA , CStringA > SkipMap;

#define CHECK_ITEM_IN_SECTION( str_item, str_section ){\
	std::pair <SkipMap::const_iterator, SkipMap::const_iterator> p1;\
	p1 = m_SkipMap.equal_range( str_section );\
	SkipMap::const_iterator it = p1.first;\
	for( ; it != p1.second; ++it )\
	{\
		if( 0 == str_item.CompareNoCase( it->second ) )\
			return TRUE;\
	}\
}

class CSkipListParser
{

public:

	HRESULT LoadBase( const CString & strWhiteListPath )
	{
		HRESULT hr = E_FAIL;

		m_SkipMap.clear();

		HANDLE hFile = CreateFile( strWhiteListPath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0 );
		if( INVALID_HANDLE_VALUE != hFile )
		{
			int iSize = static_cast<int>( GetFileSize( hFile, 0 ) );

			if( iSize > 0 )
			{
				//Only ANSI base
				CStringA strBuf;

				LPSTR pBuf = strBuf.GetBufferSetLength( iSize );
				if( pBuf )
				{
					//dexor
					DWORD dw = 0;
					if( ReadFile( hFile, pBuf, iSize, &dw, 0 ) )
					{
						LPSTR szDecryptKey	= "qwerty_key";
						INT iKeyLen		= lstrlen( szDecryptKey );

						for ( int i = 0; i < iSize; ++i )
							pBuf[ i ] ^= szDecryptKey[ i % iKeyLen ];

						hr = DoParse( strBuf );

					}

					strBuf.ReleaseBuffer();
				}
			}

			CloseHandle( hFile );
		}

		return hr;
	}

	HRESULT SaveBase( const CString & strWhiteListPath )
	{
		HRESULT hr = E_FAIL;

		HANDLE hFile = CreateFile( strWhiteListPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, 0 );
		if( INVALID_HANDLE_VALUE != hFile )
		{
			LPSTR szDecryptKey	= "qwerty_key";
			INT iKeyLen			= lstrlen( szDecryptKey );

			CStringA strUnparsed;
			DWORD dwDummy;

			hr = DoUnparse( strUnparsed );

			int iLen = strUnparsed.GetLength();

			if( S_FALSE != hr )
			{
				LPSTR szBuf = strUnparsed.GetBuffer();

				for ( int i = 0; i < iLen; ++i )
					szBuf[ i ] ^= szDecryptKey[ i % iKeyLen ];

				strUnparsed.ReleaseBuffer();

				hr = WriteFile( hFile, strUnparsed.GetString(), iLen, &dwDummy, 0 ) ?
						S_OK :
						S_FALSE;
			}

			CloseHandle( hFile );
		}

		return hr;
	}

private:

	HRESULT DoParse( CStringA & strBuf )
	{
		try
		{
			char cSectionToke = (char)0xff;
			CStringA strSectionToke( cSectionToke );
			CStringA strTokens( "\n\r" );

			CStringA strSection;
			CStringA strSectionName;
			CStringA strSectionValue;

			int iPosSection = 0;

			while( -1 != iPosSection )
			{
				strSection = strBuf.Tokenize( strSectionToke, iPosSection );

				if( strSection.IsEmpty() )
					continue;

				int iPosInner = 0;
				strSectionName = strSection.Tokenize( strTokens, iPosInner );
				while( -1 != iPosInner )
				{
					strSectionValue = strSection.Tokenize( strTokens, iPosInner );

					strSectionValue.Trim();

					if( strSectionValue.IsEmpty() )
						continue;

					strSectionName.MakeLower();
					strSectionValue.MakeLower();

					m_SkipMap.insert( std::make_pair( strSectionName, strSectionValue ) );
				}
			}
		}
		catch( ... )
		{
			m_SkipMap.clear();

			return S_FALSE;
		}

		return S_OK;
	}

	HRESULT DoUnparse( CStringA & strUnparsed )
	{
		try
		{
			CStringA strFF( (char)0xff );
			CStringA strSection;
			CStringA strSectionName;

			std::multimap< CStringA , CStringA >::const_iterator it = m_SkipMap.begin();
			std::multimap< CStringA , CStringA >::const_iterator itEnd = m_SkipMap.end();

			while( it != itEnd )
			{
				if( it->first.IsEmpty() )
					continue;

				if( strSectionName != it->first )
				{
					strSection += strFF;

					strUnparsed += strSection;

					strSectionName = it->first;

					strSection = strSectionName;
					strSection += '\n';

					strSection += it->second;
					strSection += '\n';
				}
				else
				{
					strSection += it->second;
					strSection += '\n';
				}

				++it;
			}

			strSection += strFF;
			strUnparsed += strSection;
		}
		catch( ... )
		{
			return S_FALSE;
		}

		return S_OK;
	}

protected:

	std::multimap< CStringA , CStringA > m_SkipMap;

};

/******************************************************************************************************
 IWordsWhiteList - Files and Directories skip list interface
******************************************************************************************************/
template<class TImpl>
interface IFilesWhiteList
{

	static IFilesWhiteList * Instance()
	{
		static TImpl * pWListObj = TImpl::VeritableInstance();

		if( IFilesWhiteList * pTrueWListObj = dynamic_cast<IFilesWhiteList*>( pWListObj ) )
			return pTrueWListObj;

		return NULL;
	}

	static BOOL Init( const CString & strWhiteListPath ) { return ( Instance()->DoInit( strWhiteListPath ) == S_OK ? TRUE : FALSE ); }

	virtual BOOL IsFileWhite( const CString & strFileName ) = 0;
	virtual BOOL IsDirectoryWhite( const CString & strDirectoryName ) = 0;
	virtual BOOL IsFObjectRecursivelyWhite( const CString & strFileName, const CString & strDirectoryName ) = 0;

private:

	virtual HRESULT DoInit( const CString & strWhiteListPath ) = 0;

	friend TImpl;

	IFilesWhiteList (void)
	{
	}

};

/******************************************************************************************************
 IWordsWhiteList - RegKeys skip list interface
******************************************************************************************************/
template<class TImpl>
interface IRegKeysWhiteList
{

	static IRegKeysWhiteList * Instance()
	{
		static TImpl * pWListObj = TImpl::VeritableInstance();

		if( IRegKeysWhiteList * pTrueWListObj = dynamic_cast<IRegKeysWhiteList*>( pWListObj ) )
			return pTrueWListObj;

		return NULL;
	}

	static BOOL Init( const CString & strWhiteListPath ) { return ( Instance()->DoInit( strWhiteListPath ) == S_OK ? TRUE : FALSE ); }

	virtual BOOL IsSubkeyWhite( const CString & strSubkeyName ) = 0;
	virtual BOOL IsKeyWhite( const CString & strKeyName ) = 0;
	virtual BOOL IsRObjectRecursivelyWhite( const CString & strSubkeyName, const CString & strKeyName ) = 0;

private:

	virtual HRESULT DoInit( const CString & strWhiteListPath ) = 0;

	friend TImpl;

	IRegKeysWhiteList (void)
	{
	}

};

/******************************************************************************************************
 IWordsWhiteList - Words skip list interface
******************************************************************************************************/
template<class TImpl>
interface IWordsWhiteList
{

	static IWordsWhiteList * Instance()
	{
		static TImpl * pWListObj = TImpl::VeritableInstance();

		if( IWordsWhiteList * pTrueWListObj = dynamic_cast<IWordsWhiteList*>( pWListObj ) )
			return pTrueWListObj;

		return NULL;
	}

	static BOOL Init( const CString & strWhiteListPath ) { return ( Instance()->DoInit( strWhiteListPath ) == S_OK ? TRUE : FALSE ); }

	virtual BOOL IsStringWhite( const CString & strObjName ) = 0;

private:

	virtual HRESULT DoInit( const CString & strWhiteListPath ) = 0;

	friend TImpl;

	IWordsWhiteList (void)
	{
	}

};

/******************************************************************************************************
 IWhiteListEditor - Words skip list interface
******************************************************************************************************/
template<class TImpl>
interface IWhiteListEditor
{

	static IWhiteListEditor * Instance()
	{
		static TImpl * pWListObj = TImpl::VeritableInstance();

		if( IWhiteListEditor * pTrueWListObj = dynamic_cast<IWhiteListEditor*>( pWListObj ) )
			return pTrueWListObj;

		return NULL;
	}

	static BOOL Init( const CString & strWhiteListPath ) { return ( Instance()->DoInit( strWhiteListPath ) == S_OK ? TRUE : FALSE ); }

	virtual BOOL SaveBase( const CString & strWhiteListPath ) = 0;

	virtual std::multimap< CStringA , CStringA > & GetSkipMap() = 0;
	virtual void AppendSkipMap( const std::multimap< CStringA , CStringA > & map ) = 0;

private:

	virtual HRESULT DoInit( const CString & strWhiteListPath ) = 0;

	friend TImpl;

	IWhiteListEditor (void)
	{
	}

};

/******************************************************************************************************
 CWhiteList - Skip list implementation
******************************************************************************************************/
class CWhiteList :
	public IFilesWhiteList<CWhiteList>,
	public IRegKeysWhiteList<CWhiteList>,
	public IWordsWhiteList<CWhiteList>,
	public IWhiteListEditor<CWhiteList>,
	protected CSkipListParser
{

	friend IFilesWhiteList<CWhiteList>;
	friend IRegKeysWhiteList<CWhiteList>;
	friend IWordsWhiteList<CWhiteList>;
	friend IWhiteListEditor<CWhiteList>;

private:

	virtual BOOL IsFileWhite( const CString & strFileName )
	{
		if( !m_bReadyForUse )
			return FALSE;

		CHECK_ITEM_IN_SECTION( strFileName, g_szFiles );

		return FALSE;
	}

	virtual BOOL IsDirectoryWhite( const CString & strDirectoryName )
	{
		if( !m_bReadyForUse )
			return FALSE;

		CHECK_ITEM_IN_SECTION( strDirectoryName, g_szDirectories );

		return FALSE;
	}

	virtual BOOL IsFObjectRecursivelyWhite( const CString & strFileName, const CString & strDirectoryName )
	{
		if( !m_bReadyForUse )
			return FALSE;

		// ToDo: implement it later...

		return FALSE;
	}

	// !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!!
	// !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!!

	virtual BOOL IsSubkeyWhite( const CString & strSubkeyName )
	{
		if( !m_bReadyForUse )
			return FALSE;

		CHECK_ITEM_IN_SECTION( strSubkeyName, g_szSubkeys );

		return FALSE;
	}

	virtual BOOL IsKeyWhite( const CString & strKeyName )
	{
		if( !m_bReadyForUse )
			return FALSE;

		CHECK_ITEM_IN_SECTION( strKeyName, g_szKeys );

		return FALSE;
	}

	virtual BOOL IsRObjectRecursivelyWhite( const CString & strSubkeyName, const CString & strKeyName )
	{
		if( !m_bReadyForUse )
			return FALSE;

		// ToDo: implement it later...

		return FALSE;
	}

	// !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!!
	// !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!!

	virtual BOOL IsStringWhite( const CString & strObjName )
	{
		if( !m_bReadyForUse )
			return FALSE;

		CString strToFind( strObjName );

		strToFind.MakeLower();

		std::pair <SkipMap::const_iterator, SkipMap::const_iterator> p1;
		p1 = m_SkipMap.equal_range( g_szWords );

		SkipMap::const_iterator it = p1.first;

		for( ; it != p1.second; ++it )
		{
			if( -1 != strToFind.Find( it->second ) )
				return TRUE;
		}

		return FALSE;
	}

	// !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!!
	// !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!! !!!

	virtual BOOL SaveBase( const CString & strWhiteListPath )
	{
		return ( CSkipListParser::SaveBase( strWhiteListPath ) == S_OK ? TRUE : FALSE );
	}

	virtual std::multimap< CStringA , CStringA > & GetSkipMap()
	{
		return m_SkipMap;
	}

	virtual void AppendSkipMap( const std::multimap< CStringA , CStringA > & map )
	{
		m_SkipMap = map;
	}

private:

	virtual HRESULT DoInit( const CString & strWhiteListPath )
	{
		if( S_OK != LoadBase( strWhiteListPath ) )
		{
			m_bReadyForUse = FALSE;
			return S_FALSE;
		}

		m_bReadyForUse = TRUE;
		return S_OK;
	}

	static CWhiteList * VeritableInstance()
	{
		static CWhiteList WListObj;

		return &WListObj;
	}

	CWhiteList() : m_bReadyForUse( FALSE )
	{
	}

private:

	BOOL m_bReadyForUse;

};

#define FilesWhiteList IFilesWhiteList<CWhiteList>::Instance()
#define RegKeysWhiteList IRegKeysWhiteList<CWhiteList>::Instance()
#define WordsWhiteList IWordsWhiteList<CWhiteList>::Instance()
#define WhiteListEditor IWhiteListEditor<CWhiteList>::Instance()
