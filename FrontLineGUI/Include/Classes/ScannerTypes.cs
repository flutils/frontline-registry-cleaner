using System;

namespace FrontLineGUI
{
    public enum CEScannerID
    {
        NONE = 0,

        // Registry Scanners
        FILEASSOC_SCANNER_ID = 20301,
        COMOBJECT_SCANNER_ID = 20320,
        APPLICATIONS_SCANNER_ID = 20306,
        UNINSTALL_SCANNER_ID = 20307,
        BANDS_SCANNER_ID = 20308,
        CLIENTS_SCANNER_ID = 20309,
        HELP_SCANNER_ID = 20310,
        CTRLPANEL_SCANNER_ID = 20311,
        APPPATH_SCANNER_ID = 20312,
        MENUHND_SCANNER_ID = 20313,
        BHO_SCANNER_ID = 20314,
        SHAREDDLL_SCANNER_ID = 20315,
        DISTUNITS_SCANNER_ID = 20316,
        SHELLEXT_SCANNER_ID = 20317,
        MISSPATH_SCANNER_ID = 20318,
        STARTMENU_SCANNER_ID = 20319,
        MRU_SCANNER_ID = 20402,

        // File scanners
        IECOOKIES_SCANNER_ID = 20403,
        IEHISTORY_SCANNER_ID = 20404,
        DOWNLOAD_SCANNER_ID = 20405,
        TEMPWUPDATE_SCANNER_ID = 20406,
        RECENTDOC_SCANNER_ID = 20407,
        TEMPIE_SCANNER_ID = 20501,
        RECYCLEBIN_SCANNER_ID = 20502,
        TEMPICQ_SCANNER_ID = 20503,
        BROKENSHCUTS_SCANNER_ID = 20504,
        GENERALTEMP_SCANNER_ID = 20505,
        PRFTCHSTUFF_SCANNER_ID = 20506,
        CUSTOMTEMP_SCANNER_ID = 20507,
        EMPTY_KEYS_SCANNER_ID = 20601
    }

    public static class ScannerTypes
    {
        /// <summary>
        /// Safely converts an integer ID to its corresponding CEScannerID enum.
        /// </summary>
        public static CEScannerID ScannerAsociation(int val)
        {
            // If the integer is defined in our Enum, cast and return it.
            // Otherwise, return NONE.
            return Enum.IsDefined(typeof(CEScannerID), val)
                ? (CEScannerID)val
                : CEScannerID.NONE;
        }
    }
}