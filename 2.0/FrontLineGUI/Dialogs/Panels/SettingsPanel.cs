using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Microsoft.Win32;
//using TaskScheduler;
using System.IO;
using Microsoft.Win32.TaskScheduler;
using FrontLineGUI.Dialogs;

namespace FrontLineGUI
{
    public partial class SettingsPanel : UserControl
    {
        public WizzardControl Wizzard
        {
            get;
            private set;
        }

        private SettingsPanel() { }

        public SettingsPanel(WizzardControl iWizzard)
        {
            InitializeComponent();

            Wizzard = iWizzard;

            checkBoxOnStartUp.Checked = _CheckStartUpEntry();
            checkBoxSetRestore.Checked = ApplicationSettings.SetRestore;
            checkBoxAutoShutDown.Checked = ApplicationSettings.AutoShutDown;
            checkBoxAutomaticallyRepair.Checked = ApplicationSettings.AutoRepair;
            checkBoxAutoUpdates.Checked = ApplicationSettings.AutoUpdate;

            _ComboScheduleType.SelectedIndexChanged += new EventHandler(_ComboScheduleType_SelectedIndexChanged);
            _ComboBoxDay.SelectedIndex = 0;
            _ComboScheduleType.SelectedIndex = 0;
            try
            {
                StreamReader sr = new StreamReader(Path.Combine(Environment.CurrentDirectory, "gnrd.kys"));
                while (sr.Peek() >= 0)
                {
                    listViewIgnoreKeys.Items.Add(sr.ReadLine());
                }
                sr.Close();
            }
            catch { }

            RegistryKey key = Registry.CurrentUser.OpenSubKey(Program.ApplicationOptions + @"\Scheduler",false);
            if (key != null)
            {
                try
                {
                    _ComboScheduleType.SelectedIndex = Convert.ToInt32(key.GetValue("type"));
                }
                catch { }
                try
                {
                    _ComboBoxDay.SelectedIndex = Convert.ToInt32(key.GetValue("day"));
                }
                catch { }
                try
                {
                    _HoursUpDown.Value = Convert.ToInt32(key.GetValue("hours"));
                }
                catch { }
                try
                {
                    _MinutesUpDown.Value = Convert.ToInt32(key.GetValue("minutes"));
                }
                catch { }
            }
        }

        void _ComboScheduleType_SelectedIndexChanged(object sender, EventArgs e)
        {
            _ComboBoxDay.Enabled = (_ComboScheduleType.SelectedIndex == 0) ? true : false;
        }

        private bool _CheckStartUpEntry()
        {
            var reg_entry = "FrontLine";
            try
            {
                var startup_key = Registry.LocalMachine.OpenSubKey(@"software\microsoft\windows\currentversion\run",false);
                if (startup_key != null)
                {
                    var entry = startup_key.GetValue(reg_entry);
                    if (entry != null)
                        return true;
                }
            }
            catch
            {
                try
                {
                    var startup_key = Registry.CurrentUser.OpenSubKey(@"software\microsoft\windows\currentversion\run",false);
                    if (startup_key != null)
                    {
                        var entry = startup_key.GetValue(reg_entry);
                        if (entry != null)
                            return true;
                    }
                }
                catch{ }
            }
            return false;
        }

        int[] files_scanner_ids = { 20403, 20404, 20405, 20406, 20407, 20501, 20502, 20503, 20504, 20505, 20506, 20507 };

        private void ButtonRegSettings_Click(object sender, EventArgs e)
        {
            Wizzard.SelectedIndex = 8;
            if (CleanEngineClient.Instance().IsBusy())
            {
                MessageBox.Show("Scanning engine is busy. Please wait or stop engine to perform next action");
                return;
            }

            CleanEngineClient.Instance().EnableAllScanners(true);
            foreach (var scanner in files_scanner_ids)
                CleanEngineClient.Instance().EnableScanner(ScannerHelpers.ScannerAsociation(scanner), false);
            var page = (Wizzard.TabPages[8].Controls[0] as ScanningPanel);
            var stats_page = (Wizzard.TabPages[1].Controls[0] as StatisticsPanel);
            if (CleanEngineClient.Instance().StartEngine()
                && page != null && stats_page != null)
            {
                page.ReinitScanProperties();
                stats_page.ReinitScanParameters();
            }
        }

