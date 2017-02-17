using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.IO;
using System.Reflection;
using System.Text.RegularExpressions;
using System.Diagnostics;

namespace FrontLineGUI
{
    public partial class UpdateWindow : Form
    {
        Timer tmWait = new Timer();
        String DownlodUrl = String.Empty;
        UpdateManager upd_manager = new UpdateManager();

        public UpdateWindow()
        {
            InitializeComponent();
            tmWait.Tick += new EventHandler(tmWait_Tick);
            tmWait.Interval = 1000;
            tmWait.Start();
        }

        void tmWait_Tick(object sender, EventArgs e)
        {
            tmWait.Stop();
            var response = upd_manager.CheckUpdate();
            DownlodUrl = response.DownloadUrl;
            if (Assembly.GetExecutingAssembly().GetName().Version.Major < response.Version)
            {
                ButtonDownloadUpdate.Visible = true;
                pictureBoxUpdate.Visible = false;
            }
        }

        [System.Security.Permissions.PermissionSet(System.Security.Permissions.SecurityAction.Demand, Name = "FullTrust")]
        protected override void WndProc(ref Message m)
        {

            base.WndProc(ref m);
            const int WM_NCHITTEST = 0x0084;
            const int HTCAPTION = 0x0002;
            if (m.Msg == WM_NCHITTEST)
            {
                int pos = m.LParam.ToInt32();
                short yPos = (short)(pos >> 16);
                short xPos = (short)pos;
                Point pt = this.PointToClient(new Point(xPos, yPos));
                if (pt.Y < 40)
                {
                    m.Result = new IntPtr(HTCAPTION);
                }
            }
        }

        private void ButtonClose_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void ButtonDownloadUpdate_Click(object sender, EventArgs e)
        {
            try
            {
                upd_manager.ApplyUpdate(DownlodUrl);
            }
            catch { }
            Close();
        }


    }
}
