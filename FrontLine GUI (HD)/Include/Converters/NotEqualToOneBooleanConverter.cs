using System;
using System.Diagnostics;
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
            return (int) value > 0;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }

    }
}
