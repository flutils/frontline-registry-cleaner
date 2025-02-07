using System;
using System.Windows;
using System.Windows.Input;
using System.Diagnostics; // for Process.Start
using System.Windows.Controls; // for MainButton_Click (referencing button)
using System.Windows.Media;
using System.Windows.Forms;
using static System.Windows.Forms.VisualStyles.VisualStyleElement.Window;
using System.Configuration;

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

        // Close
        // Closes the Window (exits the application)
        private void CloseButton_Click(object sender, RoutedEventArgs e)
        {

            MessageBoxButtons buttons = MessageBoxButtons.YesNo;
            DialogResult result;

            // Displays the MessageBox.
            result = System.Windows.Forms.MessageBox.Show("Are you sure that you would like to exit the application?", "Exit", buttons);
            if (result == System.Windows.Forms.DialogResult.Yes)
            { 
                Close(); // https://www.tech-recipes.com/rx/23742/create-an-exit-button-in-c-visual-studio/

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

        // Copyright Button
        // Takes us to the "FL" website
        private void CopyRightButton_Click(object sender, RoutedEventArgs e)
        {
            Process.Start(new ProcessStartInfo
                {
                    FileName = Properties.Settings.Default.FLWeb,
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

    }

}
