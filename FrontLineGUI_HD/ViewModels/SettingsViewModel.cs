using FrontLineGUI.Include.Interfaces;

namespace FrontLineGUI
{
    public partial class SettingsViewModel : ViewModelBase
    {

        private readonly IAppConfig _config;

        public SettingsViewModel(IAppConfig config)
        {
            _config = config;
        }

    }

}
