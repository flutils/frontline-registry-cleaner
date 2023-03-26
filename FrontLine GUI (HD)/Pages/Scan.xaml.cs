using System.Windows.Controls;
using System.Windows.Forms;
using System.Threading;
using System;
using System.Diagnostics;
using JCS;

namespace FrontLineGUI
{
    //Scan Logic
    public partial class Scan : Page
    {

        // RPECK 15/08/2022
        // Added for performance monitoring
        private static volatile bool _shouldStop = false;

        public Scan()
        {
            // Initialize the form
            InitializeComponent();

            // Required for OSIcon
            this.DataContext = this;

            // CPUID
            // Initialize ticker if CPUID is present
            if (App.ConfigOptions.CPUID) {}

        }

        // CPU Power
        // Value from HWMonitor to get the CPU utilization
        public int CPUPower
        {
            get { return 0; }
            set { }
        }

        // RAM Power
        // Value from HWMonitor to get the CPU utilization
        public int RAMPower
        {
            get { return 0; }
            set { }
        }

        // CPU Power
        // Value from HWMonitor to get the CPU utilization
        public int HDDSpace
        {
            get { return 0; }
            set { }
        }

        // CPU Power
        // Value from HWMonitor to get the CPU utilization
        public int GPUPower
        {
            get { return 0; }
            set { }
        }

        // OSVersionInfo
        public string OSVersionName
        {
            get { return OSVersionInfo.Name;  }
        }

        public string OSVersionBits
        {
            get { return OSVersionInfo.OSBits.ToString().Replace("Bit", ""); }
        }

        // OSVersionBuild
        public string OSVersionBuild
        {
            get { return "Build " + OSVersionInfo.VersionString; }
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

    }

}
