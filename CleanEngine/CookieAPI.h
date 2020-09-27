#ifndef COOKIEAPI_H
#define COOKIEAPI_H

#pragma once

#include <windows.h>
#include <atlstr.h>
#include <WinInet.h>
#include <shlobj.h>
#include <atlfile.h>
#include <atlpath.h>
#include <atlutil.h>
#include <atltime.h>
#include <atlcoll.h>
#include <algorithm>
#include <set>
#include <noncopyable.h>

using namespace std;

//==============================================
#define SWAP_LONG(l)                      \
( ( ((l) >> 24) & 0x000000FFL ) |     \
 (   ((l) >>  8) & 0x0000FF00L ) |     \
 (   ((l) <<  8) & 0x00FF0000L ) |     \
 (   ((l) << 24) & 0xFF000000L ) )
 //==============================================

namespace CookieAPI
{
	// NOTE: Do not forget to add shfolder.lib as first item in the
	// Linker -> Input -> Additional dependencies

	// First tries to get cookie from IE, if is failed then tries to get
	// cookie from FireFox, then from Netscape8, then from Mozilla/Netscape7,
	// then from Opera
	// returns true if any of attempts was successful, in the other case returns false
	bool GetCookie(LPCTSTR lpszUrl, LPCTSTR lpszCookieName,
		CString & strCookieData, bool bUnEscape = true);

	// Tries to set cookie to all supported browsers
	// (IE, FireFox, Mozilla, Netscape 7, 8, Opera)
	// returns true if any of attempts was successful, in the other case returns false
	bool SetCookie(LPCTSTR lpszUrl, LPCTSTR lpszCookieName,
		LPCTSTR lpszCookieData, SYSTEMTIME const & stExpires,
		LPCTSTR lpszDomain = NULL, LPCTSTR lpszPath = NULL,
		bool bSecure = false);

	//==================================================================================

	bool IEGetCookie(LPCTSTR lpszUrl, LPCTSTR lpszCookieName, CString & strCookieData);
	bool IESetCookie(LPCTSTR lpszUrl, LPCTSTR lpszCookieName,
		LPCTSTR lpszCookieData, SYSTEMTIME const & stExpires,
		LPCTSTR lpszDomain, LPCTSTR lpszPath, bool bSecure);

	//==================================================================================
	enum FF_TYPE { FIREFOX, NETSCAPE8, MOZILLA };

	struct SRecord
	{
		CStringA  m_strDomain;
		bool      m_bDomain;
		CStringA  m_strPath;
		bool      m_bSecure;
		SYSTEMTIME m_stExpires;
		CStringA  m_strName;
		CStringA  m_strValue;
	};

	class CFFCookiesFile
	{

		// Костыль, за обьяснениями к Б.Страуструпу
		// C1() : arr[] = { , , , } // Forbidden :(
		// friend CMozillaBrowsersScanner;

	public:
		CFFCookiesFile(FF_TYPE ffType);
		~CFFCookiesFile();

		HRESULT LoadFile( CString strProfile = _T(""));
		HRESULT SaveFile( CString strProfile = _T(""));

		bool GetCookie(LPCTSTR lpszUrl, LPCTSTR lpszCookieName,	CString &strCookieData);
		bool SetCookie(LPCTSTR lpszUrl, LPCTSTR lpszCookieName,
			LPCTSTR lpszCookieData, SYSTEMTIME const & stExpires,
			LPCTSTR lpszDomain, LPCTSTR lpszPath, bool bSecure);

		CString const & GetCookiesFilePath(void) const;

		CAtlArray<SRecord> m_arrRecords;

	private:
		FF_TYPE	m_ffType;
		CString m_strCookiesFilePath;
	};

	//================================================================================

	// functions used in WDC scanner 
	char * strstrn(char const * str1, char const * str1end, char const * str2);
	HRESULT GetAppDataPath(CPath & path);
	CString ReadStringFromIni(LPCTSTR lpFileName, LPCTSTR lpSection,
		LPCTSTR lpKeyName, LPCTSTR lpDefault = _T(""));
	CString ReadSectionNamesFromIni(LPCTSTR lpFileName);


	//==============================================================================================
	//============ CMemoryFile  ====================================================================
	namespace mf // для отделения от CMemoryFile в VSControlsLib
	{
		class CMemoryFile
		{
		public:
			CMemoryFile(void); 
			CMemoryFile(CMemoryFile const & other);
			CMemoryFile(DWORD nSize);
			CMemoryFile(LPCVOID lpData, DWORD nDataSize);

			HRESULT Reset(DWORD nSize);
			HRESULT Read(LPVOID pBuffer, DWORD nBufSize);
			HRESULT Read(LPVOID pBuffer, DWORD nBufSize, DWORD & nBytesRead);
			HRESULT Write(LPCVOID pBuffer, DWORD nBufSize, DWORD * pnBytesWritten = NULL);

