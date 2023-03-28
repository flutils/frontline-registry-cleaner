using System.Windows.Controls;
using JCS;
using System.Windows;

namespace FrontLineGUI
{

    // Scan Page
    // Used to provide core scanning functionality
    public partial class Scan : Page
    {

        // Public class vars
        public CPUUtilization CPUInfo;

        // Constructor
        public Scan()
        {
            // Initialize the form
            InitializeComponent();

            // Required for OSIcon
            DataContext = this;

            // CPUID
            // Initialize ticker if CPUID is present
            if (App.ConfigOptions.CPUID) {

                // RPECK 26/03/2023
                // Class instance to return the values for each of the 4 properties of the system (CPU, RAM, HDD, GPU)

            }

            CPUInfo = new CPUUtilization(10, 12, 10, 12);

            // RPECK 28/03/2023
            // Allows us to bind to the CPUInfo object
            CPUPower.DataContext = CPUInfo;

            //OSBlock.DataContext = OSVersionInfo;

            // RPECK 26/03/2023
            // Scan Items Collection
            // Presents an ObservableListCollection of "ScanItem" classes
            ScanItemsCollection ScanItems = new ScanItemsCollection();

        }

        // OS Icon
        // Shows the image at the bottom of the page (next to the OSVersion)
        public string OSIcon
        {
            get {

                // OSVersion
                // Get the version of the OS from the above method and then extract the version of Windows from it
                // https://stackoverflow.com/a/5651002/1143732
                return "/Resources/OS/" + OSVersionInfo.Name.Replace(" ", "-").ToLower() + ".png"; 
            
            }
        }

        // Select All Button Click
        public void SelectAllButton_Click(object sender, RoutedEventArgs e)
        {

            // Find ListBox
            System.Windows.Controls.ListBox ScanItems = FindName("ScanItems") as System.Windows.Controls.ListBox;

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

}
