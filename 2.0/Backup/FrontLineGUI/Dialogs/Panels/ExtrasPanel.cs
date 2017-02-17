using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace FrontLineGUI
{
    public partial class ExtrasPanel : UserControl
    {
        public WizzardControl Wizzard
        {
            get;
            private set;
        }

        public ExtrasPanel(WizzardControl iWizzard)
        {
            InitializeComponent();

            Wizzard = iWizzard;
        }
        
        int[] files_scanner_ids = { 20403, 20404, 20405, 20406, 20407, 20501, 20502, 20503, 20504, 20505, 20506, 20507 };

        private void ButtonCleanRegExtras_Click(object sender, EventArgs e)
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

        private void ButtonRemoveJunksExtras_Click(object sender, EventArgs e)
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

        private void ButtonDefragExtras_Click(object sender, EventArgs e)
        {
            Wizzard.SelectedIndex = 3;
        }

        private void ButtonStartUpExtras_Click(object sender, EventArgs e)
        {
            Wizzard.SelectedIndex = 6;
        }

        private void ButtonStartUpManagerBig_Click(object sender, EventArgs e)
        {
            Wizzard.SelectedIndex = 6;
        }

        private void ButtonDefragBig_Click(object sender, EventArgs e)
        {
            Wizzard.SelectedIndex = 3;
        }

    }
}
