using System;
using System.Globalization;
using System.Windows.Data;

namespace FrontLineGUI
{
    // Boolean Converter
    // This is used to derive a boolaen value from a varying input
    // For example, if we wanted to make a button enabled if there are more than 0 items in a collection, then we would use this

    public class NotEqualToOneBooleanConverter : IValueConverter
    {

        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {

            // RPECK 07/02/2025 - Convert value to integer so that we can accept boolean values
            int val = System.Convert.ToInt32(value);

            // RPECK 07/02/2025 - Return whether val is true or false
            return val > 0;

        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }

    }
}
