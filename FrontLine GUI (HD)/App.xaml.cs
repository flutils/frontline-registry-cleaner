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

            // RPECK 08/04/2023
            // Alocate the scan items variable to he main application thread
            ScanItemsObject = new ScanItemsCollection();

        }

    }

}
