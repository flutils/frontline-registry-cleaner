using System;
using System.Windows;
using System.Windows.Input;
using System.Diagnostics; // for Process.Start
using System.Windows.Controls; // for MainButton_Click (referencing button)
using System.Windows.Media;

namespace FrontLineGUI
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {

        // Vars
        private Brush maxbutton_bg;

        // Main Ingression Point
        // This is where the app loads
        public MainWindow()
        {
            // Init
            InitializeComponent();

            // DataContext (for the version button)
            this.DataContext = this;

            // Vars
            this.maxbutton_bg = MaximizeButton.Background;
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

        // Maximize
        // Maximizes the window to the taskbar
        private void MaximizeButton_Click(object sender, RoutedEventArgs e)
        {

            // Maximized
            if (this.WindowState == WindowState.Maximized) {

                this.WindowState = WindowState.Normal;
                this.SizeToContent = SizeToContent.WidthAndHeight;
                MaximizeButton.Background = this.maxbutton_bg;

            } else {

                this.SizeToContent = SizeToContent.Manual;
                this.WindowState   = WindowState.Maximized;
                MaximizeButton.Background = (SolidColorBrush)new BrushConverter().ConvertFromString("#1fde57");

            }
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

        // Main Buttons (Left Hand Menu)
        // Loads different pages depending on which radiobutton was checked
        private void MainButton_Click(object sender, RoutedEventArgs e)
        {
            RadioButton srcButton = e.Source as RadioButton;
            string tag = srcButton.Tag.ToString();
            if(!String.IsNullOrEmpty(tag)) ContentFrame.Navigate(new Uri($"Pages/{tag}.xaml", UriKind.Relative));
        }

        // Settings Button
        // Loads the "settings" page
        private void SettingsButton_Click(object sender, RoutedEventArgs e)
        {
            ContentFrame.Navigate(new Uri("Pages/Settings.xaml", UriKind.Relative));
        }
    }
}
