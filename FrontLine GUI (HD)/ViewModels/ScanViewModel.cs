using JCS;
using System;
using System.Collections.Generic;
using System.Configuration;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace FrontLineGUI
{
    public class ScanViewModel : ViewModelBase
    {

        // RPECK 06/02/2025 - Declarations
        // Used to give us the ability to call or modify attributes publicly
        public ScanView Model { get; private set; }
        public ScanItemsCollection _scanItemsCollection { get; set; }
        public CPUUtilization CPUInfo { get; set; }
        public OSInfo OSInformation { get; set; }

        // RPECK 06/02/2025 - Commands
        public ICommand SelectAllClick      { get; private set; }
        public ICommand LastScanButtonClick { get; private set; }

        public ScanViewModel(ScanView model)
        {

            // RPECK 07/02/2025 - Model
            // Allows us to allocate the different view settings inside the ViewModel
            Model = model;

            // RPECK 06/02/2025 - Title
            // Sets the public title of the ModelView (in this case, "Scan")
            Title = "Scan";

            // RPECK 08/02/2025 - Set up the OSInfo Value
            // This invokes a new instance of the "OSInfo" class we created for the purpose
            OSInformation = new OSInfo();

            // RPECK 26/03/2023 - Scan Items Collection
            // Presents an ObservableListCollection of "ScanItem" classes
            ScanItemsCollection = new ScanItemsCollection()
            {
                new ScanItem("Registry Errors", "Clean registry errors.", true, "/Resources/Scan/registry_errors.png", "20318;20311;20319;"),
                new ScanItem("Invalid Files", "Clear invalid file errors.", true, "/Resources/Scan/invalid_files.png", "20310;20301;20313;"),
                new ScanItem("Application Errors", "Remove old application settings.", true, "/Resources/Scan/application_errors.png", "20320;20308;20306;20312;"),
                new ScanItem("DLL Errors", "Fix orphaned DLL entries.", true, "/Resources/Scan/dll_errors.png", "20315;"),
                new ScanItem("Internet Errors", "Fix web browser errors.", true, "/Resources/Scan/internet_errors.png"),
                new ScanItem("Windows Errors", "Resolve Windows registry errors.", true, "/Resources/Scan/windows_errors.png", "20307;20316;20402;20309;"),
                new ScanItem("Temp Files", "Clear Windows temp files.", true, "/Resources/Scan/temp_files.png", "20505;20504;20506;20406;20503;20507;20407;"),
                new ScanItem("Junk Files", "Remove Windows junk files.", true, "/Resources/Scan/recycle_bin.png", "20502;20405;"),
                new ScanItem("Internet Cache", "Clear privacy data from browser caches.", true, "/Resources/Scan/internet_cache.png", "20314;20317;20403;20404;20501;")
            };

            // RPECK 06/02/2025 - Hook up Commands to associated methods
            SelectAllClick       = new DelegateCommand(o => ScanItemsCollection.SelectAll());
            LastScanButtonClick  = new DelegateCommand(o => LastScanClick());

        }

        // RPECK 07/02/2025 - Manage the ScanItems held in memory by the view
        // Allows us to ensure we are working with the correct data by scoping it to this view only
        public ScanItemsCollection ScanItemsCollection
        {

            get { return _scanItemsCollection; }
            set
            {
                _scanItemsCollection = value;
                OnPropertyChanged("ScanItemsCollection");
            }

        }

        // RPECK 08/02/2025 - LastScan Button Click
        // Should invoke the "About" view 
        public void LastScanClick()
        {
            Debug.WriteLine("test");
        }

    }

}
