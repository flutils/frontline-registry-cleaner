using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace FrontLineGUI
{
    /// <summary>
    /// Interaction logic for Scan.xaml
    /// </summary>
    public partial class Scan : Page
    {
        public Scan()
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
                if (!String.IsNullOrEmpty(page)) NavigationService.Navigate(new Uri($"Pages/{page}.xaml", UriKind.Relative));

            }
        }
    }
}
