using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;
using Microsoft.Win32;

namespace FrontLineGUI
{
    public partial class RegisterWindow : Form
    {
        public MainWindow MainWindow = null;

        public RegisterWindow()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void ButtonClickForKey_MouseEnter(object sender, EventArgs e)
        {
            var btn = (Button)sender;
            if (btn != null)
                btn.ImageIndex = 1;
        }

        private void ButtonClickForKey_MouseLeave(object sender, EventArgs e)
        {
            var btn = (Button)sender;
            if (btn != null)
                btn.ImageIndex = 0;
        }

        private void ButtonClickForKey_Click(object sender, EventArgs e)
        {
            Process.Start("http://www.frontlinecleaner.com/order/");
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

        private void linkLabel1_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            Process.Start(@"http://www.frontlinecleaner.com/order/");
        }

        private void button2_MouseEnter(object sender, EventArgs e)
        {
            var btn = (Button)sender;
            if (btn != null)
                btn.ImageIndex = 1;
        }

        private void ButtonSubmit_MouseLeave(object sender, EventArgs e)
        {
            var btn = (Button)sender;
            if (btn != null)
                btn.ImageIndex = 0;
        }

        private void ButtonSubmit_Click(object sender, EventArgs e)
        {
            LicenseManager license_manager = new LicenseManager();
            var lic_response = license_manager.CheckLicense(TextBoxLicense.Text);
            if(lic_response == LicenseStatus.eLicenseOK)
            {
                ApplicationSettings.IsPaid = true;
                if (MainWindow != null)
                {
                    RegistryKey key = Registry.CurrentUser.CreateSubKey(Program.ApplicationOptions+@"\lkey");
                    if (key != null)
                    {
                        key.SetValue("Lkey", TextBoxLicense.Text);
                    }
                    MainWindow.TransformAppToPaid(true);
                    this.Close();
                    MessageBox.Show("Thank you for purchasing our product!");
                }
            }
            else if(lic_response == LicenseStatus.eLicenseInvalid)
            {
                MessageBox.Show("Invalid application license.");
            }
            else if (lic_response == LicenseStatus.eLicenseBlock)
            {
                MessageBox.Show("Your license has expired. Please buy new one.");
            }
            MainWindow.BringToFront();
        }

        private void ButtonDontHave_Click(object sender, EventArgs e)
        {
            Process.Start("http://www.frontlinecleaner.com/order/");
        }

        private void LinkDontHave_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            Process.Start("http://www.frontlinecleaner.com/order/");
        }
    }
}
