using System.Windows.Controls;
using Microsoft.VisualBasic.Devices;

namespace FrontLineGUI
{
    //Scan Logic
    public partial class Scan : Page
    {
        public Scan()
        {
            // Initialize the form
            InitializeComponent();

            // Required for OSVersion
            this.DataContext = this;
        }

        // OS Version
        // Used to show the Operating System name in the bottom left of the grid
        // https://stackoverflow.com/a/45045313/1143732
        public string OSVersion
        {
            get { return new ComputerInfo().OSFullName.ToString(); }
        }

    }
}