        private void ButtonRemoveJunksSettings_Click(object sender, EventArgs e)
        {
            Wizzard.SelectedIndex = 8;
            if (CleanEngineClient.Instance().IsBusy())
            {
                MessageBox.Show("Scanning engine is busy. Please wait or stop engine to perform next action");
                return;
            }

            CleanEngineClient.Instance().EnableAllScanners(false);
            foreach (var scanner in files_scanner_ids)
                CleanEngineClient.Instance().EnableScanner(ScannerHelpers.ScannerAsociation(scanner), true);
            
            var page = (Wizzard.TabPages[8].Controls[0] as ScanningPanel);
            var stats_page = (Wizzard.TabPages[1].Controls[0] as StatisticsPanel);
            if (CleanEngineClient.Instance().StartEngine()
                && page != null && stats_page != null)
            {
                page.ReinitScanProperties();
                stats_page.ReinitScanParameters();
            }
        }

        private void ButtonDefragSettings_Click(object sender, EventArgs e)
        {
            Wizzard.SelectedIndex = 3;
        }

        private void ButtonStartupSettings_Click(object sender, EventArgs e)
        {
            Wizzard.SelectedIndex = 6;
        }

        private void ButtonUpdateSettings_Click(object sender, EventArgs e)
        {
            MainWindow mainWnd = (MainWindow)Wizzard.Tag;
            if (mainWnd != null)
                mainWnd.ShowUpdateScreen(mainWnd);
        }

        private void ButtonGeneralOptions_Click(object sender, EventArgs e)
        {
            _PanelGeneralOptions.Visible = true;
            listViewIgnoreKeys.Visible = false;
            _PanelSheduling.Visible = false;
            buttonRemoveIgnoreKey.Visible = false;
            //buttonAddIgnoreKey.Visible = false;
        }

        private void ButtonIgnoredKeys_Click(object sender, EventArgs e)
        {
            _PanelGeneralOptions.Visible = false;
            _PanelSheduling.Visible = false;
            listViewIgnoreKeys.Visible = true;
            buttonRemoveIgnoreKey.Visible = true;
            //buttonAddIgnoreKey.Visible = true;
            listViewIgnoreKeys.BringToFront();
            listViewIgnoreKeys.Update();
        }

        private void ButtonSheduling_Click(object sender, EventArgs e)
        {
            _PanelGeneralOptions.Visible = false;
            listViewIgnoreKeys.Visible = false;
            _PanelSheduling.Visible = true;
            buttonRemoveIgnoreKey.Visible = false;
            //buttonAddIgnoreKey.Visible = false;
        }

        #region Button Handlers

        private void checkBoxOnStartUp_CheckedChanged(object sender, EventArgs e)
        {
            var reg_entry = "FrontLine";
            var executing_name = "\"" +Application.ExecutablePath+"\"";//System.Reflection.Assembly.GetExecutingAssembly().FullName;
            if (checkBoxOnStartUp.Checked)
            {
                try
                {
                    var startup_key = Registry.LocalMachine.OpenSubKey(@"software\microsoft\windows\currentversion\run",true);
                    if (startup_key != null)
                        startup_key.SetValue(reg_entry, executing_name);
                }
                catch
                {
                    try
                    {
                        var startup_key = Registry.CurrentUser.OpenSubKey(@"software\microsoft\windows\currentversion\run", true);
                        if (startup_key != null)
                            startup_key.SetValue(reg_entry, executing_name);
                    }
                    catch (Exception ex) { /*MessageBox.Show(ex.Message);*/ }
                }
            }
            else
            {
                try
                {
                    var startup_key = Registry.LocalMachine.OpenSubKey(@"software\microsoft\windows\currentversion\run", true);
                    if (startup_key != null)
                        startup_key.DeleteValue(reg_entry);
                }
                catch { }
                try
                {
                    var startup_key = Registry.CurrentUser.OpenSubKey(@"software\microsoft\windows\currentversion\run", true);
                    if (startup_key != null)
                        startup_key.DeleteValue(reg_entry);
                }
                catch { }
            }
        }

        private void checkBoxSetRestore_CheckedChanged(object sender, EventArgs e)
        {
            ApplicationSettings.SetRestore = checkBoxSetRestore.Checked;
        }

        private void checkBoxAutomaticallyRepair_CheckedChanged(object sender, EventArgs e)
        {
            ApplicationSettings.AutoRepair = checkBoxAutomaticallyRepair.Checked;
        }

        private void checkBoxAutoUpdates_CheckedChanged(object sender, EventArgs e)
        {
            ApplicationSettings.AutoUpdate = checkBoxAutoUpdates.Checked;
        }

