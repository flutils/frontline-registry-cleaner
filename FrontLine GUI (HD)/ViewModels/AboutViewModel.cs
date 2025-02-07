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
        public AboutViewModel(AboutView model)
        {
            Model = model;

            // RPECK 06/02/2025 - Title
            // Sets the public title of the ModelView (in this case, "About")
            Title = "About";

        }

        public AboutView Model { get; private set; }

    }

}
