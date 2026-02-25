using FrontLineGUI.Include.Classes.DB.Models;
using FrontLineGUI.Include.Services;
using System;
using System.Diagnostics;
using System.Windows.Input;
using System.Windows.Threading;

namespace FrontLineGUI
{
    public class ScanViewModel : ViewModelBase
    {

        // RPECK 06/02/2025 - Declarations
        // Used to give us the ability to call or modify attributes publicly
        public Scan Scan { get; set; }
        public ScanView Model { get; private set; }
        public ScanItemsCollection _scanItemsCollection { get; set; }
        public CPUUtilization CPUInfo { get; set; }
        public OSInfo OSInformation { get; set; }

        // RPECK 24/02/2026 - Services
        // Used to provide the means to interfaec with a variety of services that are invoked in the main application
        private readonly ScanService _scanService;

        private readonly INavigationService _navigation;

        public Scan CurrentScan => _scanService.currentScan;

        // RPECK 23/02/2026 - Last Performed
        // Pulls from the Scan EntityFramework model and allows us to reference the Scan that has the youngest date
        public DateTime? lastPerformed;

        // RPECK 06/02/2025 - Commands
        // These are used to provide the means to interact with the underlying system
        public ICommand SelectAllClick { get; private set; }
        public ICommand LastScanButtonClick { get; private set; }
        public ICommand MainScanButtonClick { get; private set; }

        public ScanViewModel(INavigationService navigation)
        {

            // RPECK 06/02/2025 - Navigation
            // Used to ensure we have the means to manage how we are able to interface with the system
            _navigation = navigation;

            // RPECK 08/02/2025 - Set up the OSInfo Value
            // This invokes a new instance of the "OSInfo" class we created for the purpose
            OSInformation = new OSInfo();

            // RPECK 08/02/2025 - CPUInfo
            // Used to populate the CPU/RAM/HDD/GPU values on the scanning screen
            CPUInfo = new CPUUtilization();

            // RPECK 26/03/2023 - Scan Items Collection
            // Presents an ObservableListCollection of "ScanItem" classes
            // --
            // RPECK 24/02/2026 - So, the core function of this is to provide the user with the ability to select the ScanItems they wish to use and then click on "Scan" to create a new Scan object 
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
            SelectAllClick = new DelegateCommand(o => ScanItemsCollection.SelectAll());
            LastScanButtonClick = new DelegateCommand(o => LastScanClick());
            MainScanButtonClick = new DelegateCommand(o => MainScanClick());

            // RPECK 08/02/2025 - Set up a timer to get the hardware info to update 
            // https://spacetech.dk/c-wpf-run-a-function-every-second.html
            DispatcherTimer dispatcherTimer = new DispatcherTimer();
            dispatcherTimer.Tick += new EventHandler(update_cpu_values);
            dispatcherTimer.Interval = new TimeSpan(0, 0, 1);
            dispatcherTimer.Start();

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
        // Should invoke the "About" view and load up the latest "results" panel
        public void LastScanClick()
        {
            _navigation.NavigateTo<AboutViewModel>();
        }

        // RPECK 08/02/2025 - Main Scan Button Click
        // This should take the selected ScanItem objects and use them to create a new "Scan" object in the database
        // --
        // The "Scan" object should then run, which will chane the ViewModel to progress. The progress ViewModel should then handle the scanner function
        public void MainScanClick()
        {
            Debug.WriteLine("test22");
        }

        // RPECK 08/02/2025 - Updates CPUInfo Values
        // Called by the ticker above to provide updates to the CPU/RAM/HDD/GPU values
        public void update_cpu_values(object sender, EventArgs e)
        {
            CPUInfo.UpdateValues();
        }

        // RPECK 23/02/2026 - LastPerformed
        // Get the latest value from the database and use it to populate the front-end
        public string LastPerformed
        {
            get { return "Never"; }
            set {}
        }

    }

}
