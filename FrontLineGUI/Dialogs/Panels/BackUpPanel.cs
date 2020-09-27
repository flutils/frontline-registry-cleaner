using System;
using System.Windows.Forms;
using System.IO;

namespace FrontLineGUI
{
    public partial class BackUpPanel : UserControl
    {
        public WizzardControl Wizzard
        {
            get;
            private set;
        }

        private BackUpPanel() { }
        
        #region data_section

        BackupManager manager = new BackupManager();
        Timer tm = new Timer();

        #endregion

        public BackUpPanel(WizzardControl iWizzard)
        {
            InitializeComponent();
            
            tm.Interval = 80;
            tm.Tick += new EventHandler(tm_Tick);

            try
            {
                var backup_dir = Path.Combine(Environment.CurrentDirectory, "Backup");
                if (Directory.Exists(backup_dir))
                {
                    DirectoryInfo d_info = new DirectoryInfo(backup_dir);
                    FileInfo[] fls = d_info.GetFiles("*.bkp");
                    foreach (var fl in fls)
                    {
                        var lvitem = this.ListViewBackUps.Items.Add(fl.Name);
                        lvitem.Tag = fl.FullName;
                    }
                }
            }
            catch { }
        }

        void tm_Tick(object sender, EventArgs e)
        {
            ProgessBackUp.MarqueeUpdate();
            if ((ProgessBackUp.Value > 90)&&(ProgessBackUp.Value < 92))
            {
                CreateBackUp();
            }else
                if (ProgessBackUp.Value > 98)
                {
                    ProgessBackUp.Value = 100;
                    tm.Stop();
                }
        }

        public void CreateBackUp()
        {
           
            var backup_dir = Path.Combine(Environment.CurrentDirectory, "Backup");
            try
            {

                if (!Directory.Exists(backup_dir))
                    Directory.CreateDirectory(backup_dir);
            }
            catch { return; }
            if (BackupManager.BackUpKeys.Count > 1)
            {
                var file_name = "backup_" + DateTime.Now.Day + "_" + DateTime.Now.Month + "_" + DateTime.Now.Year + "_" + DateTime.Now.Hour + "_" + DateTime.Now.Minute + "_" + DateTime.Now.Second;
                file_name += ".bkp";
                var file_path_and_name = Path.Combine(backup_dir, file_name);
                if (manager.BackUpList(file_path_and_name))
                {
                    var lvi = this.ListViewBackUps.Items.Add(file_name);
                    lvi.Tag = file_path_and_name;
                }
            }
        }

        private void ButtonBackUpBackUp_Click(object sender, EventArgs e)
        {
            if (BackupManager.BackUpKeys.Count < 1)
                MessageBox.Show("Please make system scan first.");
            else
                tm.Start();
        }

       
        private void ButtonLoadBackUp_Click(object sender, EventArgs e)
        {
            var selected = ListViewBackUps.SelectedItems;
            if (selected.Count < 1)
                return;
            ListViewBackUps.BeginUpdate();
            foreach (ListViewItem item in selected)
            {
                try
                {
                    var file_path = (String)item.Tag;
                    if (manager.RestoreBackUp(file_path))
                    {
                        ListViewBackUps.Items.Remove(item);
                        File.Delete(file_path);
                    }
                    else
                        MessageBox.Show(manager.ErrorMessage);
                }
                catch
                {
                    continue;
                }
               
            }
            ListViewBackUps.EndUpdate();
        }

        private void ButtonDeleteBackUp_Click(object sender, EventArgs e)
        {
            var selected = ListViewBackUps.SelectedItems;
            if (selected.Count < 1)
                return;
            ListViewBackUps.BeginUpdate();
            foreach (ListViewItem item in selected)
            {
                try
                {
                    var file_path = (String)item.Tag;
                    File.Delete(file_path);
                    ListViewBackUps.Items.Remove(item);
                }
                catch
                {
                    continue;
                }
            }
            ListViewBackUps.EndUpdate();
        }
    }
}
