using System;
using System.Collections.Generic;
using System.Configuration;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FrontLineGUI
{
    public partial class SettingsViewModel : ViewModelBase
    {
        public SettingsView Model { get; private set; }

        public SettingsViewModel(SettingsView model)
        {
            Model = model;

            // RPECK 06/02/2025 - Title
            // Sets the public title of the ModelView (in this case, "Scan")
            Title = "Settings";

        }

    }

}
