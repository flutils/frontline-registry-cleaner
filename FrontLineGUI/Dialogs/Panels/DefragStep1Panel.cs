using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace FrontLineGUI
{
    public partial class DefragStep1Panel : UserControl
    {
        public WizzardControl Wizzard
        {
            get;
            private set;
        }
        BackgroundWorker _bw = new BackgroundWorker();
        public DefragStep1Panel(WizzardControl iWizzard)
        {
            InitializeComponent();
            _bw.DoWork += new DoWorkEventHandler(_bw_DoWork);
            _bw.RunWorkerCompleted += new RunWorkerCompletedEventHandler(_bw_RunWorkerCompleted);
            Wizzard = iWizzard;
        }

        void _bw_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            Wizzard.SelectedIndex = 4;
        }

        private void ButtonStartAnalysesStep1_Click(object sender, EventArgs e)
        {
            var def_dir = Path.Combine(Environment.CurrentDirectory,"DefragFiles");
            try
            {
                if (!Directory.Exists(def_dir))
                    Directory.CreateDirectory(def_dir);
            }
            catch
            {
                MessageBox.Show("No permissions to create subdirectory. " + def_dir);
            }
            if (_bw.IsBusy)
            {
                MessageBox.Show("Defrag utility is busy. Please wait while defragmentation process finishes.");
                return;
            }
            else
            {
                foreach(var file in Directory.GetFiles(Path.Combine(Environment.CurrentDirectory, "DefragFiles")))
                {
                    try
                    {
                    File.Delete(file);
                    }
                    catch{continue;}
                }
                _bw.RunWorkerAsync();
            }
        }

        void _bw_DoWork(object sender, DoWorkEventArgs e)
        {
            try
            {
                if (CleanEngineClient.Instance().LoadHivesInfo(Path.Combine(Environment.CurrentDirectory, "DefragFiles")))
                {
                    CleanEngineClient.Instance().AnalizeHives();
                }
            }
            catch { }
        }
    }
}
