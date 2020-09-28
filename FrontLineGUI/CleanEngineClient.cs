// External Calls
using System;
using System.Collections.Generic;
using FLCleanEngine;

// Class
namespace FrontLineGUI
{
    public class CleanEngineClient
    {
        private FLCleanEngine.ManagedCleanEngine _engine = null;
        private static CleanEngineClient _This = null;
        public delegate void StartScan();

        private List<CEScannerID> _ActualScanners = new List<CEScannerID>();

        public ManagedCleanEngine Engine { get => _engine; set => _engine = value; }

        private CleanEngineClient()
        {
            Engine = new FLCleanEngine.ManagedCleanEngine();
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
            Engine.EnableAllScanners(enable);
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
            Engine.EnableScanner((int)id, enable);
        }

        public void PauseEngine(bool val)
        {
            Engine.Pause(val == true? 1:0 );
        }

        /*public FLCleanEngine.CENotificationItemFound OnItemFound()
        {
            return FLCleanEngine.CENotifierItemFound;
        }

        public FLCleanEngine.CEScannerFinished OnScannerFinished()
        {
            return FLCleanEngine.CEScanFinished;
        }*/

        public void SetScannerList(IEnumerable<CEScannerID> scanners_queue)
        {
            Engine.EnableAllScanners(false);
            _ActualScanners.Clear();
            foreach (var scanner in scanners_queue)
            {
                Engine.EnableScanner((int)scanner, true);
                _ActualScanners.Add(scanner);
            }
        }

        public event StartScan ScanningPerfomed;
        public bool StartEngine()
        {
            if (ScanningPerfomed != null)
                ScanningPerfomed();
            return Engine.Start();
        }

        public void StopEngine()
        {
            Engine.Stop();
        }

        public bool IsBusy()
        {
            return Engine.IsBusy();
        }

        public bool LoadHivesInfo(String path)
        {
            return Engine.LoadHivesInfo(path);
        }

        public bool AnalizeHives()
        {
            return Engine.AnalizeHives();
        }

		public void CompactHives()
        {
            Engine.CompactHives();
        }

        public void FixAll()
        {
            #if !TEST
            Engine.FixAllItems();
            #endif
        }

        public bool FixItem(int id)
        {
            #if !TEST
            return Engine.FixItem(id);
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
