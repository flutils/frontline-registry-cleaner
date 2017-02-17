using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace RegGenie
{
    public enum CEScannerID
    {
        // Registry Scanners
        //
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
        //
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

        NONE = 0
    };

    public static class ScannerHelpers
    {
        public static CEScannerID ScannerAsociation(int val)
        {
            switch(val)
            {
                case 20301:
                    return CEScannerID.FILEASSOC_SCANNER_ID;
                case 20320:
                      return CEScannerID.COMOBJECT_SCANNER_ID ;
                case 20306:
                        return CEScannerID.APPLICATIONS_SCANNER_ID;
                case 20307:
                        return CEScannerID.UNINSTALL_SCANNER_ID;
                case 20308:
                        return CEScannerID.BANDS_SCANNER_ID;
                case 20309:
                        return CEScannerID.CLIENTS_SCANNER_ID ;
                case 20310:
                        return CEScannerID.HELP_SCANNER_ID;
                case 20311:
                        return CEScannerID.CTRLPANEL_SCANNER_ID;
                case 20312:
                        return CEScannerID.APPPATH_SCANNER_ID;
                case 20313:
                        return CEScannerID.MENUHND_SCANNER_ID;
                case 20314:
                        return CEScannerID.BHO_SCANNER_ID;
                case 20315:
                        return CEScannerID.SHAREDDLL_SCANNER_ID;
                case 20316:
                        return CEScannerID.DISTUNITS_SCANNER_ID;
                case 20317:
                        return CEScannerID.SHELLEXT_SCANNER_ID;
                case 20318:
                        return CEScannerID.MISSPATH_SCANNER_ID;
                case 20319:
                        return CEScannerID.STARTMENU_SCANNER_ID;
                case 20402:
                        return CEScannerID.MRU_SCANNER_ID;

        // File scanners
        //
                case 20403:
                        return CEScannerID.IECOOKIES_SCANNER_ID;
                case 20404:
                        return CEScannerID.IEHISTORY_SCANNER_ID;
                case 20405:
                        return CEScannerID.DOWNLOAD_SCANNER_ID;
                case 20406:
                        return CEScannerID.TEMPWUPDATE_SCANNER_ID;
                case 20407:
                        return CEScannerID.RECENTDOC_SCANNER_ID;
                case 20501:

                        return CEScannerID.TEMPIE_SCANNER_ID;
                case 20502:
                        return CEScannerID.RECYCLEBIN_SCANNER_ID;
                case 20503:
                        return CEScannerID.TEMPICQ_SCANNER_ID;
                case 20504:
                        return CEScannerID.BROKENSHCUTS_SCANNER_ID;
                case 20505:
                        return CEScannerID.GENERALTEMP_SCANNER_ID;
                case 20506:
                        return CEScannerID.PRFTCHSTUFF_SCANNER_ID;
                case 20507:
                        return  CEScannerID.CUSTOMTEMP_SCANNER_ID;
                default:
                        return CEScannerID.NONE;
                    
            }
            return CEScannerID.NONE;
        }
    }
    
}
