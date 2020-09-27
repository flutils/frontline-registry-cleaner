using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace FrontLineGUI.Dialogs.CustomControls
{
    public class AntiDoubleClickTreeView : System.Windows.Forms.TreeView
    {
        protected override void WndProc(ref System.Windows.Forms.Message m)
        {
            // Suppress WM_LBUTTONDBLCLK
            if (m.Msg == 0x203)
            {
                m.Result = System.IntPtr.Zero;
            }

            else if (0x0014 == m.Msg)
            {
                m.Msg = (int)0x0000; //reset message to null
                base.WndProc(ref m);
            }
            else
                base.WndProc(ref m);
        }
    }
}
