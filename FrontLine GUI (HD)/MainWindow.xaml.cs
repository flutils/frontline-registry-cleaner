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
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        // MouseDown
        // Drags entire window/form
        // https://stackoverflow.com/a/7418629/1143732
        private void Window_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.ChangedButton == MouseButton.Left)
                this.DragMove();
        }
    }

    // Colour / Brush Converter
    // This is used to conver the Settings.MainBackground colour to a brush object
    // Whilst unnecessary, it's a good way to standardize the system
    // https://docs.microsoft.com/en-us/dotnet/desktop/wpf/data/how-to-convert-bound-data?view=netframeworkdesktop-4.8
    // https://gist.github.com/bingelp/10011323
    [ValueConversion(typeof(System.Drawing.Color), typeof(SolidColorBrush))]
    public class SystemColorToSolidBrushConverter : IValueConverter
    {

        public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            System.Drawing.Color color = (System.Drawing.Color)value;
            System.Windows.Media.Color converted = Color.FromArgb(color.A, color.R, color.G, color.B);
            return new SolidColorBrush(converted);
        }

        public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            throw new NotImplementedException();
        }

        public static System.Drawing.Color RGBToColor(string rgb)
        {

            //Trim to RRGGBB
            if (rgb.Length > 6)
            {
                rgb = rgb.Substring(rgb.Length - 6);
            }

            if (rgb.Length != 6)
                throw new ArgumentException("Invalid rgb value given");

            int red = 0;
            int green = 0;
            int blue = 0;

            red = System.Convert.ToInt32(rgb.Substring(0, 2), 16);
            green = System.Convert.ToInt32(rgb.Substring(2, 2), 16);
            blue = System.Convert.ToInt32(rgb.Substring(4, 2), 16);


            return System.Drawing.Color.FromArgb(red, green, blue);
        }

        public static string ColorToRGB(System.Drawing.Color color)
        {
            string red = color.R.ToString("X2");
            string green = color.G.ToString("X2");
            string blue = color.B.ToString("X2");
            return String.Format("{0}{1}{2}", red, green, blue);
        }

        public static System.Windows.Media.Color ColorToColor(System.Drawing.Color color)
        {
            return Color.FromArgb(color.A, color.R, color.G, color.B);
        }

        public static System.Drawing.Color ColorToColor(System.Windows.Media.Color color)
        {
            return System.Drawing.Color.FromArgb(color.A, color.R, color.G, color.B);
        }
    }

}
