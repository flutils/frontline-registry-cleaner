using System;
using System.Globalization;
using System.Windows.Data;

namespace FrontLineGUI
{
    // RPECK 23/03/2025 - CapitalizeFirstLetterConverter
    // Used to give us the ability to translate text into having uppercase characters for each first word
    // --
    // Ref: https://stackoverflow.com/a/8027561
    // Ref: https://learn.microsoft.com/en-us/dotnet/api/system.globalization.textinfo.totitlecase?view=net-9.0
    [ValueConversion(typeof(string), typeof(string))]
    public class CapitalizeFirstLetterConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {

            // RPECK 23/03/2025 - Create new CultureInfo object (used to provide the ToTileCase function)
            string newValue = value as string;

            // RPECK 23/03/2025 - Return the newValue using ToTitleCase
            return CultureInfo.CurrentCulture.TextInfo.ToTitleCase(newValue);

        }
        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}
