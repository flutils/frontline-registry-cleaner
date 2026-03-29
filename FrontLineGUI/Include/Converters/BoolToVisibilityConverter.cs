using System;
using System.Globalization;
using System.Windows;
using System.Windows.Data; // Essential for IValueConverter

namespace FrontLineGUI
{
    // Added : IValueConverter here
    internal class BoolToVisibilityConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value is bool isVisible)
            {
                // If the parameter "Invert" is passed, flip the logic
                if (parameter?.ToString() == "Invert")
                    isVisible = !isVisible;

                return isVisible ? Visibility.Visible : Visibility.Collapsed;
            }

            return Visibility.Collapsed;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value is Visibility vis)
            {
                bool result = vis == Visibility.Visible;

                if (parameter?.ToString() == "Invert")
                    return !result;

                return result;
            }
            return false;
        }
    }
}