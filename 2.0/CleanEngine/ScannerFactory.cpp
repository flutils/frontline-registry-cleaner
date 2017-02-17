#include "StdAfx.h"
#include "ScannerFactory.h"
#include "Scanners.h"

#ifdef USE_CE // CleanEngine specific scanners

#include "RegistryScanners\\FileAssocRScanner.h"
//#include "RegistryScanners\\ComClassRScanner.h"
//#include "RegistryScanners\\ComProgRScanner.h"
//#include "RegistryScanners\\TypeLibRScanner.h"
//#include "RegistryScanners\\InterfaceRScanner.h"
#include "RegistryScanners\\ComObjectRScanner.h"
#include "RegistryScanners\\ApplicationsRScanner.h"
#include "RegistryScanners\\UninstallRScanner.h"
#include "RegistryScanners\\BandRScanner.h"
#include "RegistryScanners\\ClientsRScanner.h"
#include "RegistryScanners\\HelpRScanner.h"
#include "RegistryScanners\\ControlPanelRScanner.h"
#include "RegistryScanners\\AppPathRScanner.h"
#include "RegistryScanners\\MenuHndRScanner.h"
#include "RegistryScanners\\BhoRScanner.h"
#include "RegistryScanners\\SharedDllRScanner.h"
#include "RegistryScanners\\DistUnitsRScanner.h"
#include "RegistryScanners\\ShellExtRScanner.h"
#include "RegistryScanners\\MissPathRScanner.h"
#include "RegistryScanners\\StartMenuRScanner.h"
#include "EmtyKeysRScanner.h"

#include "FileScanners\\TempWUpdateFScanner.h"
#include "FileScanners\\BrokenShCutsFScanner.h"

#endif // USE_CE

#ifdef USE_DC // DriveCleaner specific scanners

#include "OtherScanners\\ClipboardScanner.h"
#include "OtherScanners\\AppScanner.h"
#include "OtherScanners\\MozillaBrowsersScanner.h"
#include "OtherScanners\\OperaScanner.h"
#include "OtherScanners\\CustomFilesScanner.h"
#include "OtherScanners\\IEScanner.h"
#include "OtherScanners\\MessengerScanner.h"
#include "OtherScanners\\TempFilesScanner.h"
#include "OtherScanners\\IEAutoFillScanner.h"

#endif // USE_DC

// Common scanners

#include "RegistryScanners\\MruRScanner.h"
#include "OtherScanners\\IEHistoryFScanner.h"
#include "OtherScanners\\RecycleBinFScanner.h"
#include "FileScanners\\PrefetchStuffFScanner.h"
#include "FileScanners\\IECookiesFScanner.h"
#include "FileScanners\\DownloadFScanner.h"
#include "FileScanners\\TempICQFScanner.h"
#include "FileScanners\\TempIEFScanner.h"
#include "FileScanners\\RecentDocFScanner.h"
#include "FileScanners\\GeneralTempFScanner.h"
#include "FileScanners\\CustomTempFScanner.h"

// disable warning C4290: C++ exception specification ignored 
// except to indicate a function is not __declspec(nothrow)
#pragma warning( push )
#pragma warning( disable : 4290 )

CScannerFactory::CScannerFactory()
{
}

CScannerFactory::~CScannerFactory()
{
}

BOOL Is64Bit()
{
	typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
	BOOL bIs64BitOS = FALSE;
	LPFN_ISWOW64PROCESS 
		fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(
		GetModuleHandle("kernel32"),"IsWow64Process");

	if (NULL != fnIsWow64Process)
	{
		if (!fnIsWow64Process(GetCurrentProcess(),&bIs64BitOS))
		{
			
		}
	}
	return bIs64BitOS;
}

