using FrontLineGUI.Include.Interfaces;
using FrontLineGUI.Include.Services;
using System;
using System.Diagnostics;
using System.Windows;
using System.Windows.Input;

namespace FrontLineGUI
{
    public partial class SettingsViewModel : ViewModelBase
    {
        private readonly IAppConfig _config;
        private readonly ScanService _scanService;

        // RPECK 28/03/2026 - Public 
        public IAppConfig Config => _config;

        public ScanService ScanService => _scanService; // Expose the service directly so the View can bind to _scanService.IsScanning
        public ICommand ToggleSettingCommand { get; }

        public SettingsViewModel(IAppConfig config, ScanService scanService)
        {
            _config = config;
            _scanService = scanService;

            // Subscribe to notify the UI when the service state changes
            _scanService.StateChanged += OnScanStateChanged;

            // RPECK 28/03/2026 - Used to enable the "click" logic on the labels
            ToggleSettingCommand = new DelegateCommand(o => ExecuteToggleSetting(o));
        }

        private void OnScanStateChanged(ScanProcessState newState)
        {
            // Even though we aren't storing a local bool, we must tell 
            // WPF that the "ScanService" property (or its sub-properties) 
            // might have updated values.
            Application.Current.Dispatcher.BeginInvoke(() =>
            {
                OnPropertyChanged(nameof(ScanService));
            });
        }

        private void ExecuteToggleSetting(object parameter)
        {
            // Ensure the parameter is actually the string we expect
            if (parameter is not string propertyName || string.IsNullOrEmpty(propertyName))
                return;

            // We look at the Interface or the Implementation type
            // Since Config is an IAppConfig, we use that type for the property lookup
            var property = typeof(IAppConfig).GetProperty(propertyName);

            if (property != null && property.PropertyType == typeof(bool) && property.CanWrite)
            {
                try
                {
                    bool currentValue = (bool)property.GetValue(Config);
                    property.SetValue(Config, !currentValue);

                    // Note: Your AppConfig must call OnPropertyChanged for these 
                    // properties for the UI (ToggleSwitch) to flip visually.
                }
                catch (Exception ex)
                {
                    Debug.WriteLine($"Failed to toggle {propertyName}: {ex.Message}");
                }
            }
        }

    }
}