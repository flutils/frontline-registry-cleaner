using System.Windows;

namespace FrontLineGUI
{

    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {

        // RPECK 18/03/2023
        // https://www.codeproject.com/Articles/524878/Localisation-made-easy-for-WPF
        private void Application_Startup(object sender, StartupEventArgs e)
        {

            // RPECK 05/02/2025 - Load up the MainWindow
            // Used to give us the ability to invoke dynamic content whilst maintaining state
            // Ref: https://stackoverflow.com/a/15960843
            var window = new MainWindow() { DataContext = new MainWindowViewModel() };

            // RPECK 05/02/2025 - Show the MainWindow class
            // Gives us the ability to manage its content/data without having other issues
            window.Show();

        }

    }

}
