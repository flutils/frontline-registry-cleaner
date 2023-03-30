using System.Windows.Controls;

namespace FrontLineGUI.Pages
{

    /* 
     *  RPECK 24/03/2023
     *  System to populate the 'settings' of the app - this is invoked at init with a separate class
     * 
     *  The following are options present in the app's settings: -
     *  
     *   - debug (should the app run in debug mode or not?)
     *   - language (which localization should the app be running under?)
     *   - backup (where should backups be stored?)
    */

    public partial class Settings : Page
    {
        public Settings()
        {
            DataContext = App.ConfigOptions;
            InitializeComponent();
        }
    }
}
