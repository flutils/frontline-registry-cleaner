using FrontLineGUI.Include.Interfaces;
using FrontLineGUI.Include.Services;
using System;
using System.Windows;
using System.Windows.Input;

namespace FrontLineGUI
{
    public partial class SettingsViewModel : ViewModelBase, IDisposable
    {
        private readonly IAppConfig _config;
        private readonly ScanService _scanService;

        public IAppConfig Config => _config;

        // Expose the service directly so the View can bind to _scanService.IsScanning
        public ScanService ScanService => _scanService;

        public ICommand DebugClick { get; private set; }

        public SettingsViewModel(IAppConfig config, ScanService scanService)
        {
            _config = config;
            _scanService = scanService;

            // Subscribe to notify the UI when the service state changes
            _scanService.StateChanged += OnScanStateChanged;
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

        public void Dispose()
        {
            _scanService.StateChanged -= OnScanStateChanged;
        }
    }
}