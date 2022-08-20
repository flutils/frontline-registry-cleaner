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
            // Initialize the page
            InitializeComponent();

            // Populates ListView
            var backup_dir = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData), "Frontline Utilities LTD", "FLCleaner 2.0", "Backup");
            try
            {
                if (Directory.Exists(backup_dir))
                {
                    // Get Directories
                    DirectoryInfo d_info = new DirectoryInfo(backup_dir);
                    FileInfo[] files = d_info.GetFiles("*.bkp");

                    // Title
                    Title.Text += " (" + files.Length + ")";

                    // If 
                    if(files.Length > 0) {

                        // Index
                        // This is required to give us access to the first iteration of the list
                        int index = 0;

                        // Create radio buttons for each backup file
                        foreach (var file in files)
                        {
                            // RadioButton (this is used to add to our system)
                            RadioButton button = new RadioButton() { Content = file.Name, IsChecked = (index == 0), GroupName = "backup" };

                            // Add radio button
                            Backups.Children.Add(button);

                            // Index ++
                            index++;
                        }

                    } else {

                        // Message
                        TextBlock message = new TextBlock { Text = "No Backups Present!" };

                        // Add radio button
                        Backups.Children.Add(message);

                    }//if
                }
            }
            catch { }

        }

        // Create Backup
    }
}
