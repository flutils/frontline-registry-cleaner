using System.Runtime.Versioning;
using System.Windows;

namespace FrontLineGUI
{

    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {

        // RPECK 24/03/2023
        // CPUID SDK Information
        public static ConfigurationOptions ConfigOptions;
        public ScanItemsCollection ScanItemsObject { get; set; }

        // RPECK 18/03/2023
        // https://www.codeproject.com/Articles/524878/Localisation-made-easy-for-WPF
        private void Application_Startup(object sender, StartupEventArgs e)
        {

            // Configuration Options
            // These are loaded from a file and used to populate different features of the app
            ConfigOptions = new ConfigurationOptions();

            // RPECK 26/03/2023
            // Scan Items Collection
            // Presents an ObservableListCollection of "ScanItem" classes
            ScanItemsObject = new ScanItemsCollection()
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

            // RPECK 05/02/2025 - Load up the MainWindow
            // Used to give us the ability to invoke dynamic content whilst maintaining state
            // Ref: https://stackoverflow.com/a/15960843
            var window = new MainWindow() { DataContext = new MainWindowViewModel() };
            
            // RPECK 05/02/2025 - Show the MainWindow class
            // Gives us the ability to manage its content/data without having other issues
            window.Show();

        }

    }

}
