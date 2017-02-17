using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Text.RegularExpressions;
using System.IO;
using System.Runtime.InteropServices;

namespace FrontLineGUI
{
    public partial class ScanningPanel : UserControl
    {
        public WizzardControl Wizzard
        {
            get;
            private set;
        }
        public long JunksSizeTotal
        {
            get;
            private set;
        }
        public List<CustomScanner> CustomScannerItems = new List<CustomScanner>();
        private List<FourStateControl> _ProcessingItems = new List<FourStateControl>();

        private ScanningPanel() { }

        public ScanningPanel(WizzardControl iWizzard)
        {
            InitializeComponent();

            Wizzard = iWizzard;
            FourStateActiveX.CurrentState = ItemState.eScanning;

            foreach (var item in panelScanning.Controls)
                if (item is FourStateControl)
                {
                    var fourStateItem = item as FourStateControl;
                    fourStateItem.CurrentState = ItemState.eNotScanned;
                    _ProcessingItems.Add(fourStateItem);
                    CustomScannerItems.Add(new CustomScanner
                    {
                        Name = fourStateItem.ScannerName,
                        IDs = fourStateItem.ScannerId,
                        FoundItems= new Dictionary<IDToDescript,long>()
                    });
                }

            CleanEngineClient.Instance().EnableAllScanners(true);
            CleanEngineClient.Instance().ScanningPerfomed += new CleanEngineClient.StartScan(ScanningPanel_ScanningPerfomed);
            MCleanEngine.ManagedCleanEngine.CEScanFinished += new MCleanEngine.CEScannerFinished(ScannerFinishedProcessing);
            MCleanEngine.ManagedCleanEngine.CENotifierItemFound += new MCleanEngine.CENotificationItemFound(ScannerItemFound);
            MCleanEngine.ManagedCleanEngine.CEScanStart += new MCleanEngine.CEScannerStarted(ScannerStartedProcessing);
        }

        void ScanningPanel_ScanningPerfomed()
        {
            foreach (var ctrl in _ProcessingItems)
            {
                ctrl.CurrentState = ItemState.eScanNotPerformed;
            }

            foreach (var id in CleanEngineClient.Instance().GetActualScanners())
            {
                foreach (var item in _ProcessingItems)
                {
                    var ids = item.ScannerId.Split(';');
                    if (_ContainID((int)id, ids))
                    {
                        item.CurrentState = ItemState.eNotScanned;
                        break;
                    }
                }
            }
        }

        private void ButtonStop_Click(object sender, EventArgs e)
        {
            CleanEngineClient.Instance().StopEngine();
            ProgessScan.Value = 100;
            LabelPercents.Text = 100 + "%";
            //Wizzard.SelectedIndex = 1;
            ScanningFinished(true);
        }
        
        private void ButtonPause_Click(object sender, EventArgs e)
        {
            CleanEngineClient.Instance().PauseEngine(true);
            ButtonPause.Visible = false;
            ButtonResume.Visible = true;
        }

        private void ButtonResume_Click(object sender, EventArgs e)
        {
            CleanEngineClient.Instance().PauseEngine(false);
            ButtonPause.Visible = true;
            ButtonResume.Visible = false;
        }

        private int _current_threads_count = 0;
        private delegate void ScannerChangedDelegate(String str, int id);
        private delegate void ScanningFinishedDelegate(bool bAbort);
        private void ScannerFinishedProcessing(int id)
        {

            if (ProgessScan.Value + 100 / CleanEngineClient.Instance().GetActualScanners().Count < 100)
            {
                ProgessScan.MarqueeUpdate();
                ProgessScan.Value += 100 / CleanEngineClient.Instance().GetActualScanners().Count;
            }
            else
            {
                ProgessScan.Value = 100;
                ScanningFinishedDelegate delegate_sc_finished = new ScanningFinishedDelegate(ScanningFinished);
                this.Invoke(delegate_sc_finished, new object[] {false});
            }
            ScannerChangedDelegate delegate_percents = new ScannerChangedDelegate(IncrementPercents);
            LabelPercents.Invoke(delegate_percents, new object[] { ProgessScan.Value.ToString(), id });
        }
        Timer tmFixingApplied = new Timer();
        private void ScanningFinished(bool bAborted)
        {
            if (bAborted)
            {
                foreach (var ctrl in _ProcessingItems)
                {
                    if ((ctrl.CurrentState == ItemState.eScanning))
                    {
                        if (_current_threads_count == 0)
                        {
                            ctrl.CurrentState = ItemState.eNoErrors;
                        }
                        else
                        {
                            var id = ctrl.ScannerId.Split(';').First();
                            if ((id!=null) && _IsFileScanner(Convert.ToInt32(id)))
                            {
                                ctrl.IsFileSacanner = true;
                                ctrl.JunksSize += _current_junk_size;
                                this.JunksSizeTotal += _current_junk_size;
                                ctrl.ErorrsCount = "";
                                ctrl.CurrentState = ItemState.eErrorsFound;
                            }
                            else
                            {
                                var _cur_label_errors = Convert.ToInt32(ctrl.ErorrsCount);
                                _cur_label_errors += _current_threads_count;
                                ctrl.ErorrsCount = _cur_label_errors.ToString();
                                ctrl.CurrentState = ItemState.eErrorsFound;
                            }
                        }
                        
                    }
                }
            }
            ButtonNExtBottom.Visible = true;
            ButtonNextTop.Visible = true;
            ButtonPrevBottom.Visible = true;
            ButtonPrevTop.Visible = true;
            ButtonStop.Visible = false;
            ButtonResume.Visible = false;
            ButtonPause.Visible = false;
            if (ApplicationSettings.AutoRepair)
            {
                tmFixingApplied.Interval = 1000;
                tmFixingApplied.Tick -= tmFixingApplied_Tick;
                tmFixingApplied.Tick += new EventHandler(tmFixingApplied_Tick);
                tmFixingApplied.Start();
            }
        }

