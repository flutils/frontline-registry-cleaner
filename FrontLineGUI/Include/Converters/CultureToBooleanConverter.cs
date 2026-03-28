using System;
using System.Globalization;
using System.Windows.Data;

namespace FrontLineGUI
{
    // Culture Boolean Converter
    // Used in the "Settings" view to pre-select a language based on the one that's currently assigned in the system

    public class CultureToBooleanConverter : IMultiValueConverter
    {
        public object Convert(object[] values, Type targetType, object parameter, CultureInfo culture)
        {
            // values[0] = Config.CurrentLanguage (string)
            // values[1] = This item's Name (string)
            if (values.Length < 2 || values[0] == null || values[1] == null)
                return false;

            return values[0].ToString() == values[1].ToString();
        }

        public object[] ConvertBack(object value, Type[] targetTypes, object parameter, CultureInfo culture)
        {
            // Not strictly needed for RadioButtons if we handle the change via Command, 
            // but if you want Two-Way binding to work:
            return new object[] { Binding.DoNothing, Binding.DoNothing };
        }
    }

}