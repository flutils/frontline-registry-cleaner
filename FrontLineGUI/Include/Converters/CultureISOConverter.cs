using System;
using System.Linq;
using System.Globalization;
using System.Windows.Data;

namespace FrontLineGUI
{
    // ButtonColour Converter
    // This is used to convert the background colour of buttons to be lighter or darker
    // https://wpf-tutorial.com/data-binding/value-conversion-with-ivalueconverter/

    public class CultureISOConverter : IValueConverter
    {

        // Convert
        // This is used to take the "background" image (xyz.png) and adds _hover before .png
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {

            // RPECK 24/03/2025 - Define a variable to store the value
            string newValue = value as string;

            // RPECK 24/03/2025 - Return the last value
            return newValue.Split('-').Last();

        }

        // ConvertBack
        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }

    }
}