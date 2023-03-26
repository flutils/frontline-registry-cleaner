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
            Add(new ScanItem("Registry Errors"));
            Add(new ScanItem("Invalid Files"));
            Add(new ScanItem("Application Errors"));
            Add(new ScanItem("DLL Errors"));
        }
    }

}
