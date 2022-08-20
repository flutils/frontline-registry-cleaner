using System.Windows.Controls;
using System.Windows.Forms;
using Microsoft.VisualBasic.Devices;
using System.Threading;
using System.Text.RegularExpressions; // Regex
using System;

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

            // Thread
            Thread UpdateThread;
            bool res;
            int dll_version = 0;
            int error_code = 0, extended_error_code = 0;
            string error_message;

            UpdateThread = new Thread(new ThreadStart(ThreadLoop));

            pSDK = new CPUIDSDK();
            pSDK.CreateInstance();

            res = pSDK.Init(CPUIDSDK.szDllPath,
                            CPUIDSDK.szDllFilename,
                            CPUIDSDK.CPUIDSDK_CONFIG_USE_EVERYTHING,
                            ref error_code,
                            ref extended_error_code);

            if (error_code != CPUIDSDK.CPUIDSDK_ERROR_NO_ERROR)
            {
                //	Init failed, check errorcode
                switch ((uint)error_code)
                {
                    case CPUIDSDK.CPUIDSDK_ERROR_EVALUATION:
                        {
                            switch ((uint)extended_error_code)
                            {
                                case CPUIDSDK.CPUIDSDK_EXT_ERROR_EVAL_1:
                                    error_message = "You are running a trial version of the DLL SDK. In order to make it work, please run CPU-Z at the same time.";
                                    break;

                                case CPUIDSDK.CPUIDSDK_EXT_ERROR_EVAL_2:
                                    error_message = "Evaluation version has expired.";
                                    break;

                                default:
                                    error_message = "Eval version error " + extended_error_code;
                                    break;
                            }
                        }
                        break;

                    case CPUIDSDK.CPUIDSDK_ERROR_DRIVER:
                        error_message = "Driver error " + extended_error_code;
                        break;

                    case CPUIDSDK.CPUIDSDK_ERROR_VM_RUNNING:
                        error_message = "Virtual machine detected.";
                        break;

                    case CPUIDSDK.CPUIDSDK_ERROR_LOCKED:
                        error_message = "SDK mutex locked.";
                        break;

                    default:
                        error_message = "Error code 0x%X" + error_code;
                        break;
                }
                MessageBox.Show(error_message, "CPUID SDK Error");
            }

            if (res)
            {
                pSDK.GetDllVersion(ref dll_version);

                UpdateThread.Start();

                _shouldStop = true;
                UpdateThread.Join();
            }

            pSDK.Close();
            pSDK.DestroyInstance();

        }

        // Threadloop
        public static void ThreadLoop()
        {
            while (!_shouldStop)
            {
                pSDK.RefreshInformation();
                Thread.Sleep(1000);
            }
        }

        // OS Version
        // Used to show the Operating System name in the bottom left of the grid
        // https://stackoverflow.com/a/45045313/1143732
        public string OSVersion
        {
            get { return new ComputerInfo().OSFullName.ToString(); }
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

                return "/Resources/" + image; 
            
            }
        }

    }
}
