using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;

namespace FrontLineGUI.Extras.Components
{
    /// <summary>
    /// Interaction logic for QuickLinks.xaml
    /// </summary>
    public partial class QuickLinks : UserControl
    {
        public QuickLinks()
        {
            InitializeComponent();
        }

        // Quick Links
        // Click handler for each button
        private void QuickLinksButton_Click(object sender, RoutedEventArgs e)
        {
            Button srcButton = e.Source as Button;
            string tag = srcButton.Tag.ToString();
            string page = "";
            if (!String.IsNullOrEmpty(tag))
            {
                // Behaviour/Pages
                switch (tag)
                {
                    case "Clean":
                        page = "Scan";
                        break;

                    case "Junk":
                        page = "Scan";
                        break;

                    case "Defragment":
                        page = "Extras";
                        break;

                    case "Startup":
                        page = "Extras";
                        break;
                }

                // https://stackoverflow.com/a/26969093/1143732
                if (!String.IsNullOrEmpty(page))
                {
                    NavigationService nav = NavigationService.GetNavigationService(this);
                    nav.Navigate(new Uri($"Pages/{page}.xaml", UriKind.Relative));
                }

            }
        }
    }
}
