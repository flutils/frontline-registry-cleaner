using System.Windows.Controls;
using System.Windows;
using JCS;
using System.Diagnostics;
using System;
using System.Windows.Threading;
using System.Runtime.Versioning;
using System.Reflection;
using System.Windows.Data;

namespace FrontLineGUI
{

    // RPECK 05/04/2023
    // Notes on the use of "OSVersionInfo"
    // https://www.codeproject.com/Articles/73000/Getting-Operating-System-Version-Info-Even-for-Win

    // RPECK 08/04/2023 - this was required to limit the application to Windows OS versions only
    [SupportedOSPlatform("windows")]
    public partial class Scan : Page
    {

        // Public class vars

        public CPUUtilization CPUInfo { get; set; }
        public ScanItemsCollection ScanItemsObject { get; set; }

        // Constructor
        public Scan()
        {

            // Required for OSIcon
            DataContext = this;

            // Define CPUInfo
            // This is contained within the class itself and doesn't need any configuration options
            CPUInfo = new CPUUtilization();

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

            // Initialize the form
            InitializeComponent();

            // RPECK 28/03/2023
            // Allows us to bind to the CPUInfo object
            CPUInformation.DataContext = CPUInfo;

            // RPECK 02/04/2023
            // Bind to OSVersionInfo -- need to do it programmatically because it requires an instance of the class (XAML attempts to instantiate classes)
            OSName.Text    = OSNameText + " (" + OSVersionInfo.OSBits.ToString().Remove(0,3) + "bit)";
            OSEdition.Text = OSVersionInfo.VersionString.ToString();

        }

        #region Methods

        // RPECK 05/04/2023
        // OS Name
        // This is a private method used to give us the means to access information about the OS
        private string OSNameText
        {
            get
            {
                // Check to see if the name needs to be changed
                if (OSVersionInfo.MajorVersion == 10 && OSVersionInfo.BuildVersion >= 22000) return "Windows 11";
  
                // Default
                return OSVersionInfo.Name;
            }
        }

        // OS Icon
        // Shows the image at the bottom of the page (next to the OSVersion)
        public string OSIcon
        {
            get
            {

                // OSVersion
                // Get the version of the OS from the above method and then extract the version of Windows from it
                // https://stackoverflow.com/a/5651002/1143732
                return "/Resources/OS/" + OSNameText.Replace(" ", "-").ToLower() + ".png";

            }
        }

        #endregion

        #region XAMLEvents

        // RPECK 28/03/2023
        // Fire once form has loaded

        public void ScanForm_Loaded(object sender, RoutedEventArgs e)
        {

            // RPECK 08/04/2023
            // This was recommended rather than a thread
            // https://spacetech.dk/c-wpf-run-a-function-every-second.html
            DispatcherTimer dispatcherTimer = new DispatcherTimer();
            dispatcherTimer.Tick += new EventHandler(update_cpu_usage);
            dispatcherTimer.Interval = new TimeSpan(0, 0, 1);
            dispatcherTimer.Start();

        }

        // RPECK 08/04/2023
        // Update the CPU speed etc using CPUID
        private void update_cpu_usage(object sender, EventArgs e)
        {
            CPUInfo.UpdateValues();
        }

        // RPECK 05/04/2023
        // Add New Button click
        public void AddNewButton_Click(object sender, RoutedEventArgs e)
        {
            InputForm.IsEnabled = !InputForm.IsEnabled;
        }

        // RPECK 07/04/2023
        // Grid cancel button click
        public void GridCancel_Click(object sender, RoutedEventArgs e)
        {
            InputForm.IsEnabled = false;
        }

        // Select All Button Click
        public void SelectAllButton_Click(object sender, RoutedEventArgs e)
        {

            // Find ListBox
            ListBox ScanItems = FindName("ScanItemsElement") as ListBox;

            // If present, selectAll else deselect everything
            if (ScanItems != null && ScanItems.SelectedItems.Count != ScanItems.Items.Count)
            {
                ScanItems.SelectAll();
            }
            else
            {
                ScanItems.UnselectAll();
            }

        }

        // RPECK 03/04/2023
        // LastScan Button Click - requires MainWindow
        // https://stackoverflow.com/a/55469913/1143732
        public void LastScanButton_Click(object sender, RoutedEventArgs e)
        {
            ((MainWindow)Application.Current.MainWindow).SettingsButton_Click(sender, e);
        }

        // RPECK 04/02/2025 - Main Scan Button Click
        // Allows us to trigger the main scan of the system
        public void MainScanButton_Click(object sender, RoutedEventArgs e)
        {

            // Find ListBox
            ListBox ScanItems = FindName("ScanItemsElement") as ListBox;

            // RPECK 04/02/2025 - First check to see if there are some selected options, otherwise don't do anything
            if (ScanItems != null && ScanItems.SelectedItems.Count < 1)
            {
                // RPECK 04/02/2025 - Show a message box to highlight the issue
                MessageBox.Show("Please Select At Least 1 Category To Scan", "Scan");

                // RPECK 04/02/2025 - Exit
                return;

            }

            // RPECK 04/02/2025 - Check to see if the Scanner Client engine is busy
            // This provides some protection against loading the scanner up more than once
            if (CleanEngineClient.Instance().IsBusy())
            {
                // RPECK 04/02/2025 - Show that the engine is busy
                MessageBox.Show("Scanning engine is busy. Please wait or stop engine to perform next action", "Scan");

                // RPECK 04/02/2025 - Exit
                return;

            } else {

                

            }

        }

    }

    #endregion

}
