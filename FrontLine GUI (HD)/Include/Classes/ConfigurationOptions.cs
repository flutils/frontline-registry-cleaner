using System;
using System.IO;
using EZLocalizeNS;

namespace FrontLineGUI
{
    public class ConfigurationOptions : PropertyChangedBase
    {

        // Private Options
        private bool debug;
        private bool cpuid;
        private string language;

        // Constructor
        // Used to instantiate the class (IE define different options when it's loaded)
        public ConfigurationOptions() {

            // RPECK 07/04/2023
            // Added for localization system
            Localization = new EZLocalize(System.Windows.Application.Current.Resources, "en", null, "Include\\Localization\\", "Strings");

        }

        // RPECK 07/04/2023
        // Added for EZLocalization
        // Check demo at https://www.codeproject.com/Articles/524878/Localisation-made-easy-for-WPF
        public EZLocalize Localization = null;

        // RPECK 26/03/2023
        // Default Directory for app
        public string configDir = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData), "Frontline Utilities LTD", "FLCleaner");

        // RPECK 25/03/2023
        // Language - used to determine the localization 
        public string Language
        {
            get { return language; }
            set { language = value; }
        }

        // RPECK 25/03/2023
        // CPUID Option -- this is used to determine whether the CPUID library should be invoked
        public bool CPUID {
            get { return cpuid; }
            set { cpuid = value; }
        }

        // RPECK 29/03/2023
        // Debug Option -- used to determine if the app should run in debug mode
        public bool Debug
        {
            get { return debug; }
            set { debug = value; }
        }

        // RPECK 25/03/2023
        // Load Settings from JSON
        // https://stackoverflow.com/a/54065208/1143732
        public void LoadFromFile(string filename = "config.json")
        {
        }

        // RPECK 25/03/2023
        // Save Settings to JSON
        // https://stackoverflow.com/a/54065208/1143732
        public void SaveToFile(string filename = "config.json")
        {
        }

    }
}
