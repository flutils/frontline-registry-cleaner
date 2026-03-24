using System.Windows;
using System.Windows.Controls;

namespace FrontLineGUI.Include.Controls
{
    public partial class ToggleSwitch : UserControl
    {
        public ToggleSwitch()
        {
            InitializeComponent();
        }

        // DependencyProperty to bind to ViewModel
        public static readonly DependencyProperty IsOnProperty =
            DependencyProperty.Register(
                nameof(IsEnabled),
                typeof(bool),
                typeof(ToggleSwitch),
                new PropertyMetadata(false));

        public bool IsEnabled
        {
            get => (bool)GetValue(IsOnProperty);
            set => SetValue(IsOnProperty, value);
        }
    }
}