using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Microsoft.Win32;

namespace FrontLineGUI
{
    public partial class StatisticsPanel : UserControl
    {
        public WizzardControl Wizzard
        {
            get;
            private set;
        }

        private StatisticsPanel() { }

        public StatisticsPanel(WizzardControl iWizzard)
        {
            InitializeComponent();
            
            Wizzard = iWizzard;
            FLCleanEngine.ManagedCleanEngine.CEScanStart += new FLCleanEngine.CEScannerStarted(ScannerStartedProcessing);
            FLCleanEngine.ManagedCleanEngine.CENotifierItemFound += new FLCleanEngine.CENotificationItemFound(ScannerItemFound);
            var items = this.ListViewStats.Items;
            foreach (var item in items)
            {
                var casting = (ListViewItem)item;
                {
                    items[items.IndexOf(casting)].SubItems.Add("0");
                    items[items.IndexOf(casting)].SubItems.Add("0");
                    items[items.IndexOf(casting)].SubItems.Add("0");
                }
            }
            errors.Add(ScannerHelpers.ScannerAsociation(20601), 0);
        }

        private void _CalculateErrors(int error_fixed)
        {
            int errors_cleaned = error_fixed;
            RegistryKey key = Registry.CurrentUser.CreateSubKey(Program.ApplicationOptions + @"\stats");
            var err_val_in_reg = key.GetValue("err_count");
            if (err_val_in_reg != null)
            {
                var errs = Convert.ToInt32(err_val_in_reg);
                errors_cleaned += errs;
            }
            if(key != null)
                key.SetValue("err_count", errors_cleaned.ToString());
            LabelErrorsCount.Text = errors_cleaned.ToString();
        }

        private void _CalculateJunks()
        {
            double lbl = 0;
            var panel_scan = (ScanningPanel)Wizzard.TabPages["ScanningPage"].Controls[0];
            if (panel_scan != null)
            {
                RegistryKey key = Registry.CurrentUser.CreateSubKey(Program.ApplicationOptions + @"\stats");
                // panel_scan.JunksSize;
                if (key != null)
                {
                    var junk_val_in_reg = key.GetValue("junk_size");
                    if (junk_val_in_reg != null)
                    {
                        var sz = Convert.ToDouble(junk_val_in_reg);
                        lbl += sz;
                    }
                    lbl += LabelToSize(panel_scan.JunksSizeTotal);
                    key.SetValue("junk_size", lbl);
                }
                LabelJunkSize.Text = lbl.ToString("#.#") + "Gb";
                LabelJunkSize.Visible = (lbl < 0.1) ? false : true;
            }
        }

        public void FixItems(Dictionary<String, int> errors_fixed)
        {
            var items = this.ListViewStats.Items;
            int error_count_total = 0;
            foreach (var item in items)
            {
                var casting = (ListViewItem)item;
                {
                    var scanners_in_stat_item = (String)casting.Tag;
                    foreach (var error in errors_fixed.Keys)
                    {
                        foreach(var scanner_id in error.Split(';'))
                        {
                            if (!String.IsNullOrEmpty(scanner_id) && scanners_in_stat_item.Contains(scanner_id))
                            {
                                var fixed_items = errors_fixed[error];
                                error_count_total += fixed_items;
                                items[items.IndexOf(casting)].SubItems[2].Text = fixed_items.ToString();
                                var initial_param = Convert.ToInt32(items[items.IndexOf(casting)].SubItems[1].Text);
                                var difference = (initial_param - fixed_items);
                                items[items.IndexOf(casting)].SubItems[3].Text = difference.ToString();
                                break;
                            }
                        }
                        
                    }
                }
            }

            _CalculateJunks();
            _CalculateErrors(error_count_total);
        }

        public void FixAll()
        {
            var items = this.ListViewStats.Items;
            int errors_cleaned = 0;
            foreach (var item in items)
            {
                var casting = (ListViewItem)item;
                {
                    errors_cleaned += Convert.ToInt32(items[items.IndexOf(casting)].SubItems[1].Text);
                    items[items.IndexOf(casting)].SubItems[2].Text = items[items.IndexOf(casting)].SubItems[1].Text;
                }
            }

            _CalculateJunks();
            _CalculateErrors(errors_cleaned);
        }

