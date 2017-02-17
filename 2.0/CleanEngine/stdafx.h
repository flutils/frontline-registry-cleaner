#pragma once

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS 1
#endif

#define _CRTDBG_MAP_ALLOC

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#define WINVER 0x0500		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#define _WIN32_WINNT 0x0601		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#define _WIN32_IE 0x0500	// Change this to the appropriate value to target IE 5.0 or later.

#define _ATL_ALL_WARNINGS
#define _ATL_NO_HOSTING
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

#include <AtlStr.h>
#include <vector>

#define __countof(x) (sizeof(x) / sizeof(x[0]))

#if (_MSC_VER > 1310)
#undef AtlThrow
#define AtlThrow AtlThrowImpl
#else
#define AtlThrow AtlThrow
#endif