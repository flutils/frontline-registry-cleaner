using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace FrontLineGUI
{
    /// <summary>
    /// RPECK 03/04/2023
    /// This was implemented to give us a basis from which to apply standardized fucntionality to classes that change properties
    /// https://stackoverflow.com/a/21866331/1143732
    /// </summary>
    public class PropertyChangedBase : INotifyPropertyChanged
    {
        // Public Method
        public event PropertyChangedEventHandler PropertyChanged;

        // Private Function
        protected void OnPropertyChanged([CallerMemberName] string name = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(name));
        }
    }

}
