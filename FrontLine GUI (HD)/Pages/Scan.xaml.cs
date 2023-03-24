using System.Windows.Controls;
using System.Windows.Forms;
using System.Threading;
using System;
using Microsoft.VisualBasic.Devices;
using Microsoft.VisualBasic;
using static System.Runtime.InteropServices.JavaScript.JSType;

namespace FrontLineGUI
{
    //Scan Logic
    public partial class Scan : Page
    {

        // RPECK 15/08/2022
        // Added for performance monitoring
        public static CPUIDSDK pSDK;

        private static volatile bool _shouldStop = false;

        public Scan()
        {
            // Initialize the form
            InitializeComponent();

            // Required for OSVersion
            this.DataContext = this;

            // CPUID
            // Initialize ticker


        }

        // CPU Power
        // Value from HWMonitor to get the CPU utilization
        public int CPUPower
        {
            get { return 0; }
            set { }
        }

        // OS Version
        // Used to show the Operating System name in the bottom left of the grid
        // https://stackoverflow.com/a/45045313/1143732
        public string OSVersion
        {
            get { return Environment.OSVersion.Version.Major.ToString(); }
        }

        // OS Icon
        // Shows the image at the bottom of the page (next to the OSVersion)
        public string OSIcon
        {
            get {

                // Vars 
                string image;

                // OSVersion
                // Get the version of the OS from the above method and then extract the version of Windows from it
                // https://stackoverflow.com/a/5651002/1143732
                switch (Environment.OSVersion.Version.Major)
                {
                    case 95:
                    case 98:
                        image = "windows-95.png";
                        break;
                    case 7:
                        image = "windows-xp.png";
                        break;
                    case 8:
                        image = "windows-8.png";
                        break;
                    case 10:
                        image = "windows-10.png";
                        break;
                    default:
                        image = "windows-11.png";
                        break;
                }

                return "/Resources/OS/" + image; 
            
            }
        }

    }

}
