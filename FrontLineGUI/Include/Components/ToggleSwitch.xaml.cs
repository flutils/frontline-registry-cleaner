using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace FrontLineGUI.Include.Components
{
    public partial class ToggleSwitch : UserControl
    {
        public ToggleSwitch()
        {
            InitializeComponent();

            // RPECK 28/03/2026 - Added to provide the ability to manage clicking on the labels
            this.Focusable = true;
        }

        // 1. The Wrapper Property
        public bool IsChecked
        {
            get => (bool)GetValue(IsCheckedProperty);
            set => SetValue(IsCheckedProperty, value);
        }

        // 2. The DependencyProperty Registration
        // MUST be public, static, and readonly.
        public static readonly DependencyProperty IsCheckedProperty =
            DependencyProperty.Register(
                nameof(IsChecked),
                typeof(bool),
                typeof(ToggleSwitch),
                new FrameworkPropertyMetadata(false, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault));

    }
}