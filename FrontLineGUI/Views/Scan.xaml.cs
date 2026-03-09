using System.ComponentModel;
using System.Runtime.Versioning;
using System.Windows.Controls;

namespace FrontLineGUI
{

    // RPECK 05/04/2023
    // Notes on the use of "OSVersionInfo"
    // https://www.codeproject.com/Articles/73000/Getting-Operating-System-Version-Info-Even-for-Win

    // RPECK 08/04/2023 - this was required to limit the application to Windows OS versions only
    [SupportedOSPlatform("windows")]
    public partial class ScanView : UserControl
    {

        // Public class vars
        public ScanView()
        {
            InitializeComponent();
        }

    }

}
