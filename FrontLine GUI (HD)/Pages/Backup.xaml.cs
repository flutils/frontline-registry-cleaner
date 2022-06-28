using System;
using System.IO;
using System.Windows.Controls;
using Path = System.IO.Path;

namespace FrontLineGUI.Pages
{
    /// <summary>
    /// Interaction logic for Backup.xaml
    /// </summary>
    public partial class Backup : Page
    {
        public Backup()
        {
            InitializeComponent();

            // Populates ListView
            var backup_dir = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData), "Frontline Utilities LTD", "FLCleaner 2.0", "Backup");
            try
            {
                if (Directory.Exists(backup_dir))
                {
                    DirectoryInfo d_info = new DirectoryInfo(backup_dir);
                    FileInfo[] fls = d_info.GetFiles("*.bkp");
                    foreach (var fl in fls)
                    {
                        var lvitem = ListViewBackups.Items.Add(fl.Name);
                    }
                }
            }
            catch { }

        }
    }
}
