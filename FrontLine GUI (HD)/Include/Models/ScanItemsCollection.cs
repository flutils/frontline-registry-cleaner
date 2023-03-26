using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FrontLineGUI
{
 
    public class ScanItemsCollection : ObservableCollection<ScanItem>
    {
        public ScanItemsCollection() : base()
        {
            Add(new ScanItem("Willa", "Cather"));
            Add(new ScanItem("Isak", "Dinesen"));
            Add(new ScanItem("Victor", "Hugo"));
            Add(new ScanItem("Jules", "Verne"));
        }
    }

}
