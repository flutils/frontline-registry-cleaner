#include "StdAfx.h"
#include "IndexDatFile.h"
#include <atlfile.h>
#include <vector>
#include <set>

//---------------------------------------------------------------------------
#pragma pack(push, 1)

/*!
\brief Directory information
*/
struct CUC52_DIRECTORY
{
	DWORD           dwFileCount;    //!< Files count in directory
	char            szName[8];      //!< Directory name (8 bytes, no null-terminator)
};
//---------------------------------------------------------------------------

/*!
\brief Index.dat Database header
*/
struct CUC52_HEADER
{
	char            szSignature[28];//!< 0x00 "Client UrlCache MMF Ver 5.2" (null-terminated)
	DWORD           dwImageSize;    //!< 0x1C File size
	DWORD           dwFirstRecord;  //!< 0x20 First record offset
	DWORD           dwUnknown[9];
	DWORD           dwDirCount;     //!< 0x48 Directory count
	//CUC52_DIRECTORY dirs[0];        //!< 0x4C Directories
};
//---------------------------------------------------------------------------

//! Size of smallest block in the database. Every record consists of one ore 
//! more blocks.
DWORD const CUC52_BLOCK_SIZE = 0x80;
//---------------------------------------------------------------------------

/*!
\brief Generic record format
\sa CUC52_HASHRECORD CUC52_URLRECORD 
*/
struct CUC52_RECORD
{
	union
	{
		char            szTag[4];   //!< "URL ", "REDR", "HASH" and so on
		DWORD           dwTag;
	};
	DWORD           dwSize;         //!< Record size in blocks
};
//---------------------------------------------------------------------------

DWORD const CUC52_RECORD_NOTHING = 0x00000000;    // "nothing"
// empty record
DWORD const CUC52_RECORD_EMPTY   = 0x0BADF00D;    // "empty"
//! HASH record signature
DWORD const CUC52_RECORD_HASH    = 0x48534148;    // "HASH"
//! URL record signature
DWORD const CUC52_RECORD_URL     = 0x204C5255;    // "URL "
//! REDR record signature
DWORD const CUC52_RECORD_REDR    = 0x52444552;    // "REDR"
//! LEAK record signature
DWORD const CUC52_RECORD_LEAK    = 0x4B41454C;    // "LEAK"
//---------------------------------------------------------------------------

//! Number of hash-pairs in HASH record
DWORD const CUC52_HASHPAIR_COUNT = 448;
//---------------------------------------------------------------------------

//! Uninitialized hash-pair
DWORD const CUC52_HASHPAIR_UNINITIALIZED = 0x00000003;
//---------------------------------------------------------------------------

/*!
\brief Hash-pair record
\sa CUC52_HASHRECORD 
*/
struct CUC52_HASHPAIR
{
	DWORD           dwMagic;        
	DWORD           dwRecord;       //!< Record offset
};
//---------------------------------------------------------------------------

/*!
\brief HASH record.
\sa CUC52_RECORD CUC52_HASHPAIR
*/
struct CUC52_HASHRECORD : public CUC52_RECORD
{
	DWORD           dwNextHashTable;//!< Offset of next hash table
	DWORD           dwUnknown;
	//CUC52_HASHPAIR  data[0];        //!< Hash pairs, 448 usually
};
//---------------------------------------------------------------------------

/*!
\brief URL record.
\sa CUC52_RECORD
*/
struct CUC52_URLRECORD : public CUC52_RECORD
{
	FILETIME        ftLastModifiedTime; //!< Last modified time
	FILETIME        ftLastAccessTime;   //!< Last access time
	FILETIME        ftExpireTime;       //!< Expire time
};

#pragma pack(pop)
//---------------------------------------------------------------------------

// dwRecSize - count of blocks of size CUC52_BLOCK_SIZE
static HRESULT CleanRecord(CAtlFile & file, DWORD dwTag)
{
	DWORD const ARRAY_SIZE = CUC52_BLOCK_SIZE / sizeof(DWORD);
	static DWORD s_arrBlock[ARRAY_SIZE] = { 0 };

	if (0 == s_arrBlock[0])
	{
		// init block with values CUC52_RECORD_EMPTY
		for (DWORD i = 0; i < ARRAY_SIZE; ++i)
			s_arrBlock[i] = CUC52_RECORD_EMPTY;
	}


	CUC52_RECORD rec;
	HRESULT hr = file.Read(&rec, sizeof(rec));

	do
	{
		if (FAILED(hr))
			break;

		if (dwTag != rec.dwTag)
		{
			hr = E_FAIL;
			break;
		}

		LONGLONG lOffset = sizeof(rec);
		hr = file.Seek(-lOffset, FILE_CURRENT);

		for (DWORD i = 0; (i < rec.dwSize) && SUCCEEDED(hr); ++i)
			hr = file.Write(s_arrBlock, sizeof(s_arrBlock));
	}
	while (false);

	return hr;
}

CIndexDatFile::CIndexDatFile(void)
{
}

CIndexDatFile::~CIndexDatFile(void)
{
}

