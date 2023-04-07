using EZLocalizeNS;
using System.Diagnostics;
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
        public static CPUIDSDK pSDK;
        public static ConfigurationOptions ConfigOptions;
        public static EZLocalizeNS.EZLocalize MyEZLocalize = null; // RPECK 07/04/2023 - added for EZLocalization

        // RPECK 18/03/2023
        // https://www.codeproject.com/Articles/524878/Localisation-made-easy-for-WPF
        private void Application_Startup(object sender, StartupEventArgs e)
        {

            // Configuration Options
            // These are loaded from a file and used to populate different features of the app
            ConfigOptions = new ConfigurationOptions();

            // RPECK 07/04/2023
            // Added for localization system
            MyEZLocalize = new EZLocalize(Current.Resources, "en", null, "Include\\Localization\\", "Strings");

            // RPECK 25/03/2023
            // Fire CPUID if option is enabled
            if (ConfigOptions.CPUID)
            {

                // Create a new thread for CPUID
                //Thread CPUID_Thread = new Thread(Init_CPUID);

                // Start Thread
                //CPUID_Thread.Start();

                Init_CPUID();

            }

        }

        // RPECK 25/03/2023
        // This is used to initialize the CPUID library in a separate thread
        private void Init_CPUID()
        {
            // RPECK 24/03/2023
            // CPUID
            bool res;
            int dll_version = 0;
            int error_code = 0, extended_error_code = 0;
            string error_message;

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
                MessageBox.Show(error_message, "CPUID Error");
            }

            if (res)
            {
                pSDK.GetDllVersion(ref dll_version);

            }

        }

    }

}
