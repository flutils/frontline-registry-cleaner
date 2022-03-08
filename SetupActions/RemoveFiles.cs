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
            var path = System.IO.Path.Combine(Context.Parameters["path"], "Backup");
            if (System.IO.Directory.Exists(path)) System.IO.Directory.Delete(path, true);
        }
    }
}
