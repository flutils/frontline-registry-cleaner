using FrontLineGUI.Include.Classes;
using FrontLineGUI.Include.Interfaces;
using FrontLineGUI.Include.Services;
using System;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Windows;
using System.Windows.Forms;
using System.Windows.Input;

namespace FrontLineGUI
{
    public class MainWindowViewModel : ViewModelBase
    {
        #region Declarations

        // RPECK 05/02/2025 - Declarations
        // Set as a private attributes so can be maintained internally
        private ViewModelBase _currentViewModel;
        private ObservableCollection<NavigationItem> _viewModels;

        // RPECK 26/02/2026 - Navigation object
        // Used to inherit the navigation state from the global scope, and then populate as required
        private readonly INavigationService _navigation;
        private readonly IAppConfig _config;

        // RPECK 26/20/2026 - CurrentViewModel
        // Used for managing the navigation so that we're able to determine which viewmodel should be displayed
        public ViewModelBase CurrentViewModel
        {
            get => _currentViewModel;
            set
            {
                _currentViewModel = value;
                OnPropertyChanged(nameof(CurrentViewModel));
            }
        }


        // RPECK 06/02/2025 - Commands
        public ICommand NavigationButtonClick { get; private set; }
        public ICommand CloseCommand { get; }
        public ICommand MinimizeCommand { get; }
        public ICommand NavigateToLocalizationCommand { get; }
        public ICommand OpenUrlCommand { get; }

        // RPECK 21/02/2026 - Config
        public IAppConfig Config { get; }

        #endregion

        public MainWindowViewModel(INavigationService navigation, IAppConfig config)
        {

            // RPECK 06/02/2025 - Populate Core Variables
            _navigation = navigation;
            _config     = config;


            // RPECK 24/03/2026 - Commands
            // These were added to port away from the xaml.cs file
            CloseCommand                  = new DelegateCommand(o => ExecuteClose());
            MinimizeCommand               = new DelegateCommand(o => ExecuteMinimize());
            NavigateToLocalizationCommand = new DelegateCommand(o => _navigation.NavigateTo<SettingsViewModel>());
            OpenUrlCommand                = new DelegateCommand(o => ExecuteOpenUrl(o.ToString()));

            // RPECK 24/02/2026 - Navigation Items
            // Used by the nav bar at the top to provide a simple way to manage how they are displayed and interact
            _viewModels = new ObservableCollection<NavigationItem>
            {
                new NavigationItem("Scan", typeof(ScanViewModel)),
                new NavigationItem("Settings", typeof(SettingsViewModel)),
                new NavigationItem("About", typeof(AboutViewModel))
            };

            // RPECK 24/02/2026 - Set the "Current" view to the default one
            _navigation.Configure(vm =>
            {
                // 1. Update the actual view
                CurrentViewModel = vm;

                // 2. Synchronize the ListView selection
                // This finds the menu item that matches the new ViewModel type
                SelectedNavigationItem = _viewModels.FirstOrDefault(x => x.ViewModelType == vm.GetType());

            });

            // Set starting page
            _navigation.NavigateTo<ScanViewModel>();
            
        }

        #region Methods

        // RPECK 24/02/2026 - ViewModels
        // Used to populate the navigation area at the top of the main window
        public ObservableCollection<NavigationItem> ViewModels
        {
            get
            {
                if (_viewModels == null)
                    _viewModels = new ObservableCollection<NavigationItem>();

                return _viewModels;
            }
        }

        // RPECK 26/02/2026 - Navigation
        // Used to provide the means to traverse the navigation system 
        private NavigationItem? _selectedNavigationItem;
        public NavigationItem? SelectedNavigationItem
        {
            get => _selectedNavigationItem;
            set
            {
                if (_selectedNavigationItem == value) return;

                _selectedNavigationItem = value;
                OnPropertyChanged(nameof(SelectedNavigationItem));

                // RPECK - When the user selects a new item in the list, 
                // tell the service to update the "CurrentViewModel"
                if (_selectedNavigationItem != null)
                {
                    _navigation.NavigateTo(_selectedNavigationItem.ViewModelType);
                }
            }
        }

        // RPECK 06/02/2025 - Current Year
        // Used in the footer area (next to company name)
        public string CurrentYear
        {
            get { return DateTime.Now.Year.ToString(); }
        }

        // RPECK 06/02/2025 - Version
        // Used in the Github area cited below
        public string Version
        {
            get { return System.Reflection.Assembly.GetExecutingAssembly().GetName().Version.ToString(); }
        }

        // RPECK 24/03/2026 - Excecute URL
        // This was added to provide a single way to take users to different URL's (used mainly for the likes of Github etc)
        private void ExecuteOpenUrl(string url)
        {
            Process.Start(new ProcessStartInfo { FileName = url, UseShellExecute = true });
        }

        // RPECK 24/03/2026 - Close
        // Close the application (ported from xaml.cs)
        private void ExecuteClose()
        {
            var result = System.Windows.Forms.MessageBox.Show(
                "Are you sure you want to exit?", "Exit", MessageBoxButtons.YesNo, (MessageBoxIcon)MessageBoxImage.Warning);

            if (result == DialogResult.Yes)
                System.Windows.Application.Current.Shutdown();
        }

        // RPECK 24/03/2026 - Minimize
        // Ported from the xaml.cs file
        private void ExecuteMinimize()
        {
            System.Windows.Application.Current.MainWindow.WindowState = WindowState.Minimized;
        }

        #endregion

    }

}