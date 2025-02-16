using System;
using System.Diagnostics;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

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

        // RPECK 09/02/2025 - FLWebURL 
        // Used to provide the value of the "Frontline" domain link to the view from the settings backend
        public string FLWebURL {

            get {
                return Properties.Settings.Default.FLWeb;
            }

        }

        private void clickTest(object sender, RoutedEventArgs e)
        {

            Debug.WriteLine("tester2");

        }

    }

}
