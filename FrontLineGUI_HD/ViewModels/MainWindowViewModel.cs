using FrontLineGUI.Include.Services;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Windows.Input;

namespace FrontLineGUI
{
    public class MainWindowViewModel : ViewModelBase
    {
        #region Declarations

        // RPECK 05/02/2025 - Declarations
        // Set as a private attributes so can be maintained internally
        private ViewModelBase _currentViewModel;
        private ObservableCollection<Type> _viewModels;

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


           _viewModels = new ObservableCollection<Type>
            {
                typeof(ScanViewModel),
                typeof(SettingsViewModel),
                typeof(AboutViewModel)
            };

        }

        #region Methods

        // RPECK 24/02/2026 - ViewModels
        // Used to populate the navigation area at the top of the main window
        public ObservableCollection<Type> ViewModels
        {
            get
            {
                if (_viewModels == null)
                    _viewModels = new ObservableCollection<Type>();

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