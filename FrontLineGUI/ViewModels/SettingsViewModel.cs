using FrontLineGUI.Include.Interfaces;
using FrontLineGUI.Include.Services; 
using System.Windows.Input;
using System.Windows;

namespace FrontLineGUI
{
    public partial class SettingsViewModel : ViewModelBase
    {
        private readonly IAppConfig _config;
        private readonly ScanService _scanService;
        private bool _isScanning;

        public IAppConfig Config => _config;

        // This property returns True when the scanner is NOT busy
        public bool IsSettingsEnabled
        {
            get => !_isScanning;
        }

        public ICommand DebugClick { get; private set; }

        public SettingsViewModel(IAppConfig config, ScanService scanService)
        {
            _config = config;
            _scanService = scanService;

            // Subscribe to the service state changes
            _scanService.StateChanged += OnScanStateChanged;

            // Initial state check
            UpdateScanningState(_scanService.CurrentState);
        }

        private void OnScanStateChanged(ScanProcessState newState)
        {
            // Use the Dispatcher to ensure we update the UI property on the correct thread
            Application.Current.Dispatcher.BeginInvoke(() =>
            {
                UpdateScanningState(newState);
            });
        }

        private void UpdateScanningState(ScanProcessState state)
        {
            // We disable settings if it's Scanning or Paused
            _isScanning = (state == ScanProcessState.Scanning || state == ScanProcessState.Paused);

            // Notify the UI to refresh the IsSettingsEnabled binding
            OnPropertyChanged(nameof(IsSettingsEnabled));
        }
    }
}