			DWORD GetSize(void) const;
			LPVOID GetData(void);
			LPCVOID GetData(void) const;
			bool IsEmpty(void) const;
			void MakeEmpty();

			void operator=( CMemoryFile file );

		private:
			DWORD m_nPos;
			CAtlArray<char> m_arrData;
		};
	};
	//===============================================================================================
	//============= COperaDatFile ===================================================================

	class COperaDatFile : private noncopyable
	{
	private:
		typedef INT8	int8;
		typedef INT16	int16;
		typedef INT32	int32;

		typedef UINT8	uint8;
		typedef UINT16	uint16;
		typedef UINT32	uint32;

		typedef uint32	uintapp;

		struct SOperaFileHeader
		{
			uint32	file_version_number;
			uint32	app_version_number;
			// number of bytes in the id tag, presently 1
			uint16	idtag_length;
			// number of bytes in the length part of a record, presently 2
			uint16	length_length;

			SOperaFileHeader(void) :
			file_version_number(0x00001000),
				app_version_number(0x00002000),
				idtag_length(1),
				length_length(2)
			{
			}
		};

		struct SRecord : private noncopyable
		{
		public:
			SRecord(void);
			void Clear(void);;
			HRESULT AddField(uintapp tag, mf::CMemoryFile const & data);
			POSITION FindFirst(uintapp tag) const;
			mf::CMemoryFile &GetNext(POSITION & pos, uintapp tag);
			mf::CMemoryFile const &GetNext(POSITION & pos, uintapp tag) const;

		public:
			CRBMultiMap<uintapp, mf::CMemoryFile> m_Fields;
		};

		struct SCookieRecord : public SRecord
		{
			bool GetCookieValue(CString & strValue) const;
			bool SetCookieValue(LPCTSTR lpszCookieValue);
		};

		struct SPathRecord : public SRecord
		{
			void Clear(void);
			HRESULT AddPath(POSITION & pos);
			SPathRecord &GetPath(POSITION pos);
			void DeletePath(POSITION pos);
			HRESULT AddCookie(POSITION & pos);
			SCookieRecord & GetCookie(POSITION pos);
			void RemoveAllCookies();
			void DeleteCookie(POSITION pos);
			SPathRecord * GetPath(LPCTSTR lpName);
			SCookieRecord *GetCookie(LPCTSTR lpName);

			CAtlList<SPathRecord> m_arrPath;
			CAtlList<SCookieRecord> m_arrCookie;
		};

		struct SDomainRecord : public SPathRecord
		{
			void Clear(void);
			HRESULT AddDomain(POSITION & pos);
			SDomainRecord & GetDomain(POSITION pos);
			void DeleteDomain(POSITION pos);
			SDomainRecord *GetDomain(LPCTSTR lpName);

			CAtlList<SDomainRecord> m_arrDomain;
		};

		SOperaFileHeader m_Header;
		SDomainRecord m_RootDomain;

		uintapp MSB_VALUE(void) const { return 1 << ((m_Header.idtag_length * 8) - 1); }
		uintapp DOMAIN_BEGIN_TAG(void) const { return 0x01; }
		uintapp DOMAIN_END_TAG(void) const { return 0x04 | MSB_VALUE(); }
		uintapp PATH_BEGIN_TAG(void) const { return 0x02; }
		uintapp PATH_END_TAG(void) const { return 0x05 | MSB_VALUE(); }
		uintapp COOKIE_TAG(void) const { return 0x03; }

		template <class File>
			HRESULT ReadInt(File & file, uintapp & t, DWORD length)
		{
			ATLASSERT(sizeof(t) >= length);

			if (sizeof(t) < length)
				return E_INVALIDARG;

			HRESULT hr = S_OK;

			t = 0;
			char * pT = reinterpret_cast<char *>(&t);


			for (DWORD i = length - 1; i >= 0 ; --i)
			{
				hr = file.Read(&pT[i], sizeof(pT[i]));
				if (FAILED(hr) || (0 == i))
					break;
			}

			return hr;
		}

		template <class File>
			HRESULT WriteInt(File & file, uintapp t, DWORD length)
		{
			ATLASSERT(sizeof(t) >= length);

			if (sizeof(t) < length)
				return E_INVALIDARG;

			HRESULT hr = S_OK;

			char const * pT = reinterpret_cast<char const *>(&t);

			for (DWORD i = length - 1; i >= 0 ; --i)
			{
				hr = file.Write(&pT[i], sizeof(pT[i]));
				if (FAILED(hr) || (0 == i))
					break;
			}

			return hr;
		}

		template <class File>
			HRESULT ReadString(File & file, CString & s)
		{
			HRESULT hr = S_OK;

			s.Empty();

			do
			{
				uintapp length = 0;
				hr = LoadLength(file, length);
				if (FAILED(hr))
					break;

				hr = file.Read(s.GetBuffer(length), length);
				s.ReleaseBuffer(length);
				if (FAILED(hr))
					break;
			}
			while (false);

			return hr;
		}

