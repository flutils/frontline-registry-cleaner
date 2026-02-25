using FrontLineGUI.Include.Services;
using FrontLineGUI.Include.Classes;
using System;
using System.Collections.ObjectModel;
using System.Windows.Input;
using System.Diagnostics;

namespace FrontLineGUI
{
    public class MainWindowViewModel : ViewModelBase
    {
        #region Declarations

        // RPECK 05/02/2025 - Declarations
        // Set as a private attributes so can be maintained internally
        private ViewModelBase _currentViewModel;
        private ObservableCollection<NavigationItem> _viewModels;

        private readonly INavigationService _navigation;

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

            // RPECK 06/02/2025 - Hook up Commands to associated methods
            _navigation = navigation;

            // RPECK 24/02/2026 - Set the "Current" view to the default one
            _navigation.Configure(vm => CurrentViewModel = vm);

            // Set starting page
            _navigation.NavigateTo<ScanViewModel>();

            // RPECK 24/02/2026 - Navigation Items
            // Used by the nav bar at the top to provide a simple way to manage how they are displayed and interact
            _viewModels = new ObservableCollection<NavigationItem>
            {
                new NavigationItem("Scan", typeof(ScanViewModel)),
                new NavigationItem("Settings", typeof(SettingsViewModel)),
                new NavigationItem("About", typeof(AboutViewModel))
            };

        }

        #region Methods

        private NavigationItem _selectedNavigationItem;
        public NavigationItem SelectedNavigationItem
        {
            get => _selectedNavigationItem;
            set
            {
                _selectedNavigationItem = value;
                OnPropertyChanged("Navigation");
                if (value != null)
                    _navigation.NavigateTo(value.ViewModelType);
            }
        }

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