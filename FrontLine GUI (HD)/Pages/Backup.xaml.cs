using System;
using System.Diagnostics;
using System.IO;
using System.Windows;
using System.Windows.Controls;
using Path = System.IO.Path;

namespace FrontLineGUI.Pages
{
    /// <summary>
    /// Interaction logic for Backup.xaml
    /// </summary>
    public partial class Backup : Page
    {

        // Vars
        // Declared variables for use throughout the panel
        public string backup_dir = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData), "Frontline Utilities LTD", "FLCleaner 2.1", "Backup");
        public FileInfo[] files; // List of files (defined here to keep state)

        // Backup Manager
        private BackupManager manager = new BackupManager();

        // Main
        // This initializes first and allows us to populate the various parts of the panel
        public Backup()
        {
            // Initialize the page
            InitializeComponent();

            // Populate ListView
            try
            {
                if (Directory.Exists(backup_dir))
                {
                    // Get Directories
                    DirectoryInfo d_info = new DirectoryInfo(backup_dir);

                    // Get Files
                    files = d_info.GetFiles("*.bkp");

                    // Title
                    PageTitle.Text += " (" + files.Length + ")";

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
                        TextBlock message = new TextBlock { Text = "No Backups Present.", HorizontalAlignment = System.Windows.HorizontalAlignment.Center };

                        // Add radio button
                        Backups.Children.Add(message);

                    }//if
                }
            }
            catch (Exception ex) {

                // Message
                TextBlock message = new TextBlock { Text = "Error - " + ex.Message, HorizontalAlignment = System.Windows.HorizontalAlignment.Center };

                // Add radio button
                Backups.Children.Add(message);

            }

        }

        // Create Backup Button
        // Checks for the directory, creates filename and then passes to another function
        private void BackupNow_Click(object sender, EventArgs e)
        {
            // Check if the directory exists
            try
            {

                // If directory exists, this does nothing (apparently)
                // https://stackoverflow.com/a/27706919/1143732
                Directory.CreateDirectory(backup_dir);

            }
            catch (Exception ex)
            {

                // Message
                TextBlock error_msg = new TextBlock { Text = "Error - " + ex.Message, HorizontalAlignment = System.Windows.HorizontalAlignment.Center };

                // Add radio button
                Backups.Children.Add(error_msg);

            }

            // Check if number of files
            if (BackupManager.BackUpKeys.Count > 1) { 

                // Define file and path names
                var file_name = new string[] { "backup", DateTime.Now.Day.ToString(), DateTime.Now.Month.ToString(), DateTime.Now.Year.ToString(), DateTime.Now.Hour.ToString(), DateTime.Now.Minute.ToString(), DateTime.Now.Second.ToString() };
                var file_path_and_name = Path.Combine(backup_dir, String.Join("_", file_name));

                // Create file if one does not exist already
                if (manager.BackUpList(file_path_and_name))
                {

                    // Message
                    TextBlock error_msg = new TextBlock { Text = "Success", HorizontalAlignment = System.Windows.HorizontalAlignment.Center };

                    // Add radio button
                    Backups.Children.Add(error_msg);

                    //var lvi = this.ListViewBackUps.Items.Add(file_name);
                    //lvi.Tag = file_path_and_name;
                }


            } else {

                // Scan required
                MessageBox.Show("Please Make A System Scan First.");

            }
        }
    }
}
