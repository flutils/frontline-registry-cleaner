#pragma once

#include <set>

class CIETools
{

protected:

	void SubEnumIndexDat( std::set<CString> & arrIndexDat, LPTSTR szRoot );
	void GetAllIEHistoryIndexDat( std::set<CString> & arrIndexDat );

};
