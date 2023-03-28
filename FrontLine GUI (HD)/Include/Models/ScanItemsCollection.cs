using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace FrontLineGUI
{
 
    public class ScanItemsCollection : ObservableCollection<ScanItem>, INotifyPropertyChanged
    {
        public ScanItemsCollection() : base()
        {
            Add(new ScanItem("Registry Errors",    true, "/Resources/Scan/registry_errors.png"));
            Add(new ScanItem("Invalid Files",      true, "/Resources/Scan/invalid_files.png"));
            Add(new ScanItem("Application Errors", true, "/Resources/Scan/application_errors.png"));
            Add(new ScanItem("DLL Errors",         true, "/Resources/Scan/dll_errors.png"));
            Add(new ScanItem("Internet Errors",    true, "/Resources/Scan/internet_errors.png"));
            Add(new ScanItem("Windows Errors",     true, "/Resources/Scan/windows_errors.png"));
            Add(new ScanItem("Temp Files",         true, "/Resources/Scan/temp_files.png"));
            Add(new ScanItem("Junk Files",         true, "/Resources/Scan/recycle_bin.png"));
            Add(new ScanItem("Internet Cache",     true, "/Resources/Scan/internet_cache.png"));
        }

        // RPECK 28/03/2023
        // Get the number of selected items from the ScanItems object
        // https://stackoverflow.com/questions/25803871/counting-number-of-items-in-observablecollection-where-it-equals-1-c-sharp
        public int SelectedItems
        {
            get
            {
                return this.Where(x => x.IsSelected == true).Count(); ;
            }
        }

    }

}
