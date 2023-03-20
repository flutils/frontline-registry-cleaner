using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection.Metadata.Ecma335;
using System.Text;
using System.Threading.Tasks;

namespace FrontLineGUI
{
    class ConfigurationOptions
    {

        // RPECK 18/03/2023
        // ConfigurationOptions - used to populate 'settings' within the application
        
        // Language - used to denote which language the system is using
        private string language;

        public ConfigurationOptions(string Language = "en")
        {
            language = Language;
        }



    }
}
