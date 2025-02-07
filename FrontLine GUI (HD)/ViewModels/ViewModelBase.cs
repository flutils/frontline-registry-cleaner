using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

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

    }
}
