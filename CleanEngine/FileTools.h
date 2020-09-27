#pragma once

#include <shlobj.h>
#include <shellapi.h>
#include <list>
#include <string> // RPECK 27/10/2020 (fixes std::string problem)

#include "Stera.h"

#pragma comment( lib, "shell32.lib" )

class CFileTools
{

protected:

	HRESULT DeleteFolder(LPCTSTR lpFolder) const;
	HRESULT IsPathExist(LPCTSTR lpFolder) const;

	void GetDrivesArray( DWORD dwDrives, std::list<std::string> & lstDrives ) const;

};
