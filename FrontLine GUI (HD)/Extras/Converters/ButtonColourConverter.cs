using System;
using System.Drawing;
using System.Globalization;
using System.Windows.Data;

namespace FrontLineGUI
{
    // ButtonColour Converter
    // This is used to convert the background colour of buttons to be lighter or darker
    // https://wpf-tutorial.com/data-binding/value-conversion-with-ivalueconverter/

    public class ButtonColourConverter : IValueConverter
    {
        // Declarations
        float val;

        // Convert
        // This is used to take the "background" image (xyz.png) and adds _hover before .png
        public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            if (parameter == null || parameter.ToString() == string.Empty) {
                val = (float)0.5;
            } else {
                val =  float.Parse(parameter.ToString(), CultureInfo.InvariantCulture.NumberFormat);
            }

            Color color = ColorTranslator.FromHtml(value.ToString());
            Color newcolor = ChangeColorBrightness(color,val);

            return ColorTranslator.ToHtml(newcolor);
        }

        // ConvertBack
        public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            throw new NotImplementedException();
        }

        // Logic
        // https://gist.github.com/zihotki/09fc41d52981fb6f93a81ebf20b35cd5
        public static Color ChangeColorBrightness(Color color, float correctionFactor)
        {
            float red = (float)color.R;
            float green = (float)color.G;
            float blue = (float)color.B;

            if (correctionFactor < 0)
            {
                correctionFactor = 1 + correctionFactor;
                red *= correctionFactor;
                green *= correctionFactor;
                blue *= correctionFactor;
            }
            else
            {
                red = (255 - red) * correctionFactor + red;
                green = (255 - green) * correctionFactor + green;
                blue = (255 - blue) * correctionFactor + blue;
            }

            return Color.FromArgb(color.A, (int)red, (int)green, (int)blue);
        }

    }
}