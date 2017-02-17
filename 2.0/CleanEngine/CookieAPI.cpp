#include "stdafx.h"
#include "CookieAPI.h"

#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "shfolder.lib")

namespace CookieAPI
{
	//namespace
	//{

	/* ============================= IE =============================*/
	CString IEFormatExpiresTime(SYSTEMTIME const & stExpires)
	{
		LCID lcid = MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT);

		TCHAR lpDate[24] = { _T('\0') };
		::GetDateFormat(lcid, LOCALE_USE_CP_ACP, &stExpires,
			_T("ddd', 'dd'-'MMM'-'yyyy"), lpDate, __countof(lpDate));

		TCHAR lpTime[24] = { _T('\0') };
		::GetTimeFormat(lcid, LOCALE_USE_CP_ACP, &stExpires,
			_T("HH':'mm':'ss"), lpTime, __countof(lpTime));

		CString strResult;
		strResult.Format(_T("%s %s GMT"), lpDate, lpTime);

		return strResult;
	}

	bool GetValueData(LPCTSTR lpszCookieName,
		CString const & strCookieData, CString & strValueData)
	{
		bool bResult = false;

		int nPos = 0;
		LPCTSTR lpDelims = _T(";");

		strValueData = strCookieData.Tokenize(lpDelims, nPos);

		while (!strValueData.IsEmpty())
		{
			// strToken consist "name = value"
			int nEquPos = strValueData.Find(_T('='));
			if (0 <= nEquPos)
			{
				CString strValName = strValueData.Left(nEquPos);
				strValName.Trim();
				if (0 == strValName.CompareNoCase(lpszCookieName))
				{
					strValueData.Delete(0, nEquPos + 1);

					// strValueData now consist "value"
					strValueData.Trim();

					bResult = true;
					break;
				}
			}

			strValueData = strCookieData.Tokenize(lpDelims, nPos);
		}

		return bResult;
	}

	bool IEGetCookie(LPCTSTR lpszUrl, LPCTSTR lpszCookieName, CString & strCookieData)
	{
		bool bResult = false;

		CString strTmp;
		DWORD dwSize = 256;

		while (true)
		{
			BOOL b = ::InternetGetCookie(lpszUrl, NULL,
				strTmp.GetBuffer(dwSize), &dwSize);

			if (b)
			{
				// success
				strTmp.ReleaseBuffer();

				bResult = GetValueData(lpszCookieName, strTmp, strCookieData);
				break;
			}
			else
			{
				if (ERROR_INSUFFICIENT_BUFFER != ::GetLastError())
				{
					// error happens
					break;
				}

				// continue with new buffer size
				continue;
			}
		}

		return bResult;
	}

	bool IESetCookie(LPCTSTR lpszUrl, LPCTSTR lpszCookieName,
		LPCTSTR lpszCookieData, SYSTEMTIME const & stExpires,
		LPCTSTR lpszDomain, LPCTSTR lpszPath, bool bSecure)
	{
		CString strTmp;
		strTmp.Format(_T("%s = %s; expires = %s"),
			lpszCookieName, lpszCookieData,
			IEFormatExpiresTime(stExpires));

		if (NULL != lpszDomain)
			strTmp.AppendFormat(_T("; domain=%s"), lpszDomain);

		if (NULL != lpszPath)
			strTmp.AppendFormat(_T("; path=%s"), lpszPath);

		if (bSecure)
			strTmp.Append(_T("; secure"));

		return (FALSE != ::InternetSetCookie(lpszUrl, NULL, strTmp));
	}
	/* ============================= IE =============================*/

	/* ============================= FF =============================*/

	static TCHAR s_lpWWW[] = _T("www.");

	static TCHAR s_lpCookiesTxt[] = _T("cookies.txt");

	bool ReadLine(CAtlFile & file, CStringA & strLine)
	{
		bool bResult = false;
		strLine.Empty();

		CStringA strTmp;

		while (true)
		{
			CHAR ch;
			HRESULT hr = file.Read(&ch, sizeof(ch));
			if (FAILED(hr))
			{
				strLine = strTmp;
				bResult = !strLine.IsEmpty(); 
				break;
			}

			if ('\n' == ch)
			{
				strTmp.Remove('\r');
				strLine = strTmp;
				bResult = true;
				break;
			}

			strTmp.AppendChar(ch);
		}

		return bResult;
	}

	CString ReadSectionNamesFromIni(LPCTSTR lpFileName)
	{
		CString strResult;

		DWORD dwSize = 256;
		DWORD dwLen = 0;

		do
		{
			dwSize *= 2;
			dwLen = ::GetPrivateProfileSectionNames(
				strResult.GetBuffer(dwSize - 1), dwSize, lpFileName);
		}
		while (!(dwLen < (dwSize - 2 * sizeof(TCHAR))));

		strResult.ReleaseBuffer(dwLen);

		return strResult;
	}

	CString ReadStringFromIni(LPCTSTR lpFileName, LPCTSTR lpSection,
		LPCTSTR lpKeyName, LPCTSTR lpDefault)
	{
		CString strResult;
		DWORD dwSize = 128;
		DWORD dwLen = 0;

		do
		{
			dwSize *= 2;
			dwLen = ::GetPrivateProfileString(lpSection, lpKeyName, lpDefault,
				strResult.GetBuffer(dwSize - 1), dwSize, lpFileName);
		}
		while (!(dwLen < (dwSize - 1 * sizeof(TCHAR))));

		strResult.ReleaseBuffer(dwLen);

		return strResult;
	}

	HRESULT GetAppDataPath(CPath & path)
	{
		HRESULT hr = SHGetFolderPath(NULL, CSIDL_APPDATA, NULL,
			SHGFP_TYPE_CURRENT, path.m_strPath.GetBuffer(MAX_PATH));

		path.m_strPath.ReleaseBuffer();

		return hr;
	}

	char * strstrn(char const * str1, char const * str1end, char const * str2)
	{
		char *cp = (char *) str1;
		char *s1, *s2;

		if ( !*str2 )
			return((char *)str1);

		while (cp < str1end)
		{
			s1 = cp;
			s2 = (char *) str2;

			while ( (s1 < str1end) && *s2 && !(*s1-*s2) )
				s1++, s2++;

			if (!*s2)
				return(cp);

			cp++;
		}

		return(NULL);

	}

	bool FFGetDirs(FF_TYPE ffType, CAtlArray<CString> & arrDirs, CString strProfile )
	{
		bool bResult = false;

		arrDirs.RemoveAll();

		do
		{
			CPath path;
			HRESULT hr = GetAppDataPath(path);
			if (FAILED(hr))
				break;

			if (FIREFOX == ffType)
			{
				LPCTSTR lpDirs[] = { _T("Mozilla\\FireFox") };

				for (size_t i = 0; i < __countof(lpDirs); ++i)
				{
					CPath pathAppData = path;
					pathAppData.Append(lpDirs[i]);

					CPath pathProfilesIni = pathAppData;
					pathProfilesIni.Append(_T("profiles.ini"));

					CString strSections = ReadSectionNamesFromIni(pathProfilesIni);
					strSections.Replace(_T('\0'), _T(';'));

					int nPos = 0;
					LPCTSTR lpDelims = _T(";");
					CString strSection = strSections.Tokenize(lpDelims, nPos);

					while (!strSection.IsEmpty())
					{
						CString strProfilePath = ReadStringFromIni(
							pathProfilesIni, strSection, _T("Path"));

						if (!strProfilePath.IsEmpty())
						{
							CString strProfileName = ReadStringFromIni(	pathProfilesIni, strSection, _T("Name"));

							if( strProfile.IsEmpty() || strProfileName.CompareNoCase(strProfile) == 0 )
							{
								strProfilePath.Replace(_T('/'), _T('\\'));

								CPath pathTmp = pathAppData;
								pathTmp.Append(strProfilePath);
								pathTmp.Append( _T("\\cookies.txt") );

								arrDirs.Add(pathTmp);
							}
						}

						strSection = strSections.Tokenize(lpDelims, nPos);
					}
				}

				bResult = !arrDirs.IsEmpty();
			}
			else
				if ( NETSCAPE8 == ffType)
				{
					LPCTSTR lpDirs[] = {
											_T("Netscape\\NSB"),
											_T("Netscape\\Navigator")
										};

					for (size_t i = 0; i < __countof(lpDirs); ++i)
					{
						CPath pathAppData = path;
						pathAppData.Append(lpDirs[i]);

						CPath pathProfilesIni = pathAppData;
						pathProfilesIni.Append(_T("profiles.ini"));

						CString strSections = ReadSectionNamesFromIni(pathProfilesIni);
						strSections.Replace(_T('\0'), _T(';'));

						int nPos = 0;
						LPCTSTR lpDelims = _T(";");
						CString strSection = strSections.Tokenize(lpDelims, nPos);

						while (!strSection.IsEmpty())
						{
							CString strProfilePath = ReadStringFromIni(
								pathProfilesIni, strSection, _T("Path"));

							if (!strProfilePath.IsEmpty())
							{
								CString strProfileName = ReadStringFromIni(	pathProfilesIni, strSection, _T("Name"));

								if( strProfile.IsEmpty() || strProfileName.CompareNoCase(strProfile) == 0 )
								{
									strProfilePath.Replace(_T('/'), _T('\\'));

									CPath pathTmp = pathAppData;
									pathTmp.Append(strProfilePath);
									pathTmp.Append( _T("\\cookies.txt") );

									arrDirs.Add(pathTmp);
								}
							}

							strSection = strSections.Tokenize(lpDelims, nPos);
						}
					}

					bResult = !arrDirs.IsEmpty();
				}
				else if (MOZILLA == ffType)
				{

					CPath path;
					HRESULT hr = GetAppDataPath(path);
					if (FAILED(hr))
						return false;

					CPath pathAppData = path;
					pathAppData.Append(_T("Mozilla"));

					CPath pathRegistryDat = pathAppData;
					pathRegistryDat.Append(_T("registry.dat"));

					CAtlArray<PROFILE_ITEM> arrProfiles;
					CMozillaProfile profiles( pathRegistryDat.m_strPath );
					profiles.GetProfiles( arrProfiles );

					arrDirs.RemoveAll();
					for( size_t i=0; i<arrProfiles.GetCount(); i++ )
					{
						if( strProfile.IsEmpty() || 
							arrProfiles.GetAt(i).strName.CompareNoCase(strProfile) == 0  )
						{
							arrDirs.Add( arrProfiles.GetAt(i).strPath +_T("\\cookies.txt") );
						}
					}

					bResult = !arrDirs.IsEmpty();

				}
				else
				{
					ATLASSERT(false);
				}
		}
		while (false);

		return bResult;
	}

	bool FFMakeCookieLine( CString &strLine, CString const &strUrl,
		CString const &strName, CString const &strValue, SYSTEMTIME const &stExpires, 
		bool const &bDomain, bool const &bSecure, CString const &strPath)
	{
		CTime time( stExpires );

		strLine.Format("%s\t%s\t%s\t%s\t%u\t%s\t%s\r\n",
			strUrl,
			bDomain ? _T("TRUE") : _T("FALSE"),
			strPath,
			bSecure ? _T("TRUE") : _T("FALSE"),
			(UINT)time.GetTime(),
			strName,
			strValue );

		return true;
	}

	bool FFSplitLine(CString const & strLine, CString & strUrl,
		CString & strName, CString & strValue, SYSTEMTIME &stExpires, 
		bool &bDomain, bool &bSecure, CString &strPath)
	{
		int nPos = 0;
		LPCTSTR lpDelims = _T("\t");

		strName.Empty();
		strValue.Empty();
		strName.Empty();
		strValue.Empty();
		strPath.Empty();

		// get url ===================================
		strUrl = strLine.Tokenize(lpDelims, nPos);
		if (0 > nPos)
			return false;

		// get domain ================================
		CString strBool = strLine.Tokenize(lpDelims, nPos);
		(strBool.Find(_T("TRUE")) != -1) ? bDomain = true : bDomain = false;
		if (0 > nPos)
			return false;

		// get path ==================================
		strPath = strLine.Tokenize(lpDelims, nPos);
		if (0 > nPos)
			return false;

		// get secure ================================
		strBool = strLine.Tokenize(lpDelims, nPos);
		if (0 > nPos)
			return false;
		(strBool.Find(_T("TRUE")) != -1) ? bSecure = true : bSecure = false;

		// get expire time ===========================
		CString strTime = strLine.Tokenize(lpDelims, nPos);
		if (0 > nPos)
			return false;
		CTime cookieTime(/* (time_t)*/_tcstoul(strTime, NULL, 10) );
		cookieTime.GetAsSystemTime( stExpires );	

		// get name ==================================
		strName = strLine.Tokenize(lpDelims, nPos);
		if (0 > nPos)
			return false;

		// get value =================================
		strValue = strLine.Tokenize(lpDelims, nPos);

		return true;
	}

	HRESULT CFFCookiesFile::LoadFile( CString strProfile )
	{
		HRESULT hr = S_OK;

		CAtlArray<CString> arrDirs;
		bool bResult = FFGetDirs(m_ffType, arrDirs, strProfile);
		if (!bResult)
			return E_FAIL;

		// m_strCookiesFilePath used only for getting file path 
		// during cookies searching by profile(to show location in report)
		m_strCookiesFilePath = arrDirs.GetAt(0);


		for(size_t i=0; i< arrDirs.GetCount(); i++ )
		{
			CAtlFile file;
			HRESULT hr = file.Create(arrDirs[i], GENERIC_READ,
				FILE_SHARE_READ, OPEN_EXISTING);
			if (FAILED(hr))
				break;

			while (true)
			{
				CStringA strLine;
				if (!ReadLine(file, strLine))
					break;

				if (strLine.IsEmpty())
					continue;

				if (_T('#') == strLine[0])
					continue;

				SRecord record;
				FFSplitLine( strLine,	
					record.m_strDomain, 
					record.m_strName, 
					record.m_strValue, 
					record.m_stExpires, 
					record.m_bDomain, 
					record.m_bSecure,
					record.m_strPath );

				m_arrRecords.Add( record );
			}
			file.Close();

		}

		return hr;
	}

	CString const & CFFCookiesFile::GetCookiesFilePath(void) const
	{
		return m_strCookiesFilePath;
	}

	HRESULT CFFCookiesFile::SaveFile( CString strProfile )
	{
		CString  strLine;
		SRecord  record;
		DWORD    dwWritten = 0;
		CAtlFile file;

		CAtlArray<CString> arrDirs;
		BOOL bResult = FFGetDirs(m_ffType, arrDirs, strProfile );
		if (!bResult)
			return E_FAIL;

		HRESULT hr;
		for(size_t i=0; i<arrDirs.GetCount(); i++ )
		{
			hr = file.Create( arrDirs[i], GENERIC_WRITE,// always write in first path
				FILE_SHARE_WRITE|FILE_SHARE_READ, CREATE_ALWAYS);

			if (FAILED(hr))
				return hr;
		
			// ===== Writing FF Header to cookies.txt ===========
			CString strHeader("# HTTP Cookie File\r\n# http://www.netscape.com/newsref/std/cookie_spec.html\r\n# This is a generated file!  Do not edit.\r\n# To delete cookies, use the Cookie Manager.\r\n\r\n");
			hr = file.Write( strHeader.GetString(), strHeader.GetLength(), &dwWritten);
			if (FAILED(hr))
			{
				file.Close();
				return hr;
			}
			//===================================================

			for(size_t i=0; i<m_arrRecords.GetCount(); i++ )
			{
				record = m_arrRecords[i];
				FFMakeCookieLine( strLine,	
					record.m_strDomain, 
					record.m_strName, 
					record.m_strValue, 
					record.m_stExpires, 
					record.m_bDomain, 
					record.m_bSecure,
					record.m_strPath );

				hr = file.Write( strLine.GetString(), strLine.GetLength(), &dwWritten);
				if (FAILED(hr))
					break;
			}
			file.Close();

		}

		return hr;
	}

	CFFCookiesFile::CFFCookiesFile(FF_TYPE ffType)
	{
		m_ffType = ffType;
	}
	CFFCookiesFile::~CFFCookiesFile()
	{
		m_arrRecords.RemoveAll();
	}

	bool CFFCookiesFile::GetCookie(LPCTSTR lpszUrl, LPCTSTR lpszCookieName,
		CString &strCookieData)
	{
		CUrl url;
		if( !url.CrackUrl(lpszUrl))
			return false;

		CString strReqUrl(lpszUrl);

		for(size_t i=0; i<m_arrRecords.GetCount(); i++)
		{
			CString strUrl;
			CString strName;

			strUrl  = m_arrRecords[i].m_strDomain;
			strName = m_arrRecords[i].m_strName;
			strUrl.Trim();
			strName.Trim();

			if (strUrl.IsEmpty() || strName.IsEmpty())
				continue;

			strReqUrl.Replace( _T("http://"), _T("") );
			CString strRequestedUrl = strReqUrl;

			// check that line correspond to the requested URL and cookie name
			if (_T('.') == strUrl[0])
			{
				// compare url without common prefix

				// delete "www." prefix
				int nWWWPos = strRequestedUrl.Find(s_lpWWW);
				if (-1 != nWWWPos)
					strRequestedUrl.Delete(0, nWWWPos + __countof(s_lpWWW) - 1);

				// delete first '.'
				strUrl.Delete(0, 1);
			}
			else
			{
				// do nothing, compare url as is
			}

			if (0 != strRequestedUrl.CompareNoCase(strUrl))
				continue;

			if (0 != strName.CompareNoCase(lpszCookieName))
				continue;

			strCookieData = m_arrRecords[i].m_strValue;
			// done
			return true;
		}
		return false;
	}


	bool CFFCookiesFile::SetCookie(LPCTSTR lpszUrl, LPCTSTR lpszCookieName,
		LPCTSTR lpszCookieData, SYSTEMTIME const & stExpires,
		LPCTSTR lpszDomain, LPCTSTR lpszPath, bool bSecure)
	{
		CUrl url;
		if( !url.CrackUrl(lpszUrl) )
			return false;

		CString strReqUrl(lpszUrl);

		for(size_t i=0; i<m_arrRecords.GetCount(); i++)
		{
			CString strUrl;
			CString strName;

			strUrl  = m_arrRecords[i].m_strDomain;
			strName = m_arrRecords[i].m_strName;
			strUrl.Trim();
			strName.Trim();

			if (strUrl.IsEmpty() || strName.IsEmpty())
				continue;

			strUrl.TrimRight(_T('/'));
			CString strRequestedUrl = strReqUrl;

			// check that line correspond to the requested URL and cookie name
			if (_T('.') == strUrl[0])
			{
				// compare url without common prefix

				// delete "www." prefix
				int nWWWPos = strRequestedUrl.Find(s_lpWWW);
				if (-1 != nWWWPos)
					strRequestedUrl.Delete(0, nWWWPos + __countof(s_lpWWW) - 1);

				// delete first '.'
				strUrl.Delete(0, 1);
			}
			else
			{
				// do nothing, compare url as is
			}

			if (0 != strRequestedUrl.CompareNoCase(strUrl))
				continue;

			if (0 != strName.CompareNoCase(lpszCookieName))
				continue;

			m_arrRecords[i].m_strValue  = lpszCookieData;
			m_arrRecords[i].m_stExpires = stExpires;
			m_arrRecords[i].m_strPath   = lpszPath;
			m_arrRecords[i].m_strPath   = lpszPath;
			m_arrRecords[i].m_bSecure   = bSecure;
			m_arrRecords[i].m_bDomain   = lstrcmp(_T("TRUE"), lpszDomain) ? true : false;

			// done
			return true;
		}
		return false;
	}


	CString FFFormatCookieLine(LPCTSTR lpUrl, LPCTSTR lpName,
		LPCTSTR lpValue, SYSTEMTIME const & stExpires)
	{
		LPCTSTR lpTrue  = _T("TRUE");
		LPCTSTR lpFalse = _T("FALSE");

		CString strResult;

		do
		{
			CUrl url;
			if (!url.CrackUrl(lpUrl))
				break;

			LPCTSTR lpDomain = url.GetHostName();
			bool bHasCommonPrefix = false;

			{
				CString strDomain(lpDomain);
				strDomain.MakeLower();
				if (0 == strDomain.Find(s_lpWWW))
					bHasCommonPrefix = true;
			}

			CString strPath(url.GetUrlPath());
			if (strPath.IsEmpty())
				strPath.AppendChar(_T('/'));

			strResult.Format(_T("%s\t%s\t%s\t%s\t%lu\t%s\t%s\n"),
				(bHasCommonPrefix ? (lpDomain + __countof(s_lpWWW) - 2) : lpDomain),
				(bHasCommonPrefix ? lpTrue : lpFalse), strPath.GetString(),
				((ATL_URL_SCHEME_HTTPS == url.GetScheme()) ? lpTrue : lpFalse),
				static_cast<unsigned long>(CTime(stExpires).GetTime()),
				lpName, lpValue);
		}
		while (false);

		return strResult;
	}

	bool FFSetCookie(LPCTSTR lpszUrl, LPCTSTR lpszCookieName,
		LPCTSTR lpszCookieData, SYSTEMTIME const & stExpires,
		LPCTSTR lpszDomain, LPCTSTR lpszPath, bool bSecure,
		FF_TYPE ffType)
	{
		CFFCookiesFile ffCookies( ffType );

		HRESULT hr = ffCookies.LoadFile();
		if(FAILED(hr))
			return false;

		bool bRetVal = ffCookies.SetCookie(lpszUrl, lpszCookieName, lpszCookieData, stExpires,
			lpszDomain, lpszPath, bSecure);

		ffCookies.SaveFile();
		return bRetVal;
	}
	/* ============================= FF =============================*/

	/* ============================= Opera =============================*/


	template <typename K, typename V, class KTraits, class VTraits>
		void CopyMap(CRBMultiMap<K, V, KTraits, VTraits> & dst, CRBMultiMap<K, V, KTraits, VTraits> const & src)
	{
		dst.RemoveAll();

		POSITION pos = src.GetHeadPosition();

		while (NULL != pos)
		{
			CRBMultiMap<K, V, KTraits, VTraits>::CPair const * pPair = src.GetNext(pos);
			dst.Insert(pPair->m_key, pPair->m_value);
		}
	}

	bool IsIPAddress(LPCTSTR lpStr)
	{
		bool bResult = true;

		while (true)
		{
			TCHAR c = *lpStr++;

			if (_T('\0') == c)
				break;

			if (((_T('0') > c) || (_T('9') < c)) && (_T('.') != c))
			{
				bResult = false;
				break;
			}
		}

		return bResult;
	}

	void SplitString(LPCTSTR lpStr, LPCTSTR lpDelims,
		CAtlArray<CString> & arrResult, bool bReverse)
	{
		arrResult.RemoveAll();

		int nPos = 0;

		CString str(lpStr);
		CString strToken = str.Tokenize(lpDelims, nPos);

		while (!strToken.IsEmpty())
		{
			if (bReverse)
				arrResult.InsertAt(0, strToken);
			else
				arrResult.Add(strToken);

			strToken = str.Tokenize(lpDelims, nPos);
		}
	}

	//===================================================================================
	//=============  CMemoryFile  =======================================================

	mf::CMemoryFile::CMemoryFile(void) : m_nPos(0)
	{
	}

	mf::CMemoryFile::CMemoryFile(CMemoryFile const & other) : m_nPos(other.m_nPos)
	{
		m_arrData.Copy(other.m_arrData);
	}

	mf::CMemoryFile::CMemoryFile(DWORD nSize) : m_nPos(0)
	{
		if (!m_arrData.SetCount(nSize))
			ATL::AtlThrow(E_OUTOFMEMORY);
	}

	mf::CMemoryFile::CMemoryFile(LPCVOID lpData, DWORD nDataSize) : m_nPos(0)
	{
		if (!m_arrData.SetCount(nDataSize))
			ATL::AtlThrow(E_OUTOFMEMORY);
		::CopyMemory(m_arrData.GetData(), lpData, m_arrData.GetCount());
	}

	HRESULT mf::CMemoryFile::Reset(DWORD nSize)
	{
		m_nPos = 0;
		return m_arrData.SetCount(nSize) ? S_OK : E_OUTOFMEMORY;
	}

	HRESULT mf::CMemoryFile::Read(LPVOID pBuffer, DWORD nBufSize)
	{
		DWORD nBytesRead = 0;
		HRESULT hr = Read(pBuffer, nBufSize, nBytesRead);
		if (SUCCEEDED(hr) && (nBytesRead != nBufSize))
			hr = HRESULT_FROM_WIN32(ERROR_HANDLE_EOF);
		return hr;
	}

	HRESULT mf::CMemoryFile::Read(LPVOID pBuffer, DWORD nBufSize, DWORD & nBytesRead)
	{
		if (nBytesRead = static_cast<DWORD>(_cpp_min(m_arrData.GetCount() - m_nPos, nBufSize)))
		{
			::CopyMemory(pBuffer, &m_arrData[m_nPos], nBytesRead);
			m_nPos += nBytesRead;
			return S_OK;
		}
		return HRESULT_FROM_WIN32(ERROR_HANDLE_EOF);
	}

	HRESULT mf::CMemoryFile::Write(LPCVOID pBuffer, DWORD nBufSize, DWORD * pnBytesWritten )
	{
		DWORD nBytesWritten = m_arrData.SetCount(m_nPos + nBufSize) ?
nBufSize : static_cast<DWORD>(m_arrData.GetCount()) - m_nPos;
		::CopyMemory(&m_arrData[m_nPos], pBuffer, nBytesWritten);
		m_nPos += nBytesWritten;
		if (pnBytesWritten)
			*pnBytesWritten = nBytesWritten;
		return S_OK;
	}
	LPVOID mf::CMemoryFile::GetData(void) { return m_arrData.GetData(); }
	bool mf::CMemoryFile::IsEmpty(void) const { return m_arrData.IsEmpty(); }


	DWORD mf::CMemoryFile::GetSize(void) const { return static_cast<DWORD>(m_arrData.GetCount()); }

	LPCVOID mf::CMemoryFile::GetData(void) const { return m_arrData.GetData(); }

	void mf::CMemoryFile::MakeEmpty(){ m_arrData.RemoveAll(); };

	void mf::CMemoryFile::operator=( CMemoryFile file )
	{
		if( file.GetSize() != 0)
			Write( file.GetData(), file.GetSize() );
		else
			file.MakeEmpty();
	}

	//=======================================================================================
	//=======================================================================================

	HRESULT COperaDatFile::LoadHeader(CAtlFile & file)
	{
		HRESULT hr = S_OK;

		do
		{
			uintapp tmp = 0;
			hr = ReadInt(file, tmp, sizeof(m_Header.file_version_number));
			if (FAILED(hr))
				break;

			m_Header.file_version_number = tmp;

			tmp = 0;
			hr = ReadInt(file, tmp, sizeof(m_Header.app_version_number));
			if (FAILED(hr))
				break;

			m_Header.app_version_number = tmp;

			tmp = 0;
			hr = ReadInt(file, tmp, sizeof(m_Header.idtag_length));
			if (FAILED(hr))
				break;

			m_Header.idtag_length = static_cast<uint16>(tmp);

			tmp = 0;
			hr = ReadInt(file, tmp, sizeof(m_Header.length_length));
			if (FAILED(hr))
				break;

			m_Header.length_length = static_cast<uint16>(tmp);
		}
		while (false);

		return hr;
	}

	HRESULT COperaDatFile::SaveHeader(CAtlFile & file)
	{
		HRESULT hr = S_OK;

		do
		{
			hr = WriteInt(file, m_Header.file_version_number,
				sizeof(m_Header.file_version_number));
			if (FAILED(hr))
				break;

			hr = WriteInt(file, m_Header.app_version_number,
				sizeof(m_Header.app_version_number));
			if (FAILED(hr))
				break;

			hr = WriteInt(file, m_Header.idtag_length,
				sizeof(m_Header.idtag_length));
			if (FAILED(hr))
				break;

			hr = WriteInt(file, m_Header.length_length,
				sizeof(m_Header.length_length));
		}
		while (false);

		return hr;
	}


	HRESULT COperaDatFile::TagWithData(uintapp tag)
	{
		return (tag & MSB_VALUE()) ? S_FALSE : S_OK;
	}

	HRESULT COperaDatFile::LoadCookie(CAtlFile & file, SCookieRecord & rec)
	{
		HRESULT hr = S_OK;

		rec.Clear();

		do
		{
			mf::CMemoryFile mem_file;
			hr = LoadRecordData(file, mem_file);
			if (FAILED(hr))
				break;

			hr = LoadRecord(mem_file, rec);
		}
		while (false);

		return hr;
	}

	HRESULT COperaDatFile::LoadPath(CAtlFile & file, SPathRecord & rec)
	{
		HRESULT hr = S_OK;

		rec.Clear();

		do
		{
			mf::CMemoryFile mem_file;
			hr = LoadRecordData(file, mem_file);
			if (FAILED(hr))
				break;

			hr = LoadRecord(mem_file, rec);

			while (SUCCEEDED(hr))
			{
				uintapp tag = 0;
				hr = LoadTag(file, tag);
				if (FAILED(hr))
					break;

				if (COOKIE_TAG() == tag)
				{

					POSITION pos = NULL;
					hr = rec.AddCookie(pos);
					if (SUCCEEDED(hr))
					{
						hr = LoadCookie(file, rec.GetCookie(pos));
						if (FAILED(hr))
							rec.DeleteCookie(pos);
					}
				}
				else if (PATH_BEGIN_TAG() == tag)// добавлено
				{
					POSITION pos = NULL;
					hr = rec.AddPath(pos);
					if (SUCCEEDED(hr))
					{
						hr = LoadPath(file, rec.GetPath(pos));
						if (FAILED(hr))
							rec.DeletePath(pos);
					}
				}
				else if (PATH_END_TAG() == tag)
				{
					// stop loading
					break;
				}
				else
				{
					// broken file
					hr = E_FAIL;
				}
			}
		}
		while (false);

		return hr;
	}

	HRESULT COperaDatFile::LoadDomain(CAtlFile & file, SDomainRecord & rec)
	{
		HRESULT hr = S_OK;

		rec.Clear();

		do
		{
			mf::CMemoryFile mem_file;
			hr = LoadRecordData(file, mem_file);
			if (FAILED(hr))
				break;

			hr = LoadRecord(mem_file, rec);

			while (SUCCEEDED(hr))
			{
				uintapp tag = 0;
				hr = LoadTag(file, tag);
				if (FAILED(hr))
					break;

				if (DOMAIN_BEGIN_TAG() == tag)
				{
					/*
					SDomainRecord rec_domain;
					hr = LoadDomain(file, rec_domain);
					if (SUCCEEDED(hr))
						hr = rec.AddDomain(rec_domain);
					*/
						POSITION pos = NULL;
					hr = rec.AddDomain(pos);
					if (SUCCEEDED(hr))
					{
						hr = LoadDomain(file, rec.GetDomain(pos));
						if (FAILED(hr))
							rec.DeleteDomain(pos);
					}
				}
				else if (PATH_BEGIN_TAG() == tag)
				{
					/*
					SPathRecord rec_path;
					hr = LoadPath(file, rec_path);
					if (SUCCEEDED(hr))
						hr = rec.AddPath(rec_path);
					*/
						POSITION pos = NULL;
					hr = rec.AddPath(pos);
					if (SUCCEEDED(hr))
					{
						hr = LoadPath(file, rec.GetPath(pos));
						if (FAILED(hr))
							rec.DeletePath(pos);
					}
				}
				else if (COOKIE_TAG() == tag)
				{
					/*
					SCookieRecord rec_cookie;
					hr = LoadCookie(file, rec_cookie);
					if (SUCCEEDED(hr))
						hr = rec.AddCookie(rec_cookie);
					*/
						POSITION pos = NULL;
					hr = rec.AddCookie(pos);
					if (SUCCEEDED(hr))
					{
						hr = LoadCookie(file, rec.GetCookie(pos));
						if (FAILED(hr))
							rec.DeleteCookie(pos);
					}
				}
				else if (PATH_END_TAG() == tag)
				{
					// skip empty path record
					continue;
				}
				else if (DOMAIN_END_TAG() == tag)
				{
					// stop loading
					break;
				}
				else
				{
					// broken file
					hr = E_FAIL;
				}
			}
		}
		while (false);

		return hr;
	}

	HRESULT COperaDatFile::LoadFile(LPCTSTR lpFileName)
	{
		ATLASSERT(NULL != lpFileName);

		m_RootDomain.Clear();

		HRESULT hr = S_OK;

		do
		{
			CAtlFile file;

			hr = file.Create(lpFileName, GENERIC_READ,
				FILE_SHARE_READ, OPEN_EXISTING);
			if (FAILED(hr))
				break;

			hr = LoadHeader(file);

			while (SUCCEEDED(hr))
			{
				uintapp tag = 0;
				hr = LoadTag(file, tag);
				if (FAILED(hr))
				{
					if (HRESULT_CODE(hr) == ERROR_HANDLE_EOF)
						hr = S_OK;
					break;
				}

				if (DOMAIN_BEGIN_TAG() == tag)
				{

					POSITION pos = NULL;
					hr = m_RootDomain.AddDomain(pos);
					if (SUCCEEDED(hr))
					{
						hr = LoadDomain(file, m_RootDomain.GetDomain(pos));
						if (FAILED(hr))
							m_RootDomain.DeleteDomain(pos);
					}
				}
				else if (DOMAIN_END_TAG() == tag)
				{
					// stop loading file
					break;
				}
				else
				{
					// broken file
					hr = E_FAIL;
				}
			}
		}
		while (false);

		return hr;
	}

	HRESULT COperaDatFile::SaveFile(LPCTSTR lpFileName)
	{
		ATLASSERT(NULL != lpFileName);

		HRESULT hr = S_OK;

		CAtlFile file;

		hr = file.Create(lpFileName, GENERIC_WRITE,
			FILE_SHARE_READ, OPEN_ALWAYS);
		if (FAILED(hr))
			return S_FALSE;

		file.SetSize( 0 );

		hr = SaveHeader(file);
		if (FAILED(hr))
			return S_FALSE;

		POSITION pos = NULL;

		// save all pathes ==============================
		pos = m_RootDomain.m_arrPath.GetHeadPosition();
		if( pos )
			while( pos != NULL )
			{
				SPathRecord& pathRecord = m_RootDomain.m_arrPath.GetNext( pos );				 
				hr = SavePath(file, pathRecord );				
			}

			// save all domains =============================
			pos = m_RootDomain.m_arrDomain.GetHeadPosition();
			while( pos != NULL )
			{
				SDomainRecord &domainRecord = m_RootDomain.m_arrDomain.GetNext( pos );				 
				hr = SaveDomain(file, domainRecord );				
			}

			hr = SaveTag(file, DOMAIN_END_TAG());			

			file.Close();
			return hr;
	}

	HRESULT COperaDatFile::SaveDomain(CAtlFile &file, SDomainRecord &recDomain)
	{
		HRESULT hr = S_OK;

		hr = SaveTag(file, DOMAIN_BEGIN_TAG());			

		// save domain name ==========================
		uintapp const DOMAIN_NAME_TAG = 0x1E;
		if( POSITION pos = recDomain.FindFirst(DOMAIN_NAME_TAG))
		{		
			mf::CMemoryFile const &data = recDomain.GetNext(pos, DOMAIN_NAME_TAG);
			mf::CMemoryFile mem_data;
			SaveRecordData( mem_data, DOMAIN_NAME_TAG, data );
			SaveRecordData( file, -1, mem_data );
		}
		else
			return S_FALSE;

		//save all cookies ===========================

		POSITION posCookie;
		posCookie = recDomain.m_arrCookie.GetHeadPosition();
		while (NULL != posCookie)
		{
			SCookieRecord const & cookie = recDomain.m_arrCookie.GetNext( posCookie );

			mf::CMemoryFile mem_data;

			POSITION pos;
			pos = cookie.m_Fields.GetHeadPosition();
			if( pos )
			{
				SaveTag( file, COOKIE_TAG() );
				while( pos != NULL )
				{
					uintapp tag;
					mf::CMemoryFile data;
					cookie.m_Fields.GetNextAssoc( pos, tag, data );
					if( !data.IsEmpty() )
						SaveRecordData( mem_data, tag, data);
					else
						SaveTag( mem_data, tag );// write tag only
				}
				SaveRecordData( file, -1, mem_data );
			}

		}

		//save all pathes ============================
		POSITION pos;
		pos = recDomain.m_arrPath.GetHeadPosition();
		while( pos != NULL )
		{
			SPathRecord &pathRecord = recDomain.m_arrPath.GetNext( pos );
			hr = SavePath(file, pathRecord );				
		}

		SaveTag( file, PATH_END_TAG() ); //this tag mast to be in any case!!!

		//save all domains ===========================
		pos = recDomain.m_arrDomain.GetHeadPosition();				 
		while( pos != NULL )
		{
			SDomainRecord &domainRecord = recDomain.m_arrDomain.GetNext( pos );
			hr = SaveDomain(file, domainRecord );				
		}

		hr = SaveTag(file, DOMAIN_END_TAG());			

		return hr;						
	}

	HRESULT COperaDatFile::SavePath(CAtlFile &file, SPathRecord &recPath )
	{
		HRESULT hr = S_OK;
		SaveTag( file, PATH_BEGIN_TAG() );

		// save path name ==========================
		uintapp const PATH_NAME_TAG = 0x1D;
		if( POSITION pos = recPath.FindFirst(PATH_NAME_TAG))
		{		
			mf::CMemoryFile const &data = recPath.GetNext(pos, PATH_NAME_TAG);
			mf::CMemoryFile mem_data;
			SaveRecordData( mem_data, PATH_NAME_TAG, data );
			SaveRecordData( file, -1, mem_data );
		}
		else
			return S_FALSE;

		//save all cookies ===========================
		POSITION posCookie;
		posCookie = recPath.m_arrCookie.GetHeadPosition();
		while ( posCookie != NULL )
		{
			SCookieRecord const & cookie = recPath.m_arrCookie.GetNext( posCookie );

			mf::CMemoryFile mem_data;

			POSITION pos;
			pos = cookie.m_Fields.GetHeadPosition();
			if( pos )
			{
				SaveTag( file, COOKIE_TAG() );
				while( pos != NULL )
				{
					uintapp tag;
					mf::CMemoryFile data;
					cookie.m_Fields.GetNextAssoc( pos, tag, data );
					if( !data.IsEmpty() )
						SaveRecordData( mem_data, tag, data);
					else
						SaveTag( mem_data, tag );// write tag only
				}

				SaveRecordData( file, -1, mem_data );
			}

		}

		SaveTag( file, PATH_END_TAG() );
		return hr;			
	}

	HRESULT COperaDatFile::DeleteCookie( LPCTSTR lpszUrl )
	{
		// remove cookies array but not whole domain,
		// this domain without cookies Opera will remove itself
		HRESULT hr = E_INVALIDARG;

		do
		{
			CUrl url;
			if (!url.CrackUrl(lpszUrl))
				break;

			SDomainRecord *pDomain = NULL;

			if (IsIPAddress(url.GetHostName()))
			{
				pDomain = m_RootDomain.GetDomain(url.GetHostName());
			}
			else
			{
				CAtlArray<CString> arrDomainParts;
				SplitString(url.GetHostName(), _T("."), arrDomainParts, true);

				SDomainRecord *pCurDomain = NULL;

				for (size_t i = 0, max_i = arrDomainParts.GetCount(); i < max_i; ++i)
				{
					CString &strDomain = arrDomainParts[i];

					SDomainRecord *pTmpDomain = NULL;

					if (0 == i)
						pTmpDomain = m_RootDomain.GetDomain(strDomain);
					else
						pTmpDomain = pCurDomain->GetDomain(strDomain);

					if (NULL == pTmpDomain)
					{
						// check if it's last domain part and this part is equal to "www"
						if ((i == (max_i - 1)) && (0 == strDomain.CompareNoCase(_T("www"))))
						{
							pDomain = pCurDomain;
						}
						break;
					}

					pCurDomain = pTmpDomain;

					if (i == (max_i - 1))
						pDomain = pCurDomain;
				}
			}


			if (NULL != pDomain)
			{
				CAtlArray<CString> arrPathParts;
				SplitString(url.GetUrlPath(), _T("/"), arrPathParts, false);

				if (arrPathParts.IsEmpty())
				{
					//					pDomain->m_arrCookie.RemoveAll();	
					pDomain->RemoveAllCookies();


					hr = S_OK;
				}
				else
				{
					SPathRecord *pPath = NULL;
					for (size_t i = 0, max_i = arrPathParts.GetCount(); i < max_i; ++i)
					{
						CString &strPath = arrPathParts[i];

						if (0 == i)
							pPath = pDomain->GetPath(strPath);
						else
							pPath = pPath->GetPath(strPath);

						if (NULL == pPath)
							break;
					}

					if (NULL != pPath)
					{
						//pPath->m_arrCookie.RemoveAll();
						pPath->RemoveAllCookies();

						hr = S_OK;
					}
				}
			}
		}
		while (false);	

		return hr;
	}


	HRESULT COperaDatFile::GetCookie(LPCTSTR lpszUrl, LPCTSTR lpszCookieName, CString & strCookieData)
	{
		HRESULT hr = E_INVALIDARG;

		do
		{
			CUrl url;
			if (!url.CrackUrl(lpszUrl))
				break;

			SDomainRecord *pDomain = NULL;

			if (IsIPAddress(url.GetHostName()))
			{
				pDomain = m_RootDomain.GetDomain(url.GetHostName());
			}
			else
			{
				CAtlArray<CString> arrDomainParts;
				SplitString(url.GetHostName(), _T("."), arrDomainParts, true);

				SDomainRecord *pCurDomain = NULL;

				for (size_t i = 0, max_i = arrDomainParts.GetCount(); i < max_i; ++i)
				{
					CString const & strDomain = arrDomainParts[i];

					SDomainRecord *pTmpDomain = NULL;

					if (0 == i)
						pTmpDomain = m_RootDomain.GetDomain(strDomain);
					else
						pTmpDomain = pCurDomain->GetDomain(strDomain);

					if (NULL == pTmpDomain)
					{
						// check if it's last domain part and this part is equal to "www"
						if ((i == (max_i - 1)) && (0 == strDomain.CompareNoCase(_T("www"))))
						{
							pDomain = pCurDomain;
						}
						break;
					}

					pCurDomain = pTmpDomain;

					if (i == (max_i - 1))
						pDomain = pCurDomain;
				}
			}

			if (NULL != pDomain)
			{
				CAtlArray<CString> arrPathParts;
				SplitString(url.GetUrlPath(), _T("/"), arrPathParts, false);

				if (arrPathParts.IsEmpty())
				{
					SCookieRecord const * pCookie = pDomain->GetCookie(lpszCookieName);			
					if ((NULL != pCookie) && pCookie->GetCookieValue(strCookieData))
						hr = S_OK;
				}
				else
				{
					SPathRecord *pPath = NULL;
					for (size_t i = 0, max_i = arrPathParts.GetCount(); i < max_i; ++i)
					{
						CString const & strPath = arrPathParts[i];

						if (0 == i)
							pPath = pDomain->GetPath(strPath);
						else
							pPath = pPath->GetPath(strPath);

						if (NULL == pPath)
							break;
					}

					if (NULL != pPath)
					{
						SCookieRecord const * pCookie = pPath->GetCookie(lpszCookieName);
						if ((NULL != pCookie) && pCookie->GetCookieValue(strCookieData))
							hr = S_OK;
					}
				}
			}
		}
		while (false);

		return hr;
	}


	HRESULT COperaDatFile::SetCookie(LPCTSTR lpszUrl, LPCTSTR lpszCookieName,
		LPCTSTR lpszCookieData)
	{
		// set cookie value only, other components are not implemented
		HRESULT hr = E_INVALIDARG;

		do
		{
			CUrl url;
			if (!url.CrackUrl(lpszUrl))
				break;

			SDomainRecord *pDomain = NULL;

			if (IsIPAddress(url.GetHostName()))
			{
				pDomain = m_RootDomain.GetDomain(url.GetHostName());
			}
			else
			{
				CAtlArray<CString> arrDomainParts;
				SplitString(url.GetHostName(), _T("."), arrDomainParts, true);

				SDomainRecord *pCurDomain = NULL;

				for (size_t i = 0, max_i = arrDomainParts.GetCount(); i < max_i; ++i)
				{
					CString const & strDomain = arrDomainParts[i];

					SDomainRecord *pTmpDomain = NULL;

					if (0 == i)
						pTmpDomain = m_RootDomain.GetDomain(strDomain);
					else
						pTmpDomain = pCurDomain->GetDomain(strDomain);

					if (NULL == pTmpDomain)
					{
						// check if it's last domain part and this part is equal to "www"
						if ((i == (max_i - 1)) && (0 == strDomain.CompareNoCase(_T("www"))))
						{
							pDomain = pCurDomain;
						}
						break;
					}

					pCurDomain = pTmpDomain;

					if (i == (max_i - 1))
						pDomain = pCurDomain;
				}
			}

			if (NULL != pDomain)
			{
				CAtlArray<CString> arrPathParts;
				SplitString(url.GetUrlPath(), _T("/"), arrPathParts, false);

				if (arrPathParts.IsEmpty())
				{
					SCookieRecord *pCookie = pDomain->GetCookie(lpszCookieName);
					if ((NULL != pCookie) && pCookie->SetCookieValue(lpszCookieData))
						hr = S_OK;
				}
				else
				{
					SPathRecord *pPath = NULL;
					for (size_t i = 0, max_i = arrPathParts.GetCount(); i < max_i; ++i)
					{
						CString const & strPath = arrPathParts[i];

						if (0 == i)
							pPath = pDomain->GetPath(strPath);
						else
							pPath = pPath->GetPath(strPath);

						if (NULL == pPath)
							break;
					}

					if (NULL != pPath)
					{
						SCookieRecord *pCookie = pPath->GetCookie(lpszCookieName);
						if ((NULL != pCookie) && pCookie->SetCookieValue(lpszCookieData))
							hr = S_OK;
					}
				}
			}
		}
		while (false);

		return hr;
	}

	HRESULT COperaDatFile::GetCookiesSet(set <CString> &setURLs)
	{
		HRESULT hr = S_OK;
		POSITION pos;
		pos = m_RootDomain.m_arrDomain.GetHeadPosition();
		while( pos != NULL )
		{
			SDomainRecord &domainRecord = m_RootDomain.m_arrDomain.GetNext( pos );				 
			CString strCurName;
			GetInternalCookies( domainRecord, setURLs, strCurName );
		}

		return hr;
	}

	HRESULT COperaDatFile::GetInternalCookies( SDomainRecord &recDomain, set <CString> &setURLs, CString strName )
	{
		HRESULT hr = S_OK;
		uintapp const DOMAIN_NAME_TAG = 0x1E;
		if (POSITION pos = recDomain.FindFirst(DOMAIN_NAME_TAG))
		{
			mf::CMemoryFile &data = recDomain.GetNext(pos, DOMAIN_NAME_TAG);
			CString strThisName(static_cast<LPCSTR>(data.GetData()), data.GetSize());
			if( !strThisName.IsEmpty() )
			{
				if( !strName.IsEmpty() )
					strName += _T(".");

				strName += strThisName;
			}
			else 
				return E_FAIL;
		}

		POSITION pos;
		pos = recDomain.m_arrPath.GetHeadPosition();
		while( pos != NULL )
		{
			SPathRecord &pathRecord = recDomain.m_arrPath.GetNext( pos );
			if( pathRecord.m_arrCookie.GetCount() > 0)
			{ 
				uintapp const PATH_NAME_TAG = 0x1D;
				if( POSITION pos = pathRecord.FindFirst(PATH_NAME_TAG))
				{		
					mf::CMemoryFile const &data = pathRecord.GetNext(pos, PATH_NAME_TAG);
					CString strThisName(static_cast<LPCSTR>(data.GetData()), data.GetSize());
					if ( data.GetSize()>0 )
					{
						CString strURL = strThisName +  _T("/") + strName;
						ReverseURL( strURL );
						setURLs.insert( strURL );
					}
				}
			}
		}

		pos = recDomain.m_arrDomain.GetHeadPosition();				 
		while( pos != NULL )
		{
			SDomainRecord &domainRecord = recDomain.m_arrDomain.GetNext( pos );
			hr = GetInternalCookies( domainRecord , setURLs, strName );
		}

		if( recDomain.m_arrCookie.GetCount() > 0 )
		{
			ReverseURL( strName );
			setURLs.insert( strName );
		}

		return hr;
	}

	void COperaDatFile::ReverseURL( CString &strURL )
	{
		CAtlArray<CString> arrDomains;

		int nPos = 0;
		LPCTSTR lpDelims = _T("./");
		CString strValueData;
		strValueData = strURL.Tokenize(lpDelims, nPos);

		while (!strValueData.IsEmpty())
		{
			arrDomains.Add( strValueData );
			arrDomains.Add( strURL.Mid(nPos-1,1) );
			strValueData = strURL.Tokenize(lpDelims, nPos);				
		}

		strURL.Empty();
		if( arrDomains.GetCount() > 0 )
		{
			for( long l = static_cast<long>( arrDomains.GetCount() ) - 1; l >= 0; --l )
			{
				strURL += arrDomains[l];
			}			
		}
	}


	//********************************************************************************
	COperaDatFile::SRecord::SRecord(void)
	{
	}

	void COperaDatFile::SRecord::Clear(void)
	{
		m_Fields.RemoveAll();
	}

	HRESULT COperaDatFile::SRecord::AddField(uintapp tag, mf::CMemoryFile const & data)
	{
		HRESULT hr = S_OK;

		try
		{
			m_Fields.Insert(tag, data);
		}
		catch (CAtlException e)
		{
			hr = e;
		}

		return hr;
	}

	POSITION COperaDatFile::SRecord::FindFirst(uintapp tag) const
	{
		return m_Fields.FindFirstWithKey(tag);
	}

	mf::CMemoryFile &COperaDatFile::SRecord::GetNext(POSITION & pos, uintapp tag)
	{
		return m_Fields.GetNextValueWithKey(pos, tag);
	}

	mf::CMemoryFile const &COperaDatFile::SRecord::GetNext(POSITION & pos, uintapp tag) const
	{
		return m_Fields.GetNextValueWithKey(pos, tag);
	}

	bool COperaDatFile::SCookieRecord::GetCookieValue(CString & strValue) const
	{
		bool bResult = false;

		uintapp const COOKIE_VALUE_TAG = 0x11;
		if (POSITION pos = FindFirst(COOKIE_VALUE_TAG))
		{
			mf::CMemoryFile const & data = GetNext(pos, COOKIE_VALUE_TAG);
			strValue.SetString(static_cast<LPCSTR>(data.GetData()), data.GetSize());
			bResult = true;
		}

		uintapp const COOKIE_EXPIRY_DATE_TAG = 0x12;
		if (POSITION pos = FindFirst(COOKIE_EXPIRY_DATE_TAG))
		{
			mf::CMemoryFile const & data = GetNext(pos, COOKIE_EXPIRY_DATE_TAG);
			const time_t *time;
			time_t       timeSwaped;

			time = static_cast<const time_t*>( data.GetData() );
			timeSwaped = SWAP_LONG( *time );

			SYSTEMTIME st;// this may return 
			CTime cookieTime( timeSwaped );
			cookieTime.GetAsSystemTime( st );

			bResult = true;
		}

		return bResult;
	}

	bool COperaDatFile::SCookieRecord::SetCookieValue(LPCTSTR lpszCookieValue)
	{
		bool bResult = false;

		uintapp const COOKIE_VALUE_TAG = 0x11;
		if (POSITION pos = FindFirst(COOKIE_VALUE_TAG))
		{
			mf::CMemoryFile &data = GetNext(pos, COOKIE_VALUE_TAG);			
			data.MakeEmpty();
			if( lstrlen(lpszCookieValue) > 0 )
			{
				if( data.Write( lpszCookieValue , lstrlen(lpszCookieValue) ) == S_OK)
					bResult = true;
			}
		}

		return bResult;
	}


	void COperaDatFile::SPathRecord::Clear(void)
	{
		SRecord::Clear();
		m_arrPath.RemoveAll();
		m_arrCookie.RemoveAll();
	}

	HRESULT COperaDatFile::SPathRecord::AddPath(POSITION & pos)
	{
		HRESULT hr = S_OK;

		try
		{
			pos = m_arrPath.AddTail();
		}
		catch (CAtlException e)
		{
			hr = e;
		}

		return hr;
	}

	COperaDatFile::SPathRecord &COperaDatFile::SPathRecord::GetPath(POSITION pos)
	{
		return m_arrPath.GetAt(pos);
	}

	void COperaDatFile::SPathRecord::DeletePath(POSITION pos)
	{
		m_arrPath.RemoveAt(pos);
	}

	HRESULT COperaDatFile::SPathRecord::AddCookie(POSITION & pos)
	{
		HRESULT hr = S_OK;

		try
		{
			pos = m_arrCookie.AddTail();
		}
		catch (CAtlException e)
		{
			hr = e;
		}

		return hr;
	}

	COperaDatFile::SCookieRecord & COperaDatFile::SPathRecord::GetCookie(POSITION pos)
	{
		return m_arrCookie.GetAt(pos);
	}

	void COperaDatFile::SPathRecord::RemoveAllCookies()
	{
		m_arrCookie.RemoveAll();
	}

	void COperaDatFile::SPathRecord::DeleteCookie(POSITION pos)
	{
		m_arrCookie.RemoveAt(pos);
	}

	COperaDatFile::SPathRecord *COperaDatFile::SPathRecord::GetPath(LPCTSTR lpName)
	{
		SPathRecord *pResult = NULL;

		POSITION pos = m_arrPath.GetHeadPosition();
		while (NULL != pos)
		{
			SPathRecord &path = m_arrPath.GetNext(pos);

			uintapp const PATH_NAME_TAG = 0x1D;
			if (POSITION pos = path.FindFirst(PATH_NAME_TAG))
			{
				mf::CMemoryFile & data = path.GetNext(pos, PATH_NAME_TAG);
				CString strName(static_cast<LPCSTR>(data.GetData()), data.GetSize());
				if (0 == strName.CompareNoCase(lpName))
				{
					pResult = &path;
					break;
				}
			}
		}

		return pResult;
	}

	COperaDatFile::SCookieRecord *COperaDatFile::SPathRecord::GetCookie(LPCTSTR lpName)
	{
		SCookieRecord *pResult = NULL;

		POSITION pos = m_arrCookie.GetHeadPosition();
		while (NULL != pos)
		{
			SCookieRecord &cookie = m_arrCookie.GetNext(pos);

			uintapp const COOKIE_NAME_TAG = 0x10;
			if (POSITION pos = cookie.FindFirst(COOKIE_NAME_TAG))
			{
				mf::CMemoryFile &data = cookie.GetNext(pos, COOKIE_NAME_TAG);
				CString strName(static_cast<LPCSTR>(data.GetData()), data.GetSize());
				if (0 == strName.CompareNoCase(lpName))
				{
					pResult = &cookie;
					break;
				}
			}
		}

		return pResult;
	}

	void COperaDatFile::SDomainRecord::Clear(void)
	{
		SPathRecord::Clear();
		m_arrDomain.RemoveAll();
	}

	HRESULT COperaDatFile::SDomainRecord::AddDomain(POSITION & pos)
	{
		HRESULT hr = S_OK;

		try
		{
			pos = m_arrDomain.AddTail();
		}
		catch (CAtlException e)
		{
			hr = e;
		}

		return hr;
	}

	COperaDatFile::SDomainRecord & COperaDatFile::SDomainRecord::GetDomain(POSITION pos)
	{
		return m_arrDomain.GetAt(pos);
	}

	void COperaDatFile::SDomainRecord::DeleteDomain(POSITION pos)
	{
		m_arrDomain.RemoveAt(pos);
	}

	COperaDatFile::SDomainRecord *COperaDatFile::SDomainRecord::GetDomain(LPCTSTR lpName)
	{
		SDomainRecord *pResult = NULL;

		POSITION pos = m_arrDomain.GetHeadPosition();
		while (NULL != pos)
		{
			SDomainRecord &domain = m_arrDomain.GetNext(pos);

			uintapp const DOMAIN_NAME_TAG = 0x1E;
			if (POSITION pos = domain.FindFirst(DOMAIN_NAME_TAG))
			{
				mf::CMemoryFile &data = domain.GetNext(pos, DOMAIN_NAME_TAG);
				CString strName(static_cast<LPCSTR>(data.GetData()), data.GetSize());
				if (0 == strName.CompareNoCase(lpName))
				{
					pResult = &domain;
					break;
				}
			}
		}

		return pResult;
	}


	//********************************************************************************

	void GetOperaCookiesDirs(CAtlArray<CString> & arrDir)
	{
		arrDir.RemoveAll();

		BOOL bMultiUser = TRUE;

		CString strOperaDir;
		CString strUserDir;

		CRegKey regKey;
		LONG lRes = regKey.Open(HKEY_CURRENT_USER, _T("Software\\Opera Software"), KEY_READ);
		if (ERROR_SUCCESS == lRes)
		{
			CPath path;
			ULONG nSize = MAX_PATH;
			lRes = regKey.QueryStringValue(_T("Last Directory3"),
				path.m_strPath.GetBuffer(nSize), &nSize);
			path.m_strPath.ReleaseBuffer();
			if (ERROR_SUCCESS == lRes)
			{
				strOperaDir = path.m_strPath;

				CPath pathIni(path);
				pathIni.Append(_T("operadef6.ini"));

				bMultiUser = ::GetPrivateProfileInt(_T("System"),
					_T("Multi User"), bMultiUser, pathIni);
			}
		}

		{
			CPath path;
			HRESULT hr = GetAppDataPath(path);
			if (SUCCEEDED(hr))
			{
				path.Append(_T("Opera\\Opera"));
				strUserDir = path.m_strPath;
			}
		}

		if (bMultiUser)
		{
			if (!strUserDir.IsEmpty())
				arrDir.Add(strUserDir);

			if (!strOperaDir.IsEmpty())
				arrDir.Add(strOperaDir);
		}
		else
		{
			if (!strOperaDir.IsEmpty())
				arrDir.Add(strOperaDir);

			if (!strUserDir.IsEmpty())
				arrDir.Add(strUserDir);
		}
	}

	bool OperaGetCookie(LPCTSTR lpszUrl, LPCTSTR lpszCookieName, CString & strCookieData)
	{
		bool bResult = false;

		CAtlArray<CString> arrDir;
		GetOperaCookiesDirs(arrDir);

		for (size_t i = 0, max_i = arrDir.GetCount(); !bResult && (i < max_i); ++i)
		{
			CPath path = arrDir[i];

			path.Append(_T("profile\\cookies4.dat"));

			do
			{
				COperaDatFile file;
				HRESULT hr = file.LoadFile(path);
				if (FAILED(hr))
					break;

				CString strCookie;
				hr = file.GetCookie(lpszUrl, lpszCookieName, strCookieData);
				if (SUCCEEDED(hr))
					bResult = true;
			}
			while (false);
		}

		return bResult;
	}

	bool OperaSetCookie(LPCTSTR lpszUrl, LPCTSTR lpszCookieName,
		LPCTSTR lpszCookieData, SYSTEMTIME const & /*stExpires*/,
		LPCTSTR /*lpszDomain*/, LPCTSTR /*lpszPath*/, bool  /*bSecure*/)
	{
		bool bResult = false;

		CAtlArray<CString> arrDir;
		GetOperaCookiesDirs(arrDir);

		if( arrDir.GetCount() == 0 )
			return false;

		CPath path = arrDir[0];

		path.Append(_T("profile\\cookies4.dat"));

		COperaDatFile file;
		HRESULT hr = file.LoadFile(path);
		if (FAILED(hr))
			return false;

		hr = file.SetCookie(lpszUrl, lpszCookieName, lpszCookieData);

		if (SUCCEEDED(hr))
			bResult = true;

		return bResult;
	}
	/* ============================= Opera =============================*/

	//}	// end of namespace

	bool SetCookie(LPCTSTR lpszUrl, LPCTSTR lpszCookieName,
		LPCTSTR lpszCookieData, SYSTEMTIME const & stExpires,
		LPCTSTR lpszDomain, LPCTSTR lpszPath, bool bSecure)
	{
		bool bIEResult = IESetCookie(lpszUrl, lpszCookieName,
			lpszCookieData, stExpires, lpszDomain, lpszPath, bSecure);

		bool bNetscapeResult = FFSetCookie(lpszUrl, lpszCookieName,
			lpszCookieData, stExpires, lpszDomain, lpszPath, bSecure,
			NETSCAPE8);

		bool bFFResult = FFSetCookie(lpszUrl, lpszCookieName,
			lpszCookieData, stExpires, lpszDomain, lpszPath, bSecure,
			FIREFOX);

		bool bNSBResult = FFSetCookie(lpszUrl, lpszCookieName,
			lpszCookieData, stExpires, lpszDomain, lpszPath, bSecure,
			MOZILLA);

		bool bOperaResult = OperaSetCookie(lpszUrl, lpszCookieName,
			lpszCookieData, stExpires, lpszDomain, lpszPath, bSecure);

		return (bIEResult || bFFResult || bNSBResult || bOperaResult || bNetscapeResult );
	}

	bool FFGetCookie(LPCTSTR lpszUrl, LPCTSTR lpszCookieName,
		CString &strCookieData, FF_TYPE ffType)
	{
		CFFCookiesFile ffCookies( ffType );

		HRESULT hr = ffCookies.LoadFile();
		if(FAILED(hr))
			return false;

		bool bRetVal = ffCookies.GetCookie( lpszUrl, lpszCookieName, strCookieData );

		return bRetVal;
	}

	bool GetCookie(LPCTSTR lpszUrl, LPCTSTR lpszCookieName,
		CString & strCookieData, bool bUnEscape)
	{
		bool bResult = false;

		do
		{
			bResult = IEGetCookie(lpszUrl, lpszCookieName, strCookieData);
			if (bResult)
				break;

			bResult = FFGetCookie(lpszUrl, lpszCookieName, strCookieData, FIREFOX);
			if (bResult)
				break;

			bResult = FFGetCookie(lpszUrl, lpszCookieName, strCookieData, NETSCAPE8);
			if (bResult)
				break;

			bResult = FFGetCookie(lpszUrl, lpszCookieName, strCookieData, MOZILLA);
			if (bResult)
				break;

			bResult = OperaGetCookie(lpszUrl, lpszCookieName, strCookieData);

		}
		while (false);

		if (bResult && bUnEscape)
		{
			CString strTmp;
			DWORD dwSize = strCookieData.GetLength();
			if (AtlUnescapeUrl(strCookieData, strTmp.GetBuffer(dwSize), &dwSize, dwSize))
			{
				strTmp.ReleaseBuffer(dwSize);
				strCookieData = strTmp;
			}
		}

		return bResult;
	}

	//====================================================================
	CMozillaProfile::CMozillaProfile(LPCTSTR filePath)
	{
		m_filePath = filePath;
	}

	BOOL CMozillaProfile::GetProfiles(CAtlArray<PROFILE_ITEM> &arrProfiles)
	{
		HRESULT hr = m_file.Create(m_filePath, GENERIC_READ,
			FILE_SHARE_READ, OPEN_EXISTING);

		if( FAILED(hr))
			return FALSE;

		arrProfiles.RemoveAll();
		PROFILE_ITEM piItem;
		WORD offset = 0x0168; // offset of first profile data

		ULONGLONG lFileSize;
		if( SUCCEEDED( m_file.GetSize( lFileSize ) ) && ( lFileSize > offset ) )
		{
			while( GetNextProfile( offset, piItem.strName, piItem.strPath ) ) 
			{
				arrProfiles.Add( piItem );
			}
		}
		m_file.Close();

		return arrProfiles.GetCount() ? TRUE : FALSE;
	}

	BOOL CMozillaProfile::GetProfilesNames(CAtlArray<CString> &arrProfilesNames)
	{
		HRESULT hr = m_file.Create(m_filePath, GENERIC_READ,
			FILE_SHARE_READ, OPEN_EXISTING);

		if( FAILED(hr))
			return FALSE;

		arrProfilesNames.RemoveAll();
		PROFILE_ITEM piItem;
		WORD offset = 0x0168; // offset of first profile data

		ULONGLONG lFileSize;
		if( SUCCEEDED( m_file.GetSize( lFileSize ) ) && ( lFileSize > offset ) )
		{
			while( GetNextProfile( offset, piItem.strName, piItem.strPath ) ) 
			{
				arrProfilesNames.Add( piItem.strName );
			}
			m_file.Close();
		}

		return arrProfilesNames.GetCount() ? TRUE : FALSE;
	}

	BOOL CMozillaProfile::GetNextProfile(WORD &offset, CString &strProfileName, CString &strPath )
	{
		if( offset <= 0 )	
			return FALSE;

		strPath.Empty();
		strProfileName.Empty();

		m_file.Seek( offset, FILE_BEGIN );
		WORD prOffset;
		m_file.Read( &prOffset, sizeof(WORD) );

		if( prOffset == 0 ) // there is no profiles more
			return FALSE;

		// get name ----------------------------
		m_file.Seek( prOffset-2, FILE_BEGIN );
		char smb;
		while( m_file.Read( &smb, sizeof(char))==S_OK && smb != 0 )
		{
			strProfileName.Insert( 0, smb );
			m_file.Seek( -2 );
		}

		// read directory position -------------
		m_file.Seek( prOffset+20, FILE_BEGIN );
		WORD offPath;
		m_file.Read( &offPath, sizeof(WORD) );

		// get directory -----------------------
		m_file.Seek( offPath-2, FILE_BEGIN );
		while( m_file.Read( &smb, sizeof(char)) == S_OK && smb != 0 )
		{
			strPath.Insert( 0, smb );
			m_file.Seek(-2);
		}

		// next profile data position
		offset = prOffset+12;

		return TRUE;
	}
};
