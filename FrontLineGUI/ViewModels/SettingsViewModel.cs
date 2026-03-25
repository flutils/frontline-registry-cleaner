using FrontLineGUI.Include.Interfaces;
using System.Windows.Input;

namespace FrontLineGUI
{
    public partial class SettingsViewModel : ViewModelBase
    {

        // RPECK 21/02/2026 - Config
        private readonly IAppConfig _config;
        public IAppConfig Config { get; }

        // RPECK24/03/2026 - Commands
        public ICommand DebugClick { get; private set; }

        public SettingsViewModel(IAppConfig config)
        {
            _config = config;


        }

    }

}
