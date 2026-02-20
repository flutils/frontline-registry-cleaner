using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Threading;
using System.Windows.Input;

namespace FrontLineGUI
{
    public class MainWindowViewModel : ViewModelBase
    {
        #region Declarations

        // RPECK 05/02/2025 - Declarations
        // Set as a private attributes so can be maintained internally
        private ViewModelBase _currentViewModel;
        private List<ViewModelBase> _viewModels;

        // RPECK 06/02/2025 - Commands
        public ICommand NavigationButtonClick { get; private set; }

        #endregion

        public MainWindowViewModel()
        {

            // RPECK 13/02/2026 - Config File
            // These can be inferred from a config.toml file placed in the same directory as the binary, otherwise the default options will be used
            // --
            // Ref: https://mojoauth.com/parse-and-generate-formats/parse-and-generate-toml-with-aspnet-core#reading-and-deserializing-toml-configuration 
            if (File.Exists("config.toml"))
            {

                // RPECK 13/02/2026 - Pull in the config.toml file and compute the values it has inside
                // Available Options: -
                // - debug      - BOOLEAN (whether the app should run in debug mode)
                // - config_dir - STRING (the directory where the database should be stored // either relative or absolute)
                // - language   - ENUM (en, fr)
                // - cpu_id     - BOOLEAN (whether we should use the CPUID functionality to load the performance of the system)
                var tomlContent = File.ReadAllText("config.toml");

                // RPECK 13/02/2026 - Interpolate the file using the Tomlyn library
                // This was loaded into NuGet and is generally used to proces TOML files inside C#
                var document = Tomlyn.Toml.Parse(tomlContent);


                //var settings = document.Deserialize<ConfigurationOptions>();


            }

            // RPECK 06/02/2025 - ViewModels
            // Used to populate different views inside the system (Scan, Settings & About)
            ViewModels.Add(new ScanViewModel(new ScanView() { }));
            ViewModels.Add(new SettingsViewModel(new SettingsView() { }));
            ViewModels.Add(new AboutViewModel(new AboutView() { }));


            CultureInfo culture;

            culture = CultureInfo.CreateSpecificCulture("en");

            Thread.CurrentThread.CurrentCulture = culture;
            Thread.CurrentThread.CurrentUICulture = culture;

            // Set starting page
            CurrentViewModel = ViewModels[0];

            // RPECK 06/02/2025 - Get the base view, which is 'scan'
            LoadView(CurrentViewModel);

            // RPECK 06/02/2025 - Hook up Commands to associated methods
            NavigationButtonClick = new DelegateCommand(o => LoadView((ViewModelBase)o));

        }

        #region Public Properties

        public ViewModelBase CurrentViewModel
        {
            get { return _currentViewModel; }
            set
            {
                _currentViewModel = value;
                OnPropertyChanged("CurrentViewModel");
            }
        }

        public List<ViewModelBase> ViewModels
        {
            get
            {
                if (_viewModels == null)
                    _viewModels = new List<ViewModelBase>();

                return _viewModels;
            }
        }

        #endregion

        #region Actions

        // RPECK 06/02/2025 - LoadView
        // This is used to give us the ability to change the views as per the user's request
        // --
        // Ref: https://stackoverflow.com/a/15960843
        private void LoadView(ViewModelBase viewModel)
        {

            if (!ViewModels.Contains(viewModel)) ViewModels.Add(viewModel);

            CurrentViewModel = ViewModels.FirstOrDefault(vm => vm == viewModel);
        }

        #endregion

        #region Methods

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