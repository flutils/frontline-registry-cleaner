using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace FrontLineGUI
{
    public partial class WizzardControl : TabControl
    {
        public WizzardControl()
        {
            InitializeComponent();
            if (!DesignMode)
            {
                this.Appearance = TabAppearance.Buttons;
                this.SizeMode = TabSizeMode.Fixed;
                this.ItemSize = new Size(0, 1);
                this.Alignment = TabAlignment.Top;
                this.Padding = new Point(0, 0);
                this.Margin = new System.Windows.Forms.Padding(0);
                foreach (TabPage t in this.TabPages)
                {
                    t.Padding = new System.Windows.Forms.Padding(0);
                    t.Margin = new System.Windows.Forms.Padding(0);
                }
            }
        }

        protected override void WndProc(ref Message m)
	    {
	        // Hide tabs by trapping the TCM_ADJUSTRECT message
	        if (m.Msg == 0x1328 && !DesignMode)
	           m.Result = (IntPtr)1;
	        else
	           base.WndProc(ref m);
	    }
    }
}
