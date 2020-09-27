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
    public partial class DefragStep2Panel : UserControl
    {
        public WizzardControl Wizzard
        {
            get;
            private set;
        }
        BackgroundWorker _bw = new BackgroundWorker();
        private DefragStep2Panel() { }

        public DefragStep2Panel(WizzardControl iWizzard)
        {
            InitializeComponent();

            Wizzard = iWizzard;
            _bw.DoWork += new DoWorkEventHandler(_bw_DoWork);
            _bw.RunWorkerCompleted += new RunWorkerCompletedEventHandler(_bw_RunWorkerCompleted);
        }

        private void ButtonCancelDefragStep2_Click(object sender, EventArgs e)
        {
            Wizzard.SelectedIndex = 3;
        }

        private void ButtonCompleteDefrag_Click(object sender, EventArgs e)
        {
            if (_bw.IsBusy)
            {
                MessageBox.Show("Defrag utility is busy.");
                return;
            }
            else
                _bw.RunWorkerAsync();
        }

        void _bw_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            if (!CleanEngineClient.Instance().IsBusy())
                Wizzard.SelectedIndex = 0;
            else
                Wizzard.SelectedIndex = 8;
        }

        void _bw_DoWork(object sender, DoWorkEventArgs e)
        {
            try
            {
                CleanEngineClient.Instance().CompactHives();
            }
            catch
            { }
        }
    }
}
