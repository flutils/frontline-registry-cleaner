using System.Windows.Controls;
using System.Windows;
using JCS;
using System.Diagnostics;
using System;

namespace FrontLineGUI
{

    // RPECK 05/04/2023
    // Notes on the use of "OSVersionInfo"
    // https://www.codeproject.com/Articles/73000/Getting-Operating-System-Version-Info-Even-for-Win

    /*
     * The above library is used to give us access to operating system information
     * The reason for using the library (rather than self coded) is beacuse it offloads the functionality required to compute the results
     * The only issue with it lies in how it is not geared for Windows 11. It may be worth addressing that at some point but, for now, it should be okay
     */

    // Scan Page
    // Used to provide core scanning functionality
    public partial class Scan : Page
    {

        // Public class vars

        public CPUUtilization CPUInfo { get; set; }
        public ScanItemsCollection ScanItemsObject { get; set; }

        #region Constructor

        // Constructor
        public Scan()
        {

            // Required for OSIcon
            DataContext = this;

            // Define CPUInfo
            CPUInfo = new CPUUtilization();

            // RPECK 26/03/2023
            // Scan Items Collection
            // Presents an ObservableListCollection of "ScanItem" classes
            ScanItemsObject = new ScanItemsCollection()
            {
                new ScanItem("Registry Errors", "Clean registry errors.", true, "/Resources/Scan/registry_errors.png"),
                new ScanItem("Invalid Files", "Clear invalid file errors.", true, "/Resources/Scan/invalid_files.png"),
                new ScanItem("Application Errors", "Remove old application settings.", true, "/Resources/Scan/application_errors.png"),
                new ScanItem("DLL Errors", "Fix orphaned DLL entries.", true, "/Resources/Scan/dll_errors.png"),
                new ScanItem("Internet Errors", "Fix web browser errors.", true, "/Resources/Scan/internet_errors.png"),
                new ScanItem("Windows Errors", "Resolve Windows registry errors.", true, "/Resources/Scan/windows_errors.png"),
                new ScanItem("Temp Files", "Clear Windows temp files.", true, "/Resources/Scan/temp_files.png"),
                new ScanItem("Junk Files", "Remove Windows junk files.", true, "/Resources/Scan/recycle_bin.png"),
                new ScanItem("Internet Cache", "Clear privacy data from browser caches.", true, "/Resources/Scan/internet_cache.png")
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

        #endregion

        #region Methods

        // RPECK 05/04/2023
        // OS Name
        // This is a private method used to give us the means to access information about the OS
        private string OSNameText
        {
            get
            {
                // Check to see if the name needs to be changed
                if (OSVersionInfo.MajorVersion == 10 && OSVersionInfo.BuildVersion > 22000) return "Windows 11";
  
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

            // CPUID
            // Initialize ticker if CPUID is present
            if (App.ConfigOptions.CPUID)
            {

                string s;
                float fValue;
                int dummy = 0;

                // RPECK 26/03/2023
                // Class instance to return the values for each of the 4 properties of the system (CPU, RAM, HDD, GPU)
                fValue = App.pSDK.GetSensorTypeValue(CPUIDSDK.SENSOR_UTILIZATION_CPU, ref dummy, ref dummy);
                if (Math.Round(fValue, 0) >= 0)
                {
                    s = Convert.ToString(Math.Round(fValue, 1));
                    s = s + "  %";
                    Debug.Write(s);
                }

            }

        }

        // RPECK 04/04/2023
        // ScanItem Edit Button Click
        public void ScanItemEdit_Click(object sender, RoutedEventArgs e)
        {
            Debug.Write("Edit");
        }

        // RPECK 05/04/2023
        // Add New Button click
        public void AddNewButton_Click(object sender, RoutedEventArgs e)
        {
            ScanItemsObject.Add(
                new ScanItem("Internet Cache", "Clear privacy data from browser caches.", true, "/Resources/Scan/internet_cache.png")
            );
        }

        // Select All Button Click
        public void SelectAllButton_Click(object sender, RoutedEventArgs e)
        {

            // Find ListBox
            ListBox ScanItems = FindName("ScanItemsElement") as ListBox;

            // If present, selectAll else deselect everything
            if (ScanItems.SelectedItems.Count != ScanItems.Items.Count)
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

    }

    #endregion

}
