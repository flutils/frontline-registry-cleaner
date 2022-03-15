using System;
using System.Windows.Data;

namespace FrontLineGUI
{
    // ButtonHover Converter
    // This is used to convert the background image path from the button and allows us to make image buttons work much simpler
    // https://wpf-tutorial.com/data-binding/value-conversion-with-ivalueconverter/
    public class ButtonHoverConverter : IValueConverter
    {
        // Convert
        // This is used to take the "background" image (xyz.png) and adds _hover before .png
        public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            System.Diagnostics.Debug.Write(Properties.Resources.ResourceManager.GetObject("company_url"));
            return Properties.Resources.ResourceManager.GetObject("company_url");
        }

        // ConvertBack
        public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            throw new NotImplementedException();
        }

    }
}
