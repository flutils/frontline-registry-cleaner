using System;
using System.Diagnostics;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;

namespace FrontLineGUI
{
    /// <summary>
    /// Interaction logic for About.xaml
    /// </summary>
    public partial class AboutView : UserControl
    {

        public AboutView()
        {
            InitializeComponent();

        }

        // RPECK 23/03/2025 - External Link Click
        // Used to provide the means to open a URL in a web browser
        private void ExternalLinkClick(object sender, RequestNavigateEventArgs e)
        {

            // from .NET 6.0 onwards, need to define the following to open web browser
            // https://stackoverflow.com/questions/73113068/windows-forms-app-gives-me-error-when-i-use-process-start
            Process.Start(new ProcessStartInfo
                {
                    FileName = e.Uri.AbsoluteUri,
                    UseShellExecute = true
                }
            );

        }

        private void MiB_Click(object sender, RoutedEventArgs e)
        {
            // Example: Open a website or show a message
            Process.Start(new ProcessStartInfo
            {
                FileName = Properties.Settings.Default.MadeInBritain,
                UseShellExecute = true
            });
        }

    }

}
