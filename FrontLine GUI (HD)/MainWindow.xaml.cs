using System;
using System.Windows;
using System.Windows.Input;
using System.Diagnostics; // for Process.Start

namespace FrontLineGUI
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {

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

        // Help Button
        // Takes to the FrontlineCleaner.com website
        private void HelpButton_Click(object sender, RoutedEventArgs e)
        {
            Process.Start(Properties.Settings.Default.WebSite);
        }

        // Scan Page
        // Shows the scan page (including results)
        private void ScanButton_Click(object sender, RoutedEventArgs e)
        {
            ContentFrame.Navigate(new Uri("Pages/Scan.xaml", UriKind.Relative));
        }

        // Extras Page
        // Shows the extras features
        private void ExtrasButton_Click(object sender, RoutedEventArgs e)
        {
            ContentFrame.Navigate(new Uri("Pages/Extras.xaml", UriKind.Relative));
        }

        // Backup Page
        // Shows the extras features
        private void BackupButton_Click(object sender, RoutedEventArgs e)
        {
            ContentFrame.Navigate(new Uri("Pages/Backup.xaml", UriKind.Relative));
        }

        // Statistics Page
        // Shows scan results etc

        private void StatisticsButton_Click(object sender, RoutedEventArgs e)
        {
            ContentFrame.Navigate(new Uri("Pages/Statistics.xaml", UriKind.Relative));
        }
    }
}