        private void checkBoxAutoShutDown_CheckedChanged(object sender, EventArgs e)
        {
            ApplicationSettings.AutoShutDown = checkBoxAutoShutDown.Checked;
        }

        #endregion

        private DaysOfTheWeek _GetDay(int index)
        {
            DaysOfTheWeek retVal = DaysOfTheWeek.Sunday;
            switch (_ComboBoxDay.SelectedIndex)
            {
                case 0:
                    retVal = DaysOfTheWeek.Sunday;
                    break;
                case 1:
                    retVal = DaysOfTheWeek.Monday;
                    break;
                case 2:
                    retVal = DaysOfTheWeek.Tuesday;
                    break;
                case 3:
                    retVal = DaysOfTheWeek.Wednesday;
                    break;
                case 4:
                    retVal = DaysOfTheWeek.Thursday;
                    break;
                case 5:
                    retVal = DaysOfTheWeek.Friday;
                    break;
                case 6:
                    retVal = DaysOfTheWeek.Saturday;
                    break;
                default:
                    break;

            }
            return retVal;
        }

        private void _ButtonApplySchedule_Click(object sender, EventArgs e)
        {
            TaskService ts = new TaskService();
            
            TaskDefinition td = ts.NewTask();
            td.RegistrationInfo.Description = "FrontLine Sceduled Scan";
            switch (_ComboScheduleType.SelectedIndex)
            {
                case 0:
                    WeeklyTrigger weekly = new WeeklyTrigger(_GetDay(_ComboBoxDay.SelectedIndex), 1);
                    weekly.StartBoundary = DateTime.Today + TimeSpan.FromHours(Convert.ToDouble(_HoursUpDown.Value)) + TimeSpan.FromMinutes(Convert.ToDouble(_MinutesUpDown.Value));
                    weekly.WeeksInterval = 1;
                    td.Triggers.Add(weekly);
                    break;
                case 1:
                    DailyTrigger daily = new DailyTrigger(1);
                    daily.StartBoundary = DateTime.Today + TimeSpan.FromHours(Convert.ToDouble(_HoursUpDown.Value)) + TimeSpan.FromMinutes(Convert.ToDouble(_MinutesUpDown.Value));
                    daily.DaysInterval = 1;
                    td.Triggers.Add(daily);
                    break;
                default:
                    return;
            }

            td.Actions.Add(new ExecAction(Application.ExecutablePath, "-scan", null));

            ts.RootFolder.RegisterTaskDefinition(@"FrontLineRegScheduledScan", td);
            RegistryKey key = Registry.CurrentUser.CreateSubKey(Program.ApplicationOptions + @"\Scheduler");
            if(key != null)
            {
                key.SetValue("type", _ComboScheduleType.SelectedIndex);
                key.SetValue("day", _ComboBoxDay.SelectedIndex);
                key.SetValue("hours", _HoursUpDown.Value);
                key.SetValue("minutes", _MinutesUpDown.Value);
            }
        }

        public void AddIgnoreKey(String key)
        {
            if (!String.IsNullOrEmpty(key))
            {
                foreach (ListViewItem item in listViewIgnoreKeys.Items)
                {
                    if (item.Text.ToLower() == key.ToLower())
                        return;
                }
                listViewIgnoreKeys.Items.Add(key);
                try
                {
                    StreamWriter sw = new StreamWriter(Path.Combine(Environment.CurrentDirectory, "gnrd.kys"), true);
                    sw.WriteLine(key);
                    sw.Close();
                }
                catch { }
            }
        }

        public IEnumerable<String> GetIgnores()
        {
            foreach (ListViewItem item in listViewIgnoreKeys.Items)
                yield return item.Text;
        }
        private void buttonRemoveIgnoreKey_Click(object sender, EventArgs e)
        {
            foreach (var item in listViewIgnoreKeys.SelectedIndices)
            {
                listViewIgnoreKeys.Items.Remove(listViewIgnoreKeys.SelectedItems[0]);
                try
                {
                    StreamWriter sw = new StreamWriter(Path.Combine(Environment.CurrentDirectory, "gnrd.kys"));
                    foreach(var item2 in listViewIgnoreKeys.Items)
                    {
                        ListViewItem lv = (ListViewItem)(item2);
                        if (lv != null)
                        {
                            sw.WriteLine(lv.Text);
                        }
                    }
                    sw.Close();
                }
                catch { }
            }
        }
    }
}