        void tmFixingApplied_Tick(object sender, EventArgs e)
        {
            tmFixingApplied.Stop();
            if (ApplicationSettings.IsPaid)
            {
                var bkp_panel = (BackUpPanel)Wizzard.TabPages["BackUpPage"].Controls[0];
                if (bkp_panel != null)
                    bkp_panel.CreateBackUp();
                var settings_panel = (SettingsPanel)Wizzard.TabPages["SettingsPage"].Controls[0];

                Dictionary<String, int> errors_fixed = new Dictionary<String, int>();
                var stat_panel = (StatisticsPanel)Wizzard.TabPages["StatisticsPage"].Controls[0];
                if (stat_panel != null)
                {
                    foreach (var el in CustomScannerItems)
                    {
                        errors_fixed.Add(el.IDs, 0);
                        foreach(var threat_id in el.FoundItems.Keys)
                        {
                           if (settings_panel != null && !settings_panel.GetIgnores().Contains(threat_id.Description))
                               {
                                   if (CleanEngineClient.Instance().FixItem(threat_id.ID))
                                   {
                                       errors_fixed[el.IDs] += 1;
                                   }
                               }
                        
                        }
                    }
                    stat_panel.FixItems(errors_fixed);
                    Wizzard.SelectedTab = Wizzard.TabPages["StatisticsPage"];
                }
            }
            else
            {
                MainWindow main_window = (MainWindow)Wizzard.Tag;
                if (main_window != null)
                {
                    main_window.ShowRegisterScreen(main_window);
                }
            }
        }

        private bool _ContainID(int id, IEnumerable<String> collection)
        {
            foreach (var an_item in collection)
            {
                if (an_item == id.ToString())
                    return true;
            }
            return false;
        }

        private void IncrementPercents(String val, int id)
        {
            LabelPercents.Text = val + "%";

            try
            {
                FourStateControl current_scanner = null;
                foreach (var item in _ProcessingItems)
                {
                    var ids = item.ScannerId.Split(';');
                    if (_ContainID(id, ids))
                    {
                        current_scanner = item;
                        break;
                    }
                }
                if (_current_threads_count == 0
                    && current_scanner != null
                    && current_scanner.ErorrsCount == "0"
                    && current_scanner.JunksSize == 0)
                    current_scanner.CurrentState = ItemState.eNoErrors;
                else 
                    if(current_scanner != null)
                    {
                        if (_IsFileScanner(id))
                        {
                            //design init
                            current_scanner.IsFileSacanner = true;
                            JunksSizeTotal += _current_junk_size;
                            current_scanner.JunksSize += _current_junk_size;
                            var _cur_label_errors = Convert.ToInt32(current_scanner.ErorrsCount);
                            _cur_label_errors += _current_threads_count;
                            current_scanner.ErorrsCount = _cur_label_errors.ToString();
                            current_scanner.CurrentState = ItemState.eErrorsFound;
                            
                        }
                        else
                        {
                            var _cur_label_errors = Convert.ToInt32(current_scanner.ErorrsCount);
                            _cur_label_errors += _current_threads_count;
                            current_scanner.ErorrsCount = _cur_label_errors.ToString();
                            current_scanner.CurrentState = ItemState.eErrorsFound;
                        }
                    }
            }
            catch { }
        }

