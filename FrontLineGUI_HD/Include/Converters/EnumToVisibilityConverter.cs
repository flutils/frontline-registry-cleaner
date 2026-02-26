using System;
using System.Globalization;
using System.Windows;
using System.Windows.Data;

namespace FrontLineGUI
{
    public class EnumToVisibilityConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            // If the current state matches the parameter (e.g., "Ready"), show it
            if (value != null && parameter != null)
            {
                if (value.ToString() == parameter.ToString())
                {
                    return Visibility.Visible;
                }
            }
            return Visibility.Collapsed;
        }

        public object[] ConvertBack(object value, Type[] targetTypes, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }

        object IValueConverter.ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}