using System;
using System.Globalization;
using System.Linq;
using System.Windows;
using System.Windows.Data;

namespace FrontLineGUI
{
    public class EnumToVisibilityConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value != null && parameter != null)
            {
                string currentState = value.ToString();
                string targetParameter = parameter.ToString();

                // Split by comma to handle multiple states (e.g., "Scanning,Paused")
                // Remove empty entries and trim whitespace to be safe
                var targetStates = targetParameter.Split(new[] { ',' }, StringSplitOptions.RemoveEmptyEntries)
                                                  .Select(s => s.Trim());

                // Check if our current state exists within the allowed list
                if (targetStates.Any(s => s.Equals(currentState, StringComparison.OrdinalIgnoreCase)))
                {
                    return Visibility.Visible;
                }
            }

            return Visibility.Collapsed;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}