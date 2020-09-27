#pragma once

/*  1.4. References

[1] ms-help://MS.MSDNQTR.2004APR.1033/com/htm/reg_6ywj.htm

[2] ms-help://MS.MSDNQTR.2004APR.1033/shellcc/platform/shell/programmersguide/ 
    shell_adv/bands.htm

[3] ms-help://MS.MSDNQTR.2004APR.1033/shellcc/platform/shell/programmersguide/ 
    shell_adv/registeringapps.htm

[4] ms-help://MS.MSDNQTR.2004APR.1033/shellcc/platform/shell/programmersguide/ 
    shell_adv/apphelp.htm

[5] ms-help://MS.MSDNQTR.2004APR.1033/shellcc/platform/shell/programmersguide/ 
    shell_adv/conpanel.htm

[6] ms-help://MS.MSDNQTR.2004APR.1033/shellcc/platform/shell/programmersguide/ 
    shell_int/shell_int_extending/extensionhandlers/contextmenuhandlers.htm

[7] ms-help://MS.MSDNQTR.2004APR.1033/shellcc/platform/shell/programmersguide/ 
    shell_basics/shell_basics_extending/fileassociations/fileassoc.htm

[8] ms-help://MS.MSDNQTR.2004APR.1033/shellcc/platform/shell/programmersguide/ 
    shell_basics/shell_basics_extending/fileassociations/
    fa_arbitrary_file_types.htm

[9] ms-help://MS.MSDNQTR.2004APR.1033/shellcc/platform/shell/programmersguide/ 
    shell_basics/shell_basics_extending/fileassociations/fa_progids.htm

[10] ms-help://MS.MSDNQTR.2004APR.1033/shellcc/platform/shell/programmersguide/
    shell_basics/shell_basics_extending/context.htm

*/

enum scanner_id;

extern const size_t TotalScannersNum;
extern const scanner_id ScannersSet[ ];

/**
* @See concrete scanner implementation for brief description
*/
enum scanner_id
{
	// Registry Scanners
	//
	FILEASSOC_SCANNER_ID        = 20301,
	//COMCLASS_SCANNER_ID         = 20302,
	//COMPROG_SCANNER_ID          = 20303,
	//COMTYPELIB_SCANNER_ID       = 20304,
	//COMINTERFACE_SCANNER_ID     = 20305,
	COMOBJECT_SCANNER_ID		= 20320,
	APPLICATIONS_SCANNER_ID     = 20306,
	UNINSTALL_SCANNER_ID        = 20307,
	BANDS_SCANNER_ID            = 20308,
	CLIENTS_SCANNER_ID          = 20309,
	HELP_SCANNER_ID             = 20310,
	CTRLPANEL_SCANNER_ID        = 20311,
	APPPATH_SCANNER_ID          = 20312,
	MENUHND_SCANNER_ID          = 20313,
	BHO_SCANNER_ID              = 20314,
	SHAREDDLL_SCANNER_ID        = 20315,
	DISTUNITS_SCANNER_ID        = 20316,
	SHELLEXT_SCANNER_ID         = 20317,
	MISSPATH_SCANNER_ID         = 20318,
	STARTMENU_SCANNER_ID        = 20319,
	MRU_SCANNER_ID              = 20402,
	EMPTY_KEYS_SCANNER_ID		= 20601,
	// File scanners
	//
	IECOOKIES_SCANNER_ID        = 20403,
	IEHISTORY_SCANNER_ID        = 20404,
	DOWNLOAD_SCANNER_ID         = 20405,
	TEMPWUPDATE_SCANNER_ID      = 20406,
	RECENTDOC_SCANNER_ID        = 20407,

	TEMPIE_SCANNER_ID           = 20501,
	RECYCLEBIN_SCANNER_ID       = 20502,
	TEMPICQ_SCANNER_ID          = 20503,
	BROKENSHCUTS_SCANNER_ID     = 20504,
	GENERALTEMP_SCANNER_ID      = 20505,
	PRFTCHSTUFF_SCANNER_ID      = 20506,
	CUSTOMTEMP_SCANNER_ID       = 20507,

	// New DC scanners...
	//
	APP_SCANNER_ID				= 30401,
	CLIPBOARD_SCANNER_ID		= 30402,
	MOZILLA_BROWSERS_SCANNER_ID	= 30403,
	CUSTOMFILES_SCANNER_ID		= 30404,
	IE_SCANNER_ID				= 30405,
	MESSENGER_SCANNER_ID		= 30406,
	TEMP_FILES_SCANNER_ID		= 30407,
	OPERA_SCANNER_ID			= 30408,
	IE_AUTOFILL_SCANNER_ID		= 30409
	
};

enum scanner_type
{
	UNKNOWN_SCANNER     = -1,

	REGISTRY_SCANNER    = 1,
	FILE_SCANNER,
	MEMORY_SCANNER,
};
