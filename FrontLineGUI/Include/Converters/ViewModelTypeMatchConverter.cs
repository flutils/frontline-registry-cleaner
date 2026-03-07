using System;
using System.Globalization;
using System.Windows.Data;

namespace FrontLineGUI
{
    public class ViewModelTypeMatchConverter : IMultiValueConverter
    {
        public object Convert(object[] values, Type targetType, object parameter, CultureInfo culture)
        {
            if (values.Length < 2 || values[0] == null || values[1] == null)
                return false;

            var currentVm = values[0];      // This will be CurrentViewModel
            var targetTypeItem = values[1] as Type; // This will be the NavigationItem's Type

            return currentVm.GetType() == targetTypeItem;
        }

        public object[] ConvertBack(object value, Type[] targetTypes, object parameter, CultureInfo culture)
        {
            return null;
        }
    }

}