HRESULT CIndexDatFile::Load(LPCTSTR lpFileName)
{
	_m_mapUrls.clear();

	HRESULT hr = _m_file.Create(lpFileName, GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, OPEN_EXISTING);

	do
	{
		if (FAILED(hr))
			break;

		CUC52_HEADER header;
		hr = _m_file.Read(&header, sizeof(header));
		if (FAILED(hr))
			break;

		std::vector<CUC52_DIRECTORY> vecDir;

		if (0 < header.dwDirCount)
		{
			// Тут произошел Crash из-за попытки определить размер вектора больше максимально возможного. 
			// Причиной стало то, что не все файлы с именем index.dat имеют такую иерархическую структуру
			// которую мы пытаемся прочитать => читаем мусор :((

			ULONGLONG nLength;
			_m_file.GetSize( nLength );
			if( header.dwImageSize != nLength )// это не тот index.dat !!!
			{
				hr = E_FAIL;
				break;
			}

			vecDir.resize(header.dwDirCount);
			hr = _m_file.Read(&vecDir[0], header.dwDirCount * sizeof(vecDir[0]));
			if (FAILED(hr))
				break;
		}

		hr = _m_file.Seek(header.dwFirstRecord, FILE_BEGIN);

		while (S_OK == hr)
		{
			CUC52_RECORD rec;
			hr = _m_file.Read(&rec, sizeof(rec));
			if (FAILED(hr))
				break;

			DWORD dwRecSize = rec.dwSize * CUC52_BLOCK_SIZE - sizeof(rec);

			switch (rec.dwTag)
			{
			case CUC52_RECORD_NOTHING:
			case CUC52_RECORD_EMPTY:
				hr = _m_file.Seek(CUC52_BLOCK_SIZE - sizeof(rec), FILE_CURRENT);
				break;
			case CUC52_RECORD_HASH:
				hr = _m_file.Seek(dwRecSize, FILE_CURRENT);
				break;
			case CUC52_RECORD_URL:
				{
					ULONGLONG ullPos = 0;
					_m_file.GetPosition(ullPos);
					ullPos -= sizeof(rec);

					std::vector<char> vecUrlData;
					vecUrlData.resize(dwRecSize);
					hr = _m_file.Read(&vecUrlData[0], dwRecSize);
					if (SUCCEEDED(hr))
					{
						size_t const URL_OFFSET = 96;
						CString strUrl(&vecUrlData[URL_OFFSET]);
						int nPos = strUrl.Find(_T('@'));
						if (0 <= nPos)
						{
							strUrl.Delete(0, nPos + 1);
							strUrl.MakeLower();
							_m_mapUrls.insert(std::make_pair(strUrl, static_cast<DWORD>(ullPos)));
						}
					}
				}
				break;
			case CUC52_RECORD_REDR:
				hr = _m_file.Seek(dwRecSize, FILE_CURRENT);
				break;
			case CUC52_RECORD_LEAK:
				hr = _m_file.Seek(dwRecSize, FILE_CURRENT);
				break;
			default:
				hr = E_FAIL;
			}
		}
	}
	while (false);

	if (ERROR_HANDLE_EOF == HRESULT_CODE(hr))
		hr = S_OK;

	if (FAILED(hr))
		_m_file.Close();

	return hr;
}


void CIndexDatFile::RemoveEmptyRootItems( LPCTSTR lpFileName )
{
	_m_mapUrls.clear();

	if( _m_file.m_h != INVALID_HANDLE_VALUE )
		_m_file.Close();

	HRESULT hr = Load( lpFileName );
	if (FAILED(hr))
		return;

	if( !_m_mapUrls.empty() )
	{
		std::set<CString> setDomains;

		// получаем набор существующих доменов
		map_type::iterator it;
		for( it = _m_mapUrls.begin(); it!=_m_mapUrls.end(); it++ )
		{
			CString str( it->first );
			if( str.Find( _T(":host: ") ) != -1 )
			{
				str.Delete( 0, 7 );
				setDomains.insert( str );
			}
			
		}
		
		//удаляем пустые домены
		std::set<CString>::iterator itSet;
		for( itSet=setDomains.begin(); itSet!=setDomains.end(); itSet++ )
		{
			CString s(*itSet);
			CString strURL;
			bool bEmptyDomain = true;

			for( it = _m_mapUrls.begin(); it!=_m_mapUrls.end(); it++ )
			{
				strURL = it->first;

				if( strURL.Find(_T(":host: ")) != -1  )
					continue;

				CString e(strURL);

				if( strURL.Find( *itSet ) != -1 )
				{
					bEmptyDomain = false;
					break;
				}
			}

			if( bEmptyDomain )
				DeleteUrl( _T(":host: ") + *itSet );
		}

	}
}

HRESULT CIndexDatFile::DeleteUrl(LPCTSTR lpUrl, bool bFlush)
{
	if (NULL == _m_file.m_h)
		return E_FAIL;

	CString strUrl(lpUrl);
	strUrl.MakeLower();
	map_type::iterator it = _m_mapUrls.find(strUrl);
	if (_m_mapUrls.end() == it)
		return E_INVALIDARG;

	HRESULT hr = DeleteRecord(it->second, CUC52_RECORD_URL);
	if (SUCCEEDED(hr) && bFlush)
		_m_file.Flush();

	return hr;
}

HRESULT CIndexDatFile::CleanHistory(void)
{
	if (NULL == _m_file.m_h)
		return E_FAIL;

	for (map_type::iterator it = _m_mapUrls.begin(),
		end = _m_mapUrls.end(); it != end; ++it)
	{
		DeleteRecord(it->second, CUC52_RECORD_URL);
	}

	_m_file.Flush();

	return S_OK;
}

HRESULT CIndexDatFile::DeleteRecord(DWORD dwOffset, DWORD dwTag)
{
	HRESULT hr = _m_file.Seek(dwOffset, FILE_BEGIN);

	if (SUCCEEDED(hr))
		hr = CleanRecord(_m_file, dwTag);

	return hr;
}
