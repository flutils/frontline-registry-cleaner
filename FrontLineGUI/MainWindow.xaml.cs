using System.Diagnostics; // for Process.Start
using System.Windows;
using System.Windows.Forms;
using System.Windows.Input;

namespace FrontLineGUI
{

    public partial class MainWindow : Window
    {

        // Main Ingression Point
        // This is where the app loads
        public MainWindow()
        {

            // Init
            InitializeComponent();

        }

        // RPECK 07/03/2023
        // Added to give us the means to display the current flag
        public string LanguageFlag
        {
            get
            {
                string language = "en-GB";// App.ConfigOptions.Localization.CurrentLanguage;
                return $"/Resources/Localization/Icons/{language}.jpg";
            }
        }

        // Load
        // Makes the form default to the top of the Window stack
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            Topmost = true;
            Topmost = false;
        }

        // MouseDown
        // Drags entire window/form
        // https://stackoverflow.com/a/7418629/1143732
        private void Window_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.ChangedButton == MouseButton.Left) DragMove();
        }

    }

}
