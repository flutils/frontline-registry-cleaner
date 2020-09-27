#pragma once
#include <atlstr.h>
#include <atlpath.h>


static void GetShortPath(CPath& Path, CString& strPath)
{
  DWORD Size = Path.m_strPath.GetLength()+sizeof(TCHAR);
  Path.MakePretty();
  GetShortPathName(Path,strPath.GetBufferSetLength(Size),Size);
  strPath.ReleaseBuffer();
}


static void GetCLSIDFromPath(CString& strToFind, CString& strResult)
{

   int idx = strToFind.Find( _T("\\{") );
   if(idx!=-1)
   {
	  int lastidx = strToFind.Find(_T('}'),idx + sizeof(TCHAR));
	  if(lastidx!=-1)
         strResult = strToFind.Mid(idx+sizeof(TCHAR), lastidx-idx);
   }

}