using System.Linq;
using System.Windows;
using System.Windows.Input;

namespace FrontLineGUI.Include.Components
{
    /// <summary>
    /// Interaction logic for CustomMessageBox.xaml
    /// </summary>
    public partial class CustomMessageBox : Window
    {
        public CustomMessageBox(string message, string title)
        {
            InitializeComponent();
            txtMessage.Text = message;
            txtTitle.Text = title;
        }

        // The "Magic" Static Method
        public static bool? Show(string message, string title, Window owner = null)
        {

            var msgBox = new CustomMessageBox(message, title);

            // Find the active window (usually MainWindow)
            Window activeWindow = Application.Current.Windows.OfType<Window>().FirstOrDefault(x => x.IsActive)
                                  ?? Application.Current.MainWindow;

            if (activeWindow != null)
            {
                msgBox.Owner = activeWindow;

                // Try to find the 'OverlayDimmer' element in the parent window
                var dimmer = activeWindow.FindName("OverlayDimmer") as FrameworkElement;

                if (dimmer != null)
                {
                    dimmer.Visibility = Visibility.Visible;
                    // A tiny bit of Opacity makes it look better than pitch black
                    dimmer.Opacity = 0.4;
                }

                // ShowDialog blocks the code here until the user closes the message box
                bool? result = msgBox.ShowDialog();

                // Once closed, hide the dimmer again
                if (dimmer != null)
                {
                    dimmer.Visibility = Visibility.Collapsed;
                }

                return result;
            }

            return msgBox.ShowDialog();
        }

        private void Window_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.ChangedButton == MouseButton.Left)
            {
                this.DragMove();
            }
        }

        private void BtnOk_Click(object sender, RoutedEventArgs e)
        {
            this.DialogResult = true;
            this.Close();
        }

        private void BtnCancel_Click(object sender, RoutedEventArgs e)
        {
            this.DialogResult = false;
            this.Close();
        }
    }

}