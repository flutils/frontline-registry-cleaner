using FrontLineGUI.Include.Classes.DB;
using FrontLineGUI.Include.Classes.DB.Models;
using FrontLineGUI.Include.Interfaces;
using FrontLineGUI.Include.Services;
using FrontLineGUI.Resources.Localization;
using System.Linq;
using System.Windows.Input;

namespace FrontLineGUI
{
    public class ScanViewModel : ViewModelBase
    {
        // RPECK 24/02/2026 - Services
        private readonly ScanService        _scanService;
        private readonly INavigationService _navigation;
        private readonly HardwareService    _hardwareService;
        private readonly IAppConfig         _config;
        private readonly AppDbContext       _db;

        // RPECK 06/02/2025 - Hardware Info Classes
        public OSInfo OSInformation { get; set; }

        // RPECK 27/02/2026 - Scan Options
        // Various datapoints required to manage how the scan should transpire
        private ScanItemsCollection _scanItemsCollection;
        private ScanProcessState _currentState;
        private double _progress;
        private string _lastPerformed = Strings.ScanLastPerformedNever;

        // RPECK 27/02/2026 - CPU/RAM Information
        // This is a service that allows us to manage how the CPU/RAM/HDD information is displayed
        public HardwareService Hardware => _hardwareService;

        // RPECK 06/02/2025 - Commands
        public ICommand SelectAllClick { get; private set; }
        public ICommand LastScanButtonClick { get; private set; }
        public ICommand MainScanButtonClick { get; private set; }
        public ICommand ResetCommand { get; private set; }

        public ScanViewModel(INavigationService navigation, ScanService scanService, HardwareService hardwareService, AppDbContext db, IAppConfig config)
        {

            // RPECK 27/02/2026 - Services
            // These provide an interface between the main application (global) scope and this local scope
            _navigation      = navigation;
            _scanService     = scanService;
            _hardwareService = hardwareService;
            _db              = db;
            _config          = config;

            // Initialize the Scan Items
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

            // Commands
            SelectAllClick      = new DelegateCommand(o => ScanItemsCollection.SelectAll());
            LastScanButtonClick = new DelegateCommand(o => LastScanClick());
            MainScanButtonClick = new DelegateCommand(o => MainScanClick());
            ResetCommand        = new DelegateCommand(o => _scanService.Clear());

            // Sync with Service State (In case we navigated back to an ongoing scan)
            CurrentState = _scanService.CurrentState;
            Progress = _scanService.CurrentProgress;

            // Subscribe to Service Events
            _scanService.ProgressChanged     += OnScanProgressChanged;
            _scanService.StateChanged        += OnScanStateChanged;
            _hardwareService.HardwareUpdated += OnHardwareUpdated;

        }

        #region Observables (UI Bindings)

        public ScanProcessState CurrentState
        {
            get => _currentState;
            set { _currentState = value; OnPropertyChanged(nameof(CurrentState)); }
        }

        public double Progress
        {
            get => _progress;
            set { _progress = value; OnPropertyChanged(nameof(Progress)); }
        }

        public ScanItemsCollection ScanItemsCollection
        {
            get => _scanItemsCollection;
            set { _scanItemsCollection = value; OnPropertyChanged(nameof(ScanItemsCollection)); }
        }

        // RPECK 23/02/2026 - LastPerformed
        // Get the latest value from the database and use it to populate the front-end
        public string LastPerformed
        {
            get => _lastPerformed;
            set { _lastPerformed = value; OnPropertyChanged(nameof(LastPerformed)); }
        }

        #endregion

        #region Logic Methods

        // RPECK 02/03/2026 - Enable the scan by clicking the "MainScanClick" button
        private void MainScanClick()
        {

            var newScan = new Scan
            {
                Status = ScanStatus.Scanning,
                ScanItems = ScanItemsCollection.Where(i => i.IsSelected).ToList()
            };

            _db.Scans.Add(newScan);
            _db.SaveChanges();

            // Hand off to the service to start the actual work
            _scanService.StartScan(newScan.ScanItems);

        }

        private void LastScanClick()
        {
            _navigation.NavigateTo<AboutViewModel>();
        }

        // --- Service Event Callbacks ---

        private void OnScanProgressChanged(double newProgress)
        {
            System.Windows.Application.Current.Dispatcher.Invoke(() => Progress = newProgress);
        }

        private void OnScanStateChanged(ScanProcessState newState)
        {
            System.Windows.Application.Current.Dispatcher.Invoke(() => CurrentState = newState);
        }

        // RPECK 27/02/2026 - Update hardware value per tick
        private void OnHardwareUpdated()
        {
            // Because the service updates on a background timer, 
            // we must marshal the property change back to the UI thread.
            System.Windows.Application.Current.Dispatcher.Invoke(() =>
            {
                // This tells the XAML that "Hardware" properties have new values
                OnPropertyChanged(nameof(Hardware));
            });
        }

        // Clean up the event subscription when the view is destroyed
        // (Optional but good practice)
        public void Dispose()
        {
            _hardwareService.HardwareUpdated -= OnHardwareUpdated;
        }
    }

        #endregion
}