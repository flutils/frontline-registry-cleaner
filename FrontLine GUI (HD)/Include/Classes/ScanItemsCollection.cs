using System.Collections.ObjectModel;
using System.Collections.Specialized;
using System.Diagnostics;
using System.Linq;
using System.Runtime.CompilerServices;

namespace FrontLineGUI
{

    // https://learn.microsoft.com/en-us/dotnet/desktop/wpf/data/how-to-create-and-bind-to-an-observablecollection?view=netframeworkdesktop-4.8
    public class ScanItemsCollection : ObservableCollection<ScanItem>
    {
        public ScanItemsCollection() : base()
        {}

        // RPECK 28/03/2023
        // Get the number of selected items from the ScanItems object
        // https://stackoverflow.com/questions/25803871/counting-number-of-items-in-observablecollection-where-it-equals-1-c-sharp
        public int SelectedItems
        {
            get
            {
                return this.Where(x => x.IsSelected == true).Count();
            }

        }

        // RPECK 07/02/2025 - Select All
        // Used to change all "IsSelected" values to true
        public void SelectAll()
        {

            // RPECK 07/02/2025 - Loops through all items within collection and sets "IsSelected" to true
            this.All(x => x.IsSelected = true);

        }

    }

}
