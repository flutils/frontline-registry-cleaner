using System.Windows.Controls;
using System.Windows;
using JCS;
using System.Diagnostics;
using System;
using System.Windows.Threading;
using System.Runtime.Versioning;
using System.Reflection;
using System.Windows.Data;

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
        public CPUUtilization CPUInfo { get; set; }

        public ScanView()
        {
            InitializeComponent();
        }

    }

}
