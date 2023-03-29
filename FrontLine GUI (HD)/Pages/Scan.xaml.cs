using System.Windows.Controls;
using JCS;
using System.Windows;
using System.Diagnostics;
using System;
using System.Linq;
using System.Collections.ObjectModel;

namespace FrontLineGUI
{

    // Scan Page
    // Used to provide core scanning functionality
    public partial class Scan : Page
    {

        // Public class vars
        public CPUUtilization CPUInfo;
        //public ScanItemsCollection ScanItemsObject;
        public ObservableCollection<ScanItem> ScanItemsObject;

        #region Constructor

        // Constructor
        public Scan()
        {

            // Required for OSIcon
            DataContext = this;
            
            // Define CPUInfo
            CPUInfo = new CPUUtilization(0,10,0,0);

            // RPECK 26/03/2023
            // Scan Items Collection
            // Presents an ObservableListCollection of "ScanItem" classes
            ScanItemsObject = new ObservableCollection<ScanItem>()
            {
                new ScanItem("Registry Errors", "Clean registry errors.", true, "/Resources/Scan/registry_errors.png")
            };

            Debug.Write(ScanItemsObject.Where(x => x.IsSelected == true).Count());

            // Initialize the form
            InitializeComponent();

            // RPECK 28/03/2023
            // Allows us to bind to the CPUInfo object
            CPUPower.DataContext = CPUInfo;
        }

        #endregion

        #region Methods

        // OS Icon
        // Shows the image at the bottom of the page (next to the OSVersion)
        public string OSIcon
        {
            get
            {

                // OSVersion
                // Get the version of the OS from the above method and then extract the version of Windows from it
                // https://stackoverflow.com/a/5651002/1143732
                return "/Resources/OS/" + OSVersionInfo.Name.Replace(" ", "-").ToLower() + ".png";

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

        // Scan Items ListBox Change
        // This is used to create functionality that is dependent on the 
        public void ScanItemsListBox_Changed(object sender, SelectionChangedEventArgs args)
        {

            if (ScanItemsObject != null)
            {
                Debug.Write(ScanItemsObject[0].IsSelected);
                ScanItemsObject.Clear();
                ScanItemsObject.Add(new ScanItem("Registry Errors", "Clean registry errors.", true, "/Resources/Scan/registry_errors.png"));
                Debug.Write(ScanItemsObject.Count());
            }

        }

        // Select All Button Click
        public void SelectAllButton_Click(object sender, RoutedEventArgs e)
        {

            if (ScanItemsObject != null)
            {
                Debug.Write(ScanItemsObject[0].IsSelected);
                ScanItemsObject.Clear();
                ScanItemsObject.Add(new ScanItem("Registry Errors", "Clean registry errors.", true, "/Resources/Scan/registry_errors.png"));
                Debug.Write(ScanItemsObject.Count());
            }

            if (App.pSDK != null)
            { 
                string s;
                float fValue;
                int dummy = 0;

                // RPECK 26/03/2023
                // Class instance to return the values for each of the 4 properties of the system (CPU, RAM, HDD, GPU)
                fValue = App.pSDK.GetSensorTypeValue(CPUIDSDK.SENSOR_UTILIZATION_CPU, ref dummy, ref dummy);
                Debug.Write(fValue);
                if (Math.Round(fValue, 0) >= 0)
                {
                    s = Convert.ToString(Math.Round(fValue, 1));
                    s = s + "  %";
                    Debug.Write(s);
                }
            }

            // Find ListBox
            ListBox ScanItems = FindName("ScanItems") as ListBox;

            // If present, selectAll else deselect everything
            if (ScanItems.SelectedItems.Count != ScanItems.Items.Count)
            {
                ScanItems.SelectAll();
            } else 
            {
                ScanItems.UnselectAll();
            }

        }

    }

    #endregion

}
