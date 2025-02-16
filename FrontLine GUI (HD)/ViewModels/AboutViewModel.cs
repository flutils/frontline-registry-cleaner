using System;
using System.Collections.Generic;
using System.Configuration;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FrontLineGUI
{
    public partial class AboutViewModel : ViewModelBase
    {

        // RPECK 09/02/2025 - Public properties
        // Used to provide the means to access different properties within the system
        public AboutView Model { get; private set; }

        public AboutViewModel(AboutView model)
        {

            // RPECK 09/02/2025 - Model
            // Used to get the DataContext for the system
            Model = model;

            // RPECK 06/02/2025 - Title
            // Sets the public title of the ModelView (in this case, "About")
            Title = "About";

        }

    }

}