        public void ReinitScanProperties()
        {
            LabelPercents.Text = "0%";
            ProgessScan.Value = 0;
            labelItems.Text = "0 Errors Found!";
            labelJunkSize.Text = "0MB Junk Files Found!";
            _JunksSize = 0.0;
            _current_junk_size = 0;
            JunksSizeTotal = 0;
            ProgessScan.MarqueeUpdate();
            ButtonResume.Visible = false;
            ButtonPause.Visible = true;
            ButtonStop.Visible = true;
            ButtonNExtBottom.Visible = false;
            ButtonNextTop.Visible = false;
            ButtonPrevBottom.Visible = false;
            ButtonPrevTop.Visible = false;
            foreach (var item in _ProcessingItems)
            {
                item.JunksSize = 0;
                item.ErorrsCount = "0";
                //item.CurrentState = ItemState.eNotScanned;
            }
            foreach (var items in CustomScannerItems)
            {
                items.FoundItems.Clear();
            }
            BackupManager.BackUpKeys.Clear();
            GC.Collect();
        }

        private void ChangeItemsNumber(String val, bool isFileScanner)
        {
            if (!isFileScanner)
            {
                var chunks = labelItems.Text.Split(' ');
                chunks[0] = val;
                var lbltext = chunks[0] + " " + chunks[1] + " " + chunks[2];
                labelItems.Text = lbltext;
            }
            ++_current_threads_count;
        }

        private void ChangeDescription(String val, bool bFake)
        {
            if (val.Length > 50)
                val = val.Substring(0, 50);
            this.ScanningNowLabel.Text = val;
        }

        private delegate void ItemIdToDescriptionDelegate(int item_id, string desc, int scanner_id);
        private delegate void SimpleStrinDelegate(String str, bool isFileScanner);
        private void ScannerItemFound(String desc, int itemid, int scannerid)
        {
            if (!CleanEngineClient.Instance().IsBusy())
                return;
            ScannerToIdDelegate delegate_filescanners = new ScannerToIdDelegate(FileScannerProcessing);
            Invoke(delegate_filescanners, new object[] { scannerid, desc });
           
            SimpleStrinDelegate delegate_errors = new SimpleStrinDelegate(ChangeItemsNumber);
            if (scannerid != (int)CEScannerID.RECYCLEBIN_SCANNER_ID)
                labelItems.Invoke(delegate_errors, new object[] { (itemid + 1).ToString(), _IsFileScanner(scannerid) });
            else
                labelItems.Invoke(delegate_errors, new object[] { (itemid + 1 + _current_threads_count).ToString(), _IsFileScanner(scannerid) });
           
            SimpleStrinDelegate delegate_descr = new SimpleStrinDelegate(ChangeDescription);
            ScanningNowLabel.Invoke( delegate_descr, new object[] { desc , true} );

            ItemIdToDescriptionDelegate delegate_id_desc = new ItemIdToDescriptionDelegate(_ItemIdToDescription);
            Invoke(delegate_id_desc, new object[] { itemid, desc, scannerid});
        }

        private void _ItemIdToDescription(int id, string desc, int scanner_id)
        {
            foreach (var scaner_res_item in CustomScannerItems)
            {
               if (scaner_res_item.IDs.Contains(scanner_id.ToString()))
               {
                   var str = new IDToDescript() {  Description = desc, ID = id };
                   if (scaner_res_item.IsFileScanner())
                   {
                       scaner_res_item.FoundItems.Add(str, _current_junk_size);
                   }
                   else
                   {
                       scaner_res_item.FoundItems.Add(str, 0);
                   }
               }
            }
        }

        private bool _IsFileScanner(int id)
        {
            int[] files_scanner_ids = { 20403, 20404, 20405, 20406, 20407, 20501, 20502, 20503, 20504, 20505, 20506, 20507 };
            if (files_scanner_ids.Contains(id))
                return true;
            return false;
        }

        private void _SizeLabelProcess(long bytes)
        {
            double sz = (double)bytes / 1024.0 / 1024.0;
            _JunksSize += sz;
            if ((_JunksSize > 0.2) && (999 > _JunksSize))
            {
                var vals = labelJunkSize.Text.Split(' ');
                Regex rx = new Regex("(.+)MB Junk Files Found!", RegexOptions.IgnoreCase);
                if (rx.IsMatch(labelJunkSize.Text))
                {
                    var replacement = rx.Replace(labelJunkSize.Text, _JunksSize.ToString("#.##"));
                    labelJunkSize.Text = replacement + "MB Junk Files Found!";
                }
            }
            else if(_JunksSize > 1024)
            {
                var tmp_junk = _JunksSize / 1024.0;
                var vals = labelJunkSize.Text.Split(' ');
                Regex rx = new Regex("(.+)MB Junk Files Found!", RegexOptions.IgnoreCase);
                Regex rx2 = new Regex("(.+)GB Junk Files Found!", RegexOptions.IgnoreCase);
                if (rx.IsMatch(labelJunkSize.Text))
                {
                    var replacement = rx.Replace(labelJunkSize.Text, tmp_junk.ToString("#.##"));
                    labelJunkSize.Text = replacement + "GB Junk Files Found!";
                    return;
                }else
                    if (rx2.IsMatch(labelJunkSize.Text))
                    {
                        var replacement = rx2.Replace(labelJunkSize.Text, tmp_junk.ToString("#.##"));
                        labelJunkSize.Text = replacement + "GB Junk Files Found!";
                    }
            }
        }

