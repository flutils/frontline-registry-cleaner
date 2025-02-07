using System;
using System.Collections.Generic;
using System.Configuration;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FrontLineGUI
{
    public class ScanViewModel : ViewModelBase
    {

        // RPECK 06/02/2025 - Declarations
        // Used to give us the ability to call or modify attributes publicly
        public ScanView Model { get; private set; }

        public ScanViewModel(ScanView model)
        {

            Model = model;

            // RPECK 06/02/2025 - Title
            // Sets the public title of the ModelView (in this case, "Scan")
            Title = "Scan";

        }

  

    }

}
