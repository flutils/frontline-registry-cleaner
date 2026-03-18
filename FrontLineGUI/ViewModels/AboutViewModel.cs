using FrontLineGUI.Include.Classes.DB;
using FrontLineGUI.Include.Interfaces;
using FrontLineGUI.Include.Services;
using System.Windows;

namespace FrontLineGUI
{
    public partial class AboutViewModel : ViewModelBase
    {

        // RPECK 24/02/2026 - Services
        private readonly HardwareService _hardwareService;
        private int _selectedTabIndex;

        // RPECK 27/02/2026 - CPU/RAM Information
        // This is a service that allows us to manage how the CPU/RAM/HDD information is displayed
        public HardwareService Hardware => _hardwareService;
        public int SelectedTabIndex
        {
            get => _selectedTabIndex;
            set
            {
                _selectedTabIndex = value;
                OnPropertyChanged();
            }
        }

        public AboutViewModel(HardwareService hardwareService)
        {
            // Initialize the service. 
            // The service starts its own 1-second timer automatically.
            _hardwareService = hardwareService;

        }

        public override void OnNavigatedTo(object parameter)
        {
            if (parameter is int tabIndex)
            {
                SelectedTabIndex = tabIndex;
            }
        }

    }
}