		template <class File>
			HRESULT WriteString(File & file, CString const & s)
		{
			HRESULT hr = S_OK;

			do
			{
				uintapp length = s.GetLength();
				hr = SaveLength(file, length);
				if (FAILED(hr))
					break;

				DWORD dwWrite = 0;
				hr = file.Write(s.GetString(), length, &dwWrite);
				if (FAILED(hr))
					break;

				ATLASSERT(length == dwWrite);
			}
			while (false);

			return hr;
		}

		template <class File>
			HRESULT LoadTag(File & file, uintapp & tag)
		{
			return ReadInt(file, tag, m_Header.idtag_length);
		}

		template <class File>
			HRESULT SaveTag(File & file, uintapp tag)
		{
			return WriteInt(file, tag, m_Header.idtag_length);
		}

		template <class File>
			HRESULT LoadLength(File & file, uintapp & length)
		{
			return ReadInt(file, length, m_Header.length_length);
		}
		template <class File>
			HRESULT SaveLength(File & file, uintapp length)
		{
			return WriteInt(file, length, m_Header.length_length);
		}

		template <class File>
			HRESULT LoadRecordData(File & file, mf::CMemoryFile & mem_file)
		{
			HRESULT hr = S_OK;

			do
			{
				uintapp length = 0;
				hr = LoadLength(file, length);
				if (FAILED(hr))
					break;

				hr = mem_file.Reset(length);
				if (FAILED(hr))
					break;

				if( length > 0 )
					hr = file.Read(mem_file.GetData(), mem_file.GetSize());
			}
			while (false);

			return hr;
		}

		template <class File>
			HRESULT SaveRecordData(File &file, uintapp tag, mf::CMemoryFile mem_file)
		{
			if( tag != -1)		
				SaveTag( file, tag );

			SaveLength(file, mem_file.GetSize() );

			if(mem_file.GetSize() > 0)
				file.Write(mem_file.GetData(), mem_file.GetSize());

			return S_OK;
		}

		template <class File>
			HRESULT LoadRecord(File & file, SRecord & rec)
		{
			HRESULT hr = S_OK;

			rec.Clear();

			while (true)
			{
				uintapp tag = 0;
				hr = LoadTag(file, tag);
				if (FAILED(hr))
				{
					if (ERROR_HANDLE_EOF == HRESULT_CODE(hr))
						hr = S_OK;
					break;
				}

				mf::CMemoryFile mem_file;
				bool bTagOnly = false;

				hr = TagWithData(tag);
				if ( hr == S_FALSE ) 	 // flag only
					bTagOnly = true;

				if (S_OK == hr)
				{
					hr = LoadRecordData(file, mem_file);
					if (FAILED(hr))
						break;
				}

				if( bTagOnly )
					mem_file.MakeEmpty();

				hr = rec.AddField(tag, mem_file); // flag with data

				if (FAILED(hr))
					break;
			}

			return hr;
		}

	public:
		HRESULT LoadHeader(CAtlFile & file);
		HRESULT SaveHeader(CAtlFile & file);
		HRESULT TagWithData(uintapp tag);
		HRESULT LoadCookie(CAtlFile & file, SCookieRecord & rec);
		HRESULT	LoadPath(CAtlFile & file, SPathRecord & rec);
		HRESULT LoadDomain(CAtlFile & file, SDomainRecord & rec);
		HRESULT LoadFile(LPCTSTR lpFileName);
		HRESULT SaveFile(LPCTSTR lpFileName);
		HRESULT SaveDomain(CAtlFile &file, SDomainRecord &recDomain);
		HRESULT SavePath(CAtlFile &file, SPathRecord &recPath );
		HRESULT DeleteCookie( LPCTSTR lpszUrl );
		HRESULT GetCookie(LPCTSTR lpszUrl, LPCTSTR lpszCookieName, CString & strCookieData);
		HRESULT SetCookie(LPCTSTR lpszUrl, LPCTSTR lpszCookieName, LPCTSTR lpszCookieData);

		HRESULT GetCookiesSet(set <CString> &setURLs);
		HRESULT GetInternalCookies( SDomainRecord &recDomain, set <CString> &setURLs, CString strName );
		void    ReverseURL( CString &strURL );
	};

	//==============================================================
	//
	// Getting Mozilla profile name and directory

	struct PROFILE_ITEM
	{
		CString strName;
		CString strPath;
	};

	class CMozillaProfile
	{
	public:
		CMozillaProfile( LPCTSTR filePath );
		BOOL GetProfiles( CAtlArray<PROFILE_ITEM> &arrProfiles);
		BOOL GetProfilesNames(CAtlArray<CString> &arrProfilesNames);

	private:
		BOOL GetNextProfile( WORD &offset, CString &strProfileName, CString &strPath);
		LPCTSTR m_filePath;
		CAtlFile m_file;
	};
};

using namespace CookieAPI;
#endif
