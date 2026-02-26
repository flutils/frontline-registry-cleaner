using FrontLineGUI.Include.Classes;
using FrontLineGUI.Include.Services;
using System;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Linq;
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

        // RPECK 21/02/2026 - Config
        public IAppConfig Config { get; }

        #endregion

        public MainWindowViewModel(INavigationService navigation)
        {

            // RPECK 24/02/2026 - Navigation Items
            // Used by the nav bar at the top to provide a simple way to manage how they are displayed and interact
            _viewModels = new ObservableCollection<NavigationItem>
            {
                new NavigationItem("Scan", typeof(ScanViewModel)),
                new NavigationItem("Settings", typeof(SettingsViewModel)),
                new NavigationItem("About", typeof(AboutViewModel))
            };

            // RPECK 06/02/2025 - Hook up Commands to associated methods
            _navigation = navigation;

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

        #endregion

    }

}