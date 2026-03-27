using FrontLineGUI.Include.Classes.DB;
using FrontLineGUI.Include.Services;
using FrontLineGUI.Include.Interfaces;
using Microsoft.Extensions.DependencyInjection;
using System;
using System.Globalization;
using System.Linq;
using System.Windows;
using Velopack;
using NavigationService = FrontLineGUI.Include.Services.NavigationService;

namespace FrontLineGUI
{

    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {

        // RPECK 21/02/2026 - Services
        // Used to bring in Configuration Options from global scope

        public static IServiceProvider Services { get; private set; }

        // RPECK 22/02/2026 - VeloPack
        // This was added to integrate the VeloPack library (used for installation / update management)
        // --
        // Ref: https://docs.velopack.io/getting-started/csharp?platform=wpf
        [STAThread]
        public static void Main(string[] args)
        {   
            
            // RPECK 24/02/2026 - Services
            // Various Globally-Scoped items which are used to provide the means to access them outside of the local scope
            var services = new ServiceCollection();

            // RPECK 24/02/2026 - Add the various services required by the app
            // This gives us the ability to manage each of the services from within other scopes
            services.AddSingleton<ScanService>();     // RPECK 24/02/2026 - Set up a new scan (IE when the application loads, invoke a new instance of the Scan object)
            services.AddDbContext<AppDbContext>();    // RPECK 24/02/2026 - Set up the databsae (this requires ensuring the db file is accessible)
            services.AddSingleton<HardwareService>(); // RPECK 01/03/2026 - Set up RAM/CPU/HDD management
            services.AddSingleton<IAppConfig, AppConfig>();


            // RPECK 24/02/2026 - Navigation
            // Extracted from ViewModels to provide the means to manage how each of the views should display
            services.AddSingleton<MainWindowViewModel>();
            services.AddSingleton<INavigationService, NavigationService>();

            // Register ViewModels
            services.AddTransient<ScanViewModel>();
            services.AddTransient<SettingsViewModel>();
            services.AddTransient<AboutViewModel>();

            // RPECK 24/02/2026 - Services
            // This is the main Services attribute that can be used within the application
            Services = services.BuildServiceProvider();

            // RPECK 02/03/2026 - Initialize Database
            // This is done to ensure we have the required settings set up 
            InitializeDatabase(Services);

            // RPECK 01/03/2026 - Default Language
            // Required to ensure we are only supporting English or French (can expand later)
            var config = Services.GetRequiredService<IAppConfig>();

            // If the user's OS is Spanish, but we only support EN/FR:
            if (!config.SupportedLanguages.Any(c => c.Name == config.CurrentLanguage))
                config.CurrentLanguage = "en-GB";

            // RPECK 01/03/2026 - Set the initial culture (text) of the application
            // Required to ensure we are defaulting to either English or French
            var culture = new CultureInfo(config.CurrentLanguage);
            CultureInfo.DefaultThreadCurrentCulture = culture;
            CultureInfo.DefaultThreadCurrentUICulture = culture;

            // RPECK 22/02/2026 - Velopack Integration
            // Uses the core Velopack recommended code from their samples repo
            // --
            // Ref: https://github.com/velopack/velopack/blob/develop/samples/CSharpWpf/App.xaml.cs
            try
            {
                // It's important to Run() the VelopackApp as early as possible in app startup.
                VelopackApp.Build().Run();

                // RPECK 22/02/2026 - Set up the "App" environment (this is required to ensure that everything else works)
                // Basically required to ensure we are running everything that we need
                var app = new App();
                app.InitializeComponent();

                // RPECK 05/02/2025 - Load up the MainWindow
                // Used to give us the ability to invoke dynamic content whilst maintaining state
                // Ref: https://stackoverflow.com/a/15960843
                var window = new MainWindow() { DataContext = App.Services.GetRequiredService<MainWindowViewModel>() };

                // RPECK 05/02/2025 - Show the MainWindow class
                // Gives us the ability to manage its content/data without having other issues
                app.Run(window);

            }
            catch (Exception ex)
            {

                // RPECK 23/02/2026 - This was added to provide an exception in case of the application not loading properly
                MessageBox.Show("Unhandled exception: " + ex.ToString());

            }

        }

        // RPECK 02/03/2026 - Initiaze SQLite Database
        // This isn't strictly necessary but felt that it would add some value as regards ensuring we had
        private static void InitializeDatabase(IServiceProvider serviceProvider)
        {
            using (var scope = serviceProvider.CreateScope())
            {
                var db = scope.ServiceProvider.GetRequiredService<AppDbContext>();

                // 1. Check if the DB exists and create it if not
                // This will also apply your EF models to the SQLite file
                db.Database.EnsureCreated();

            }
        }

    }

}
