using System;
using System.Windows;
using System.Windows.Input;
using System.Diagnostics; // for Process.Start
using System.Windows.Controls; // for MainButton_Click (referencing button)
using System.Windows.Media;
using System.ComponentModel;

namespace FrontLineGUI
{

    public partial class MainWindow : Window
    {

        // Main Ingression Point
        // This is where the app loads
        public MainWindow()
        { 

            // DataContext (for the version button)
            DataContext = this;

            // Init
            InitializeComponent();

        }

        // Version
        // Used to show the version number on the bottom menu area 
        // https://stackoverflow.com/a/15873711/1143732
        public string Version
        {
            get { return System.Reflection.Assembly.GetExecutingAssembly().GetName().Version.ToString(); }
        }

        // Year
        // Get the current year
        public string CurrentYear
        {
            get { return DateTime.Now.Year.ToString(); }
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

        // Close
        // Closes the Window (exits the application)
        private void CloseButton_Click(object sender, RoutedEventArgs e)
        {
            Close(); // https://www.tech-recipes.com/rx/23742/create-an-exit-button-in-c-visual-studio/
        }

        // Closing
        // Fired when the main Window is preparing to close
        private void Window_Closing(object sender, CancelEventArgs e)
        {
            // Close CPUID
            if (App.pSDK != null)
            {
                App.pSDK.Close();
                App.pSDK.DestroyInstance();
            }

        }

        // Minimize
        // Minimizes the window to the taskbar
        private void MinimizeButton_Click(object sender, RoutedEventArgs e)
        {
            this.WindowState = WindowState.Minimized; // https://stackoverflow.com/a/2841278/1143732
        }

        // Github Button
        // Takes the user to the GIT repo
        private void GithubButton_Click(object sender, RoutedEventArgs e)
        {
            // from .NET 6.0 onwards, need to define the following to open web browser
            // https://stackoverflow.com/questions/73113068/windows-forms-app-gives-me-error-when-i-use-process-start
            Process.Start(new ProcessStartInfo
                {
                    FileName = Properties.Settings.Default.Repo,
                    UseShellExecute = true
                }
            );
        }

        // Version Button
        // Takes us to the 'releases' Github URL
        private void VersionButton_Click(object sender, RoutedEventArgs e)
        {
            Process.Start(new ProcessStartInfo
                {
                    FileName = Properties.Settings.Default.Repo + "/releases",
                    UseShellExecute = true
                }
            );
        }

        // Settings Buttons
        // Loads the "settings" page
        public void SettingsButton_Click(object sender, RoutedEventArgs e)
        {
            // Elements
            Button srcButton  = e.Source as Button;
            StackPanel parent = FindName("SubActionButtons") as StackPanel;

            // Parameters
            // There are two parameters which can be handled here -- "Tab" and "Element"
            // These are used to identify which "Settings" tab should be loaded and which Element within that tab should be focused (IE tab -> General, element -> Debug)
            if (srcButton.CommandParameter != null)
            {
                Array value = srcButton.CommandParameter as Array;
                foreach (var item in value)
                {
                    Debug.Write(item.ToString());
                }
            }

            // Define Tag
            var tag = srcButton.Tag ?? "Settings";

            // Only proceed if tag is present
            if (tag != null)
            {

                // Count children
                int childrenCount = VisualTreeHelper.GetChildrenCount(parent);

                // Go through siblings
                if (childrenCount > 0)
                {
                    for (int i = 0; i < childrenCount; i++)
                    {
                        // Get sibling element
                        Button child = VisualTreeHelper.GetChild(parent, i) as Button;

                        // If element's tag is different to current, update the IsEnabled
                        if (child != null)
                        {
                            // Cast
                            String child_tag   = child.Tag.ToString() as String;
                            String current_tag = tag.ToString() as String;

                            // Enabled
                            child.IsEnabled = (child_tag != current_tag);
                        }

                    }

                }

                // Navigate the main content area
                ContentFrame.Navigate(new Uri($"Pages/{tag.ToString()}.xaml", UriKind.Relative));

            }

        }

    }
}
