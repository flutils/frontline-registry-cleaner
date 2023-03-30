using JCS;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Diagnostics;
using System.Linq;
using System.Runtime.CompilerServices;

namespace FrontLineGUI
{

    // RPECK 30/03/2023
    // This was created to help us get the ObservationCollection to work
    // https://github.com/Ramaloke/WPF-MVVM-Example

    public class ScanViewModel : INotifyPropertyChanged
    {

        // Public class vars
        public CPUUtilization CPUInfo;
        public ObservableCollection<ScanItem> ScanItemsObject { get; set; }

        public ScanViewModel()
        {
            // Define CPUInfo
            CPUInfo = new CPUUtilization(0, 10, 0, 0);

            // RPECK 26/03/2023
            // Scan Items Collection - used to provide a list of the items to be scanned
            ScanItemsObject = new ObservableCollection<ScanItem>()
                {
                    new ScanItem("Registry Errors", "Clean registry errors.", true, "/Resources/Scan/registry_errors.png"),
                    new ScanItem("Registry Errors", "Clean registry errors.", true, "/Resources/Scan/registry_errors.png")
                };

        }


        // PropertyChanged
        // This is required for the INotifyPropertyChanged system to process changes
        #region PropertyChanged

            public event PropertyChangedEventHandler PropertyChanged;
            public void OnPropertyChanged([CallerMemberName] string propertyName = null)
            {
                if(PropertyChanged != null) PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
            }

        #endregion

    }
}
