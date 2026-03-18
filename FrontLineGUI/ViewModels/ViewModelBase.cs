using System.Collections.Generic;
using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace FrontLineGUI
{
    /// <summary>
    /// RPECK 05/02/2025 - Updated view system (moving away from frame/pages)
    /// Ref: https://stackoverflow.com/a/15960843
    /// </summary>
    public abstract class ViewModelBase : INotifyPropertyChanged
    {
        // Event
        public event PropertyChangedEventHandler PropertyChanged;

        // Navigation hook
        public virtual void OnNavigatedTo(object parameter) { }

        // Core PropertyChanged trigger (single, correct implementation)
        protected void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        // Optional explicit version (only if you REALLY need it)
        protected void OnPropertyChanged(PropertyChangedEventArgs args)
        {
            PropertyChanged?.Invoke(this, args);
        }

        // Standard SetProperty helper
        protected bool SetProperty<T>(ref T field, T value, [CallerMemberName] string name = null)
        {
            if (EqualityComparer<T>.Default.Equals(field, value))
                return false;

            field = value;
            OnPropertyChanged(name);
            return true;
        }
    }
}