        private double LabelToSize(long parameter)
        {
            double retVal = 0.0;
            //if (parameter > 1024.0 * 1024.0 * 102.4)
            {
                var gb_size = parameter / (1024.0 * 1024.0 * 1024.0);
                retVal = gb_size;
            }
            /*else if (parameter > 1024.0 * 102.4)
            {
                var mb_size = parameter / (1024.0 * 1024.0);
                retVal = mb_size.ToString("#.#") + "Mb";
            }
            else if (parameter > 102.40)
            {
                var kb_size = parameter / 1024.0;
                retVal = kb_size.ToString("#.#") + "Kb";
            }
            if (!String.IsNullOrEmpty(LabelErrorsCount.Text)
                && (retVal.StartsWith(".") || retVal.StartsWith(",")))
            {
                var new_text = retVal.Insert(0, "0");
                retVal = new_text;
            }*/
            return retVal;
        }
        private void ListViewStats_KeyDown(object sender, KeyEventArgs e)
        {
           
        }

        private void ListViewStats_KeyPress(object sender, KeyPressEventArgs e)
        {
        }

        private void ListViewStats_KeyUp(object sender, KeyEventArgs e)
        {
           
        }

        private void ListViewStats_MouseClick(object sender, MouseEventArgs e)
        {
            return;
        }

        private void ListViewStats_ItemActivate(object sender, EventArgs e)
        {
            return;
        }

        private void ListViewStats_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private Dictionary<CEScannerID, int> errors = new Dictionary<CEScannerID, int>();
        private delegate void ScannerToErrorsDelegate(int scanner_id);
        private int _ErrorsFound = 0;
        private void ScannerItemFound(String desc, int itemid, int scannerid)
        {
            ScannerToErrorsDelegate delegate_errors = new ScannerToErrorsDelegate(UpdateErrorCount);
            this.Invoke(delegate_errors, new object[] { scannerid });
        }

        private void UpdateErrorCount(int val)
        {
            var scanner = ScannerHelpers.ScannerAsociation(val);
            var number = errors[scanner];
            bool bRecyclebin = false;
            if (val == (int)CEScannerID.RECYCLEBIN_SCANNER_ID)
            {
                var lst = ScanningPanel.GetRecycleBinSize();
                if (lst != null && (lst.Count == 2))
                {
                    bRecyclebin = true;
                    _ErrorsFound += Convert.ToInt32(lst[0]);
                    number += Convert.ToInt32(lst[0]);
                }
            }
            else
            {
                _ErrorsFound++;
                number++;
            }
           
           errors[scanner] = number;
           var items = this.ListViewStats.Items;
           foreach (var item in items)
           {
               var casting = (ListViewItem)item;
               var tag = (String)Convert.ChangeType(casting.Tag, TypeCode.String);

               if (tag.Contains(val.ToString()) && !String.IsNullOrEmpty(tag))
               {
                   var prev_errors = items[items.IndexOf(casting)].SubItems[1].Text;
                   int all_errors = 0;
                   if (!bRecyclebin)
                       all_errors = (int)Convert.ChangeType(prev_errors, TypeCode.Int32) + 1;// 
                   else
                   {
                       bRecyclebin = false;
                       all_errors = (int)Convert.ChangeType(prev_errors, TypeCode.Int32) + number;
                   }
                   items[items.IndexOf(casting)].SubItems[1].Text = all_errors.ToString();
                   break;
               }
           }
           //this.LabelErrorsCount.Text = _ErrorsFound.ToString();
        }

        #region ScannerStarted

        private delegate void SimpleScannerIDDelegate(int id);
        private void ScannerStart(int id)
        {
            try
            {
                errors[ScannerHelpers.ScannerAsociation(id)] = 0;
            }
            catch
            {
                errors.Add(ScannerHelpers.ScannerAsociation(id), 0);
            }
        }

        private void ScannerStartedProcessing(int id)
        {
            SimpleScannerIDDelegate del = new SimpleScannerIDDelegate(ScannerStart);
            this.Invoke(del, new object[] { id });
        }

        public void ReinitScanParameters()
        {
            _ErrorsFound = 0;
            var items = this.ListViewStats.Items;
            
            foreach (var item in items)
            {
                var casting = (ListViewItem)item;
                
                items[items.IndexOf(casting)].SubItems[1].Text = _ErrorsFound.ToString();
                items[items.IndexOf(casting)].SubItems[2].Text = _ErrorsFound.ToString();
                items[items.IndexOf(casting)].SubItems[3].Text = _ErrorsFound.ToString();
            }
            //int errors_cleaned = 0;
            //LabelErrorsCount.Text = errors_cleaned.ToString();
            //LabelJunkSize.Text = "0GB";
        }

        #endregion
    }
}
