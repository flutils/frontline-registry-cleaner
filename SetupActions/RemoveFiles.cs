using System;
using System.Collections;
using System.ComponentModel;
using System.Configuration.Install;

// RPECK 08/03/2022
// Used to get rid of any remnants left after usage (Backup folder)
// https://stackoverflow.com/a/46787462/1143732

// Powershell for showing all installed MSI's
// https://stackoverflow.com/questions/29937568/how-can-i-find-the-product-guid-of-an-installed-msi-setup
namespace SetupActions
{
    [RunInstaller(true)]
    public partial class RemoveFiles : Installer
    {
        protected override void OnBeforeUninstall(IDictionary savedState)
        {
            // Vars
            var application_path = Context.Parameters["path"];
            var app_data_path    = System.IO.Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData), "Frontline Utilities LTD", "FLCleaner 2.0");

            // If there are any files/folders in the Program Files location
            if (application_path != null && System.IO.Directory.Exists(application_path))
            {

                // Remove Subdirectories
                // Incase the app puts directories in the program files directory
                foreach (var subDir in new System.IO.DirectoryInfo(application_path).GetDirectories())
                {
                    subDir.Delete(true);
                }

                // Remove Files 
                // Seems that some redundant files may be left, so remove them
                foreach (var file in new System.IO.DirectoryInfo(application_path).GetFiles("*.txt"))
                {
                    file.Delete();
                }

            }

            // Remove the AppData directory
            if(System.IO.Directory.Exists(app_data_path))
            {
                System.IO.Directory.Delete(app_data_path, true);
            }

        }
    }
}
