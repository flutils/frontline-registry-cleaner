using System;
using System.Windows;
using System.Windows.Input;
using System.Diagnostics; // for Process.Start
using System.Windows.Controls; // for MainButton_Click (referencing button)
using System.Windows.Media;
using System.Windows.Media.Effects;

namespace FrontLineGUI
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {

        // Vars
        public EZLocalizeNS.EZLocalize MyEZLocalize = FrontLineGUI.App.MyEZLocalize;

        // Main Ingression Point
        // This is where the app loads
        public MainWindow()
        {
            // Init
            InitializeComponent();

            // DataContext (for the version button)
            this.DataContext = this;

        }

        // Version
        // Used to show the version number on the left menu area 
        // https://stackoverflow.com/a/15873711/1143732
        public string Version
        {
            get { return $"v {System.Reflection.Assembly.GetExecutingAssembly().GetName().Version.ToString()}"; }
        }

        // Load
        // Makes the form default to the top of the Window stack
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            this.Topmost = true;
            this.Topmost = false;
        }

        // MouseDown
        // Drags entire window/form
        // https://stackoverflow.com/a/7418629/1143732
        private void Window_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.ChangedButton == MouseButton.Left) this.DragMove();
        }

        // Close
        // Closes the Window (exits the application)
        private void CloseButton_Click(object sender, RoutedEventArgs e)
        {
            this.Close(); // https://www.tech-recipes.com/rx/23742/create-an-exit-button-in-c-visual-studio/
        }

        // Minimize
        // Minimizes the window to the taskbar
        private void MinimizeButton_Click(object sender, RoutedEventArgs e)
        {
            this.WindowState = WindowState.Minimized; // https://stackoverflow.com/a/2841278/1143732
        }

        // FL Button
        // This loads up the Frontline web interface. The URL may change, so it's stored as a Setting (accessed via "Properties" of the WPF project
        private void FrontlineButton_Click(object sender, RoutedEventArgs e)
        {
            Process.Start(Properties.Settings.Default.FLWeb);
        }

        // Version Button
        // Takes the user to the GIT repo
        private void VersionButton_Click(object sender, RoutedEventArgs e)
        {
            Process.Start(Properties.Settings.Default.Repo);
        }

        // Settings Button
        // Loads the "settings" page
        private void SettingsButton_Click(object sender, RoutedEventArgs e)
        {

            ContentFrame.Navigate(new Uri("Pages/Settings.xaml", UriKind.Relative));

            // RPECK 18/03/2023
            // Updated this functionality to use a modal dialog
            // https://stackoverflow.com/a/42933694/1143732

            //ModalWindow modalWindow = new ModalWindow(new Uri("Pages/Settings.xaml", UriKind.Relative));
        }

        // Localization Button
        // This loads the localization modal (which should be in the settings pane)
        private void LocalizationButton_Click(object sender, RoutedEventArgs e)
        {

        }
    }
}
