using Microsoft.VisualBasic.Logging;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.Reflection;
using System.Windows.Controls;
using System.Windows.Data;

namespace FrontLineGUI
{
    // Boolean Converter
    // This is used to derive a boolaen value from a varying input
    // For example, if we wanted to make a button enabled if there are more than 0 items in a collection, then we would use this

    public class ListNotfullConverter : IValueConverter
    {

        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {

            // RPECK 30/12/2023 - This was added to enable us to manage the Select All button for the list view
            // --
            // Source: https://www.codeproject.com/Articles/28006/Using-converters-to-aggregate-a-list-in-a-ListView
            Type ValueType = parameter.GetType();

            // RPECK 30/12/2023 - Check if the ValueType is equal to ListView
            if (ValueType.Name == typeof(ListView).Name)
            {

                // RPECK 30/12/2023 - If the parameter is a ListView type, then get the items
                ListView items = (ListView)parameter;

                // RPECK 30/12/2023 - Return whether the selected value is below the total count 
                return (int)value < items.Items.Count;

            }

            // RPECK 30/12/2023 - If the 
            return 0;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }

    }
}
