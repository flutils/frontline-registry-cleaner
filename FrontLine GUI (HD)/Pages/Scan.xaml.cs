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

        // RPECK 15/08/2022
        // Added for performance monitoring
        private static volatile bool _shouldStop = false;

        // CPU Information (Usage/Power)
        // Value from HWMonitor to get the CPU utilization
        public int CPUPower, RAMPower, HDDSpace, GPUPower;

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
