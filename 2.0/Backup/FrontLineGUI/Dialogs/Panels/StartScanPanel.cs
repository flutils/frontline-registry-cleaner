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
    public partial class StartScanPanel : UserControl
    {
        public WizzardControl Wizzard
        {
            get;
            private set;
        }

        private StartScanPanel()
        {
        }
        
        Timer tmScan = new Timer()
        {
            Interval = 2000
        };
        List<CustomFrontLineCheckButton> list_checks = new List<CustomFrontLineCheckButton>();
        public StartScanPanel(WizzardControl iWizzard)
        {
            InitializeComponent();

            Wizzard = iWizzard;
            this.ButtonStartScanMain.MouseHover += new System.EventHandler(ButtonStartScanMain_MouseHover);
            this.ButtonStartScanMain.MouseLeave += new System.EventHandler(ButtonStartScanMain_MouseLeave);

            RegistryKey key = Registry.CurrentUser.OpenSubKey(Program.ApplicationOptions);
            if (key != null)
            {
                String date = String.Empty;
                try
                {
                    date = key.GetValue("LastScan").ToString();
                }
                catch { date = String.Empty; }
                var labels_parts = labelLastScanPerformed.Text.Split(' ');
                if ((labels_parts.Count() > 1) && !String.IsNullOrEmpty(date))
                {
                    labelLastScanPerformed.Text = "";
                    labels_parts[labels_parts.Count() - 1] = date;
                    foreach (var word in labels_parts)
                    {
                        labelLastScanPerformed.Text += (" " + word);
                    }
                }
            }
            list_checks.AddRange(new CustomFrontLineCheckButton[]{  CheckButtonAppErrors,
                                                                    CheckButtonWinErrors,
                                                                    CheckButtonOCXErrors,
                                                                    CheckButtonRegErrors,
                                                                    CheckButtonRecycleBin,
                                                                    CheckButtonTempInternet,
                                                                    CheckButtonDLLErrors,
                                                                    CheckButtonTempFiles,
                                                                    CheckButtonInvalidFiles
                                                                   });
            CleanEngineClient.Instance().ScanningPerfomed += new CleanEngineClient.StartScan(StartScanPanel_ScanningPerfomed);
            if (ApplicationSettings.NeedScan)
            {
                tmScan.Tick -= tmScan_Tick;
                tmScan.Tick += new EventHandler(tmScan_Tick);
                tmScan.Start();
            }
        }

        void tmScan_Tick(object sender, EventArgs e)
        {
            tmScan.Stop();
            ButtonStartScanMain_Click(null, null);
        }

        void StartScanPanel_ScanningPerfomed()
        {
            var labels_parts = labelLastScanPerformed.Text.Split(' ');
            if (labels_parts.Count() > 1)
            {
                var date = labels_parts[labels_parts.Count() - 1] = DateTime.Now.Day + "/" + DateTime.Now.Month + "/" + DateTime.Now.Year;
                RegistryKey key = Registry.CurrentUser.CreateSubKey(Program.ApplicationOptions);
                if (key != null)
                {
                    key.SetValue("LastScan", date.ToString());
                }
                labelLastScanPerformed.Text = "";
                foreach (var word in labels_parts)
                {
                    labelLastScanPerformed.Text += (" " + word);
                }
            }
            
        }

        private void ButtonStartScanMain_MouseHover(object sender, EventArgs e)
        {
            Button btn = (Button)sender;
            if (btn != null)
                btn.BackgroundImage = Properties.Resources.StatScanMainHover_button;
        }

        private void ButtonStartScanMain_MouseLeave(object sender, EventArgs e)
        {
            Button btn = (Button)sender;
            if (btn != null)
                btn.BackgroundImage = Properties.Resources.StatScanMain_button;
        }
        int[] files_scanner_ids = { 20403, 20404, 20405, 20406, 20407, 20501, 20502, 20503, 20504, 20505, 20506, 20507 };
        private void ButtonCleanRegistry_Click(object sender, EventArgs e)
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

        private void ButtonDefragRegistry_Click(object sender, EventArgs e)
        {
            Wizzard.SelectedIndex = 3;
        }

        private void ButtonEditStartUp_Click(object sender, EventArgs e)
        {
            Wizzard.SelectedIndex = 6;
        }

        private void ButtonRemoveJunks_Click(object sender, EventArgs e)
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
            Wizzard.SelectedIndex = 8;
            var page = (Wizzard.TabPages[8].Controls[0] as ScanningPanel);
            var stats_page = (Wizzard.TabPages[1].Controls[0] as StatisticsPanel);
            if (CleanEngineClient.Instance().StartEngine()
                && page != null && stats_page != null)
            {
                page.ReinitScanProperties();
                stats_page.ReinitScanParameters();
            }
        }

        private void ButtonStartScanMain_Click(object sender, EventArgs e)
        {
            foreach (var ctrl_check in list_checks)
            {
                
                //if (ctrl_check != null)
                //{
                    foreach (var index in ctrl_check.ScannersToActivate.Split(';'))
                    {
                        if(!String.IsNullOrEmpty(index))
                            CleanEngineClient.Instance().EnableScanner(ScannerHelpers.ScannerAsociation(Convert.ToInt32(index))
                                                                   , (ctrl_check.ImageIndex == 1) ? true : false);
                    }
                //}
            }

            if (CleanEngineClient.Instance().GetActualScanners().Count < 1)
            {
                MessageBox.Show("Please select at least 1 category to scan");
                return;
            }
            
            Wizzard.SelectedIndex = 8;
            var page = (Wizzard.TabPages[8].Controls[0] as ScanningPanel);
            var stats_page = (Wizzard.TabPages[1].Controls[0] as StatisticsPanel);
            if(CleanEngineClient.Instance().StartEngine() 
                && page != null && stats_page != null)
            {
                page.ReinitScanProperties();
                stats_page.ReinitScanParameters();
            }
        }
    }
}