CCleanScanner * CScannerFactory::CreateScanner( scanner_id Id, CCleanEngine * pEngine, INotifier * pNotifier, IScanProfiler * pProfiler )
	throw( CUnknownScanner, CGenException )
{
	CCleanScanner * pScanner = NULL;

	switch( Id )
	{

#ifdef USE_CE // CleanEngine specific scanners

	case FILEASSOC_SCANNER_ID:
	    pScanner = new CFileAssocRScanner(
	        pEngine, pNotifier, pProfiler );
	    break;

	case COMOBJECT_SCANNER_ID:
		{
		if(Is64Bit())
			return NULL;
		
		pScanner = new CComObjectRScanner(
			pEngine, pNotifier, pProfiler );
		}
		break;

	case APPLICATIONS_SCANNER_ID:
	    pScanner = new CApplicationsRScanner(
	        pEngine, pNotifier, pProfiler );
	    break;

	case UNINSTALL_SCANNER_ID:
	    pScanner = new CUninstallRScanner(
	        pEngine, pNotifier, pProfiler );
	    break;

	case BANDS_SCANNER_ID:
	    pScanner = new CBandRScanner(
	        pEngine, pNotifier, pProfiler );
	    break;

	case CLIENTS_SCANNER_ID:
	    pScanner = new CClientsRScanner(
	        pEngine, pNotifier, pProfiler );
	    break;

	case HELP_SCANNER_ID:
	    pScanner = new CHelpRScanner(
	        pEngine, pNotifier, pProfiler );
	    break;

	case CTRLPANEL_SCANNER_ID:
	    pScanner = new CControlPanelRScanner(
	        pEngine, pNotifier, pProfiler );
	    break;

	case APPPATH_SCANNER_ID:
	    pScanner = new CAppPathRScanner(
	        pEngine, pNotifier, pProfiler );
	    break;

	case MENUHND_SCANNER_ID:
	    pScanner = new CMenuHndRScanner(
	        pEngine, pNotifier, pProfiler );
	    break;

	case BHO_SCANNER_ID:
	    pScanner = new CBhoRScanner(
	        pEngine, pNotifier, pProfiler );
	    break;

	case SHAREDDLL_SCANNER_ID:
	    pScanner = new CSharedDllRScanner(
	        pEngine, pNotifier, pProfiler );
	    break;

	case DISTUNITS_SCANNER_ID:
	    pScanner = new CDistUnitsRScanner(
	        pEngine, pNotifier, pProfiler );
	    break;

	case SHELLEXT_SCANNER_ID:
	    pScanner = new CShellExtRScanner(
	        pEngine, pNotifier, pProfiler );
	    break;

	case MISSPATH_SCANNER_ID:
	    pScanner = new CMissPathRScanner(
	        pEngine, pNotifier, pProfiler );
	    break;

	case STARTMENU_SCANNER_ID:
	    pScanner = new CStartMenuRScanner(
	        pEngine, pNotifier, pProfiler );
	    break;

	case TEMPWUPDATE_SCANNER_ID:
	    pScanner = new CTempWUpdateFScanner(
	        pEngine, pNotifier, pProfiler );
	    break;

	case BROKENSHCUTS_SCANNER_ID:
	    pScanner = new CBrokenShCutsFScanner(
	        pEngine, pNotifier, pProfiler );
	    break;

#endif // USE_CE

#ifdef USE_DC // DriveCleaner specific scanners

	case APP_SCANNER_ID:
	    pScanner = new CAppScanner(
	        pEngine, pNotifier, pProfiler );
	    break;

	case CLIPBOARD_SCANNER_ID:
	    pScanner = new CClipboardScanner(
	        pEngine, pNotifier, pProfiler );
	    break;

	case MOZILLA_BROWSERS_SCANNER_ID:
	    pScanner = new CMozillaBrowsersScanner(
	        pEngine, pNotifier, pProfiler );
	    break;

	case OPERA_SCANNER_ID:
	    pScanner = new COperaScanner(
	        pEngine, pNotifier, pProfiler );
	    break;

	case CUSTOMFILES_SCANNER_ID:
	    pScanner = new CCustomFilesScanner(
	        pEngine, pNotifier, pProfiler );
	    break;

	case IE_SCANNER_ID:
	    pScanner = new CIEScanner(
	        pEngine, pNotifier, pProfiler );
	    break;

	case MESSENGER_SCANNER_ID:
	    pScanner = new CMessengerScanner(
	        pEngine, pNotifier, pProfiler );
	    break;

	case TEMP_FILES_SCANNER_ID:
	    pScanner = new CTempFilesScanner(
	        pEngine, pNotifier, pProfiler );
	    break;

	case IE_AUTOFILL_SCANNER_ID:
		pScanner = new CIEAutoFillScanner(
			pEngine, pNotifier, pProfiler );
		break;

#endif // USE_DC

	case MRU_SCANNER_ID:
	    pScanner = new CMruRScanner(
	        pEngine, pNotifier, pProfiler );
	    break;

	case IECOOKIES_SCANNER_ID:
	    pScanner = new CIECookiesFScanner(
	        pEngine, pNotifier, pProfiler );
	    break;

	case IEHISTORY_SCANNER_ID:
	    pScanner = new CIEHistoryFScanner(
	        pEngine, pNotifier, pProfiler );
	    break;

	case DOWNLOAD_SCANNER_ID:
	    pScanner = new CDownloadFScanner(
	        pEngine, pNotifier, pProfiler );
	    break;

	case RECENTDOC_SCANNER_ID:
	    pScanner = new CRecentDocFScanner(
	        pEngine, pNotifier, pProfiler );
	    break;

	case TEMPIE_SCANNER_ID:
	    pScanner = new CTempIEFScanner(
	        pEngine, pNotifier, pProfiler );
	    break;

	case RECYCLEBIN_SCANNER_ID:
	    pScanner = new CRecycleBinFScanner(
	        pEngine, pNotifier, pProfiler );
	    break;

	case TEMPICQ_SCANNER_ID:
	    pScanner = new CTempICQFScanner(
	        pEngine, pNotifier, pProfiler );
	    break;

	case GENERALTEMP_SCANNER_ID:
	    pScanner = new CGeneralTempFScanner(
	        pEngine, pNotifier, pProfiler );
	    break;

	case PRFTCHSTUFF_SCANNER_ID:
	    pScanner = new CPrefetchStuffFScanner(
	        pEngine, pNotifier, pProfiler );
	    break;

	case CUSTOMTEMP_SCANNER_ID:
	    pScanner = new CCustomTempFScanner(
	        pEngine, pNotifier, pProfiler );
	    break;
	case EMPTY_KEYS_SCANNER_ID:
		pScanner = new CEmtyKeysRScanner(
			pEngine, pNotifier, pProfiler );
		break;
	default:
	    throw new CUnknownScanner( __FILE__, __LINE__ );
	}

	SAFEEXEC( pScanner );
	return pScanner;
}