        public double _JunksSize = 0.0;
        long _current_junk_size = 0;
        private delegate void ScannerToIdDelegate(int id, String desc);
        private void FileScannerProcessing(int id, String descr)
        {
            #region file_scanners
            if (_IsFileScanner(id))
            {
                //Regex rg = new Regex("File (");
                Regex expFile = new Regex("^file \\((.*?)\\) at directory \\((.*?)\\)$", RegexOptions.IgnoreCase);
                if (expFile.IsMatch(descr))
                {
                    var col = expFile.Split(descr);
                    var path = Path.Combine(col[2], col[1]);
                    try
                    {
                        FileInfo file_info = new FileInfo(path);
                        var file_size = file_info.Length;
                        _SizeLabelProcess(file_size);
                        _current_junk_size += file_size;
                    }
                    catch { }
                }

                Regex expDir = new Regex("^directory \\((.*?)\\)$", RegexOptions.IgnoreCase);
                if (expDir.IsMatch(descr))
                {
                    //var col = expDir.Split(descr);
                }

                if (id == (int)CEScannerID.RECYCLEBIN_SCANNER_ID)
                {
                    var lst = GetRecycleBinSize();
                    if (lst != null && (lst.Count == 2))
                    {
                        _SizeLabelProcess(lst[1]);
                        _current_junk_size += lst[1];
                        _current_threads_count += Convert.ToInt32(lst[0] - 1);
                    }
                }
            }
            #endregion
            else
            {
                Regex expDir = new Regex("^key \\((.*?)\\)$", RegexOptions.IgnoreCase);
                if (expDir.IsMatch(descr))
                {
                    var col = expDir.Split(descr);
                    try
                    {
                        BackupManager.BackUpKeys.Add(col[1]);
                    }
                    catch { }
                }
            }
        }

        private delegate void SimpleScannerIDDelegate(int id);
        private void ScannerStart(int id)
        {
            _current_threads_count = 0;
            _current_junk_size = 0;
            try
            {
                FourStateControl current_scanner = null;
                foreach (var item in _ProcessingItems)
                {
                    var ids = item.ScannerId.Split(';');
                    if (_ContainID(id, ids))
                    {
                        current_scanner = item;
                        break;
                    }
                }
                if (current_scanner != null)
                    current_scanner.CurrentState = ItemState.eScanning;
            }
            catch { }
        }

        private void ScannerStartedProcessing(int id)
        {
            SimpleScannerIDDelegate del = new SimpleScannerIDDelegate(ScannerStart);
            this.Invoke(del, new object[] { id });
        }

        #region RecycleBin
        
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode, Pack = 1)]
        public struct SHQUERYRBINFO
        {
            public Int32 cbSize;
            public UInt64 i64Size;
            public UInt64 i64NumItems;
        }
       
        [DllImport("shell32.dll", CharSet = CharSet.Unicode)]
        public static extern int SHQueryRecycleBin(string pszRootPath, ref SHQUERYRBINFO pSHQueryRBInfo);

        public static List<long> GetRecycleBinSize()
        {
            SHQUERYRBINFO query = new SHQUERYRBINFO();
            List<long> info = new List<long>();
            query.cbSize = Marshal.SizeOf(typeof(SHQUERYRBINFO));

            try
            {
                int result = SHQueryRecycleBin(null, ref query);

                if (result == 0)
                {
                    info.Add(Convert.ToInt64(query.i64NumItems));
                    info.Add(Convert.ToInt64(query.i64Size));

                    return info;
                }
                else
                    throw new Win32Exception(Marshal.GetLastWin32Error());
            }
            catch (Exception)
            {
                //MessageBox.Show(string.Format("Error accessing Recycle Bin: {0}", ex.Message), "System Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return null;
            }

        }
        #endregion

        private void ButtonNextTop_Click(object sender, EventArgs e)
        {
            var fixing_panel = (FixErrorsPanel)Wizzard.TabPages["FixingPage"].Controls[0];
            if (fixing_panel != null)
            {
                fixing_panel.PrepareCaption(labelItems.Text, labelJunkSize.Text, Convert.ToInt32(labelItems.Text.Split(' ').First()), JunksSizeTotal);
                fixing_panel.PrepareTreeWithErrors();
            }
            Program.CurrentState = ApplicationState.eFixingErrors;
            Wizzard.SelectedIndex = 9;
        }

        private void ButtonPrevTop_Click(object sender, EventArgs e)
        {
            Wizzard.SelectedIndex = 0;
        }

    }
}
