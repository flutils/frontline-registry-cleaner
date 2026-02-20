using System;
using System.Globalization;
using System.IO;

namespace FrontLineGUI
{
    public class ConfigurationOptions : PropertyChangedBase
    {

        // Private Options
        // Used to hold the values within the system that can be manipulated at RunTime
        private bool debug;
        private bool cpuid;
        private string language;

        // Constructor
        // Used to instantiate the class (IE define different options when it's loaded)
        public ConfigurationOptions()
        {

            // RPECK 23/03/2025 - Language
            // Set the language to use within the system
            language = "en";

        }

        // RPECK 26/03/2023
        // Default Directory for app
        public string configDir = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData), "Frontline Utilities LTD", "FLCleaner");

        // RPECK 25/03/2023
        // Language - used to determine the localization 
        // Updates the CurrentCulture value to ensure we have the means to hotswap values
        public string Language
        {
            get {

                // RPECK 23/03/2025 - Return the present language (defaults to 'en')
                return language;
            
            }
            set {
                
                // RPECK 23/03/2025 - Update the language if it is not the same as present value
                if(language != value) {
                    
                    language = value;

                    //System.Threading.Thread.CurrentThread.CurrentUICulture = CultureInfo.GetCultureInfo("fr");

                }
            
            }
        }

        // RPECK 25/03/2023
        // CPUID Option -- this is used to determine whether the CPUID library should be invoked
        public bool CPUID
        {
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

    }
}
