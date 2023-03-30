using System.Collections.ObjectModel;
using System.Linq;

namespace FrontLineGUI
{

    // https://learn.microsoft.com/en-us/dotnet/desktop/wpf/data/how-to-create-and-bind-to-an-observablecollection?view=netframeworkdesktop-4.8
    public class ScanItemsCollection : ObservableCollection<ScanItem>
    {
        public ScanItemsCollection() : base()
        {
            /*Add(new ScanItem("Registry Errors",    "Clean registry errors.", true, "/Resources/Scan/registry_errors.png"));
            Add(new ScanItem("Invalid Files",      "Clear invalid file errors.", true, "/Resources/Scan/invalid_files.png"));
            Add(new ScanItem("Application Errors", "Remove old application settings.", true, "/Resources/Scan/application_errors.png"));
            Add(new ScanItem("DLL Errors",         "Fix orphaned DLL entries.", true, "/Resources/Scan/dll_errors.png"));
            Add(new ScanItem("Internet Errors",    "Fix web browser errors.", true, "/Resources/Scan/internet_errors.png"));
            Add(new ScanItem("Windows Errors",     "Resolve Windows registry errors.", true, "/Resources/Scan/windows_errors.png"));
            Add(new ScanItem("Temp Files",         "Clear Windows temp files.", true, "/Resources/Scan/temp_files.png"));
            Add(new ScanItem("Junk Files",         "Remove Windows junk files.", true, "/Resources/Scan/recycle_bin.png"));
            Add(new ScanItem("Internet Cache",     "Clear privacy data from browser caches.", true, "/Resources/Scan/internet_cache.png")); */
        }

        // RPECK 28/03/2023
        // Get the number of selected items from the ScanItems object
        // https://stackoverflow.com/questions/25803871/counting-number-of-items-in-observablecollection-where-it-equals-1-c-sharp
        public int SelectedItems
        {
            get
            {
                return this.Where(x => x.IsSelected == true).Count();
            }

        }

    }

}
