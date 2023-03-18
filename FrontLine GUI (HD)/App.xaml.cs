using EZLocalizeNS;
using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;

namespace FrontLineGUI
{

    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {

        // RPECK 18/03/2023
        // Added to give us the means to change localization (for string interpolation) on app load
        public static EZLocalizeNS.EZLocalize MyEZLocalize = null;

        // RPECK 18/03/2023
        // https://www.codeproject.com/Articles/524878/Localisation-made-easy-for-WPF
        private void Application_Startup(object sender, StartupEventArgs e)
        {
            //MyEZLocalize = new EZLocalize(App.Current.Resources, "en-GB", null, "Localization\\", "Strings");

            // EZ
            //EZLocalize.ChangeLanguage(System.Globalization.CultureInfo.CurrentCulture.TwoLetterISOLanguageName);
        }

    }

}
