// RPECK 25/04/2017
// http://stackoverflow.com/questions/619104/how-to-get-the-namespace-alias-operator-to-work-under-c
extern alias MCleanEngine;

// External Calls
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MCleanEngine;

// Class
namespace FrontLineGUI
{
    public class CleanEngineClient
    {
        private MCleanEngine.ManagedCleanEngine _engine = null;
        private static CleanEngineClient _This = null;
        public delegate void StartScan();

        private List<CEScannerID> _ActualScanners = new List<CEScannerID>();

        private CleanEngineClient()
        {
            _engine = new MCleanEngine.ManagedCleanEngine();
        }

        public static CleanEngineClient Instance()
        {
            if (_This == null)
                _This = new CleanEngineClient();
            return _This;
        }

        public void EnableAllScanners(bool enable)
        {
            if (enable)
            {
                _ActualScanners.Clear();
                _ActualScanners.AddRange(new CEScannerID[] 
                                           {CEScannerID.APPLICATIONS_SCANNER_ID,
                                            CEScannerID.APPPATH_SCANNER_ID,
                                            CEScannerID.BANDS_SCANNER_ID,
                                            CEScannerID.BHO_SCANNER_ID,
                                            CEScannerID.BROKENSHCUTS_SCANNER_ID,
                                            CEScannerID.CLIENTS_SCANNER_ID,
                                            CEScannerID.COMOBJECT_SCANNER_ID,
                                            CEScannerID.CTRLPANEL_SCANNER_ID,
                                            CEScannerID.CUSTOMTEMP_SCANNER_ID,
                                            CEScannerID.DISTUNITS_SCANNER_ID,
                                            CEScannerID.DOWNLOAD_SCANNER_ID,
                                            CEScannerID.FILEASSOC_SCANNER_ID,
                                            CEScannerID.GENERALTEMP_SCANNER_ID,
                                            CEScannerID.HELP_SCANNER_ID,
                                            CEScannerID.IECOOKIES_SCANNER_ID,
                                            CEScannerID.IEHISTORY_SCANNER_ID,
                                            CEScannerID.MENUHND_SCANNER_ID,
                                            CEScannerID.MISSPATH_SCANNER_ID,
                                            CEScannerID.MRU_SCANNER_ID,
                                            CEScannerID.PRFTCHSTUFF_SCANNER_ID,
                                            CEScannerID.RECENTDOC_SCANNER_ID,
                                            CEScannerID.RECYCLEBIN_SCANNER_ID,
                                            CEScannerID.SHAREDDLL_SCANNER_ID,
                                            CEScannerID.SHELLEXT_SCANNER_ID,
                                            CEScannerID.STARTMENU_SCANNER_ID,
                                            CEScannerID.TEMPICQ_SCANNER_ID,
                                            CEScannerID.TEMPIE_SCANNER_ID,
                                            CEScannerID.TEMPWUPDATE_SCANNER_ID,
                                            CEScannerID.UNINSTALL_SCANNER_ID});
            }
            else
                _ActualScanners.Clear();
            _engine.EnableAllScanners(enable);
        }

        public void EnableScanner(CEScannerID id, bool enable)
        {
            lock(this)
            {
                if (enable)
                {
                    if (!_ActualScanners.Contains(id))
                        _ActualScanners.Add(id);
                }
                else
                    _ActualScanners.Remove(id);
            }
            _engine.EnableScanner((int)id, enable);
        }

        public void PauseEngine(bool val)
        {
            _engine.Pause(val == true? 1:0 );
        }

        public MCleanEngine.CENotificationItemFound OnItemFound()
        {
            return ManagedCleanEngine.CENotifierItemFound;
        }

        public MCleanEngine.CEScannerFinished OnScannerFinished()
        {
            return ManagedCleanEngine.CEScanFinished;
        }

        public void SetScannerList(IEnumerable<CEScannerID> scanners_queue)
        {
            _engine.EnableAllScanners(false);
            _ActualScanners.Clear();
            foreach (var scanner in scanners_queue)
            {
                _engine.EnableScanner((int)scanner, true);
                _ActualScanners.Add(scanner);
            }
        }

        public event StartScan ScanningPerfomed;
        public bool StartEngine()
        {
            if (ScanningPerfomed != null)
                ScanningPerfomed();
            return _engine.Start();
        }

        public void StopEngine()
        {
            _engine.Stop();
        }

        public bool IsBusy()
        {
            return _engine.IsBusy();
        }

        public bool LoadHivesInfo(String path)
        {
            return _engine.LoadHivesInfo(path);
        }

        public bool AnalizeHives()
        {
            return _engine.AnalizeHives();
        }

		public void CompactHives()
        {
            _engine.CompactHives();
        }

        public void FixAll()
        {
            #if !TEST
            _engine.FixAllItems();
            #endif
        }

        public bool FixItem(int id)
        {
            #if !TEST
            return _engine.FixItem(id);
            #else
            return true;
            #endif

        }

        public List<CEScannerID> GetActualScanners()
        {
            return _ActualScanners;
        }
    }
}
