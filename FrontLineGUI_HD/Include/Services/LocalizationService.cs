using System.Globalization;
using System.Resources;

namespace FrontLineGUI
{

    /// <summary>
    /// RPECK 09/02/2025 - Added to give us the ability to hook into the native localization handling of WPF/C# to provide the means to switch languages
    /// Ref: https://medium.com/@PoulLorca/easy-localization-a-simple-guide-to-internationalizing-your-wpf-app-33b210a7cff2
    /// </summary>

    public class LocalizationService
    {
        private ResourceManager _resourceManager;
        private CultureInfo _currentCulture;

        public LocalizationService()
        {
            _resourceManager = new ResourceManager("FrontlineGUI.Resources.Strings", typeof(LocalizationService).Assembly);
            _currentCulture = CultureInfo.CurrentCulture;
        }

        public string GetString(string key)
        {
            return _resourceManager.GetString(key, _currentCulture);
        }

        public void SetCulture(string cultureCode)
        {
            _currentCulture = new CultureInfo(cultureCode);
        }
    }
}
