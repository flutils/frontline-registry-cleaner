using System.Collections.Generic;
using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace FrontLineGUI
{
    /// <summary>
    ///   RPECK 05/02/2025 - Updated view system (moving away from frame/pages)
    ///   Ref: https://stackoverflow.com/a/15960843
    /// </summary>

    public abstract class ViewModelBase : INotifyPropertyChanged
    {

        // RPECK 06/02/2025 - Public Declarations
        public event PropertyChangedEventHandler PropertyChanged;

        // RPECK 06/02/2025 - Title
        // Used to output the title to display in the view
        public string Title { get; set; }

        protected void OnPropertyChanged(string propertyName)
        {
            OnPropertyChanged(new PropertyChangedEventArgs(propertyName));
        }

        protected virtual void OnPropertyChanged(PropertyChangedEventArgs e)
        {
            var handler = PropertyChanged;
            if (handler != null) handler(this, e);

        }

        // RPECK 23/02/2026 - SetProperty
        // Required to provide the means to change a core property of the view model
        protected bool SetProperty<T>(ref T field, T value, [CallerMemberName] string? name = null)
        {
            if(EqualityComparer<T>.Default.Equals(field, value)) return false;

            field = value;
            OnPropertyChanged(name);
            return true;
        }

    }
}
