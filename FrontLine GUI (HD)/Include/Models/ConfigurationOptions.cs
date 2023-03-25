using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection.Metadata.Ecma335;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace FrontLineGUI
{
    public class ConfigurationOptions
    {
        
        public ConfigurationOptions() {}

        //////////////
        //////////////
        // Public

        // RPECK 25/03/2023
        // Language - used to determine the localization 
        public string Language { get; set; }

        // RPECK 25/03/2023
        // CPUID Option -- this is used to determine whether the CPUID library should be invoked
        public bool CPUID { get; set; }

        // RPECK 25/03/2023
        // Load Settings from JSON
        // https://stackoverflow.com/a/54065208/1143732
        public void LoadFromFile(string filename)
        {

            


        }

        // RPECK 25/03/2023
        // Save Settings to JSON
        // https://stackoverflow.com/a/54065208/1143732
        public void SaveToFile(string filename)
        {

            


        }

        //////////////
        //////////////
        // Private

    }
}
