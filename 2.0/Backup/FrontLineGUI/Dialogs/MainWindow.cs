using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;
using System.Drawing.Drawing2D;
using System.Runtime.InteropServices;
using System.Reflection;
using Microsoft.Win32;
using FrontLineGUI.Dialogs;

namespace FrontLineGUI
{
    public partial class MainWindow : Form
    {
        private List<CustomFrontLineButton> _LeftMenuGroup = new List<CustomFrontLineButton>();
        Timer tmAutoUpd = new Timer();

        SplashWindow splash = new SplashWindow();
        public MainWindow()
        {
            splash.Show();
            InitializeComponent();
            this.FormClosing += new FormClosingEventHandler(MainFormClosing);
            this.Load += new EventHandler(MainWindow_Load);
            wizControl.TabPages["ScanPage"].Controls.Add(new StartScanPanel(wizControl));
            wizControl.TabPages["BackUpPage"].Controls.Add(new BackUpPanel(wizControl));
            wizControl.TabPages["StartUpPage"].Controls.Add(new StartUpPanel(wizControl));
            wizControl.TabPages["SettingsPage"].Controls.Add(new SettingsPanel(wizControl));
            wizControl.TabPages["DefragStep2"].Controls.Add(new DefragStep2Panel(wizControl));
            wizControl.TabPages["DefragStep1"].Controls.Add(new DefragStep1Panel(wizControl));
            wizControl.TabPages["ExtrasPage"].Controls.Add(new ExtrasPanel(wizControl));
            wizControl.TabPages["StatisticsPage"].Controls.Add(new StatisticsPanel(wizControl));
            wizControl.TabPages["ScanningPage"].Controls.Add(new ScanningPanel(wizControl));
            wizControl.TabPages["FixingPage"].Controls.Add(new FixErrorsPanel(wizControl));
            wizControl.Tag = this;

            wizControl.Deselecting += new TabControlCancelEventHandler(wizControl_Deselecting);
            wizControl.SelectedIndexChanged += new System.EventHandler(wizControl_SelectedIndexChanged);
            
            _LeftMenuGroup.Add(ButtonScan);
            _LeftMenuGroup.Add(ButtonExtras);
            _LeftMenuGroup.Add(ButtonBackUp);
            _LeftMenuGroup.Add(ButtonStatistics);

            RegistryKey key = Registry.CurrentUser.OpenSubKey(Program.ApplicationOptions + @"\lkey",false);
            if (key != null)
            {
                var license = key.GetValue("Lkey").ToString();
                if (!String.IsNullOrEmpty(license))
                {
                    LicenseManager license_manager = new LicenseManager();
                    var response = license_manager.CheckLicense(license);
                    if (response == LicenseStatus.eLicenseBlock)
                    {
                        ApplicationSettings.IsPaid = false;
                    }
                }
            }
            TransformAppToPaid(ApplicationSettings.IsPaid);
            
            ButtonScan_Click(ButtonScan, null);
            if (ApplicationSettings.AutoUpdate)
            {
                tmAutoUpd.Interval = 60 * 1000;
                tmAutoUpd.Tick += new EventHandler(tmAutoUpd_Tick);
                tmAutoUpd.Start();
            }
        }

        void tmAutoUpd_Tick(object sender, EventArgs e)
        {
            tmAutoUpd.Stop();
            UpdateManager upd_man = new UpdateManager();
            var response = upd_man.CheckUpdate();
            if (!String.IsNullOrEmpty(response.DownloadUrl))
            {
                if (Assembly.GetExecutingAssembly().GetName().Version.Major < response.Version)
                {
                    upd_man.ApplyUpdate(response.DownloadUrl);
                }
            }
        }

        public void TransformAppToPaid(bool bPaid)
        {
            ButtonRegistration.Visible = !bPaid;
            ButtonNeedHelp.Visible = !bPaid;
            NeedHelp2Button.Visible = bPaid;
        }

        void MainWindow_Load(object sender, EventArgs e)
        {
            this.TopMost = true;
            this.TopMost = false;
        }
       
        void MainFormClosing(object sender, FormClosingEventArgs e)
        {
            Process.GetCurrentProcess().Kill();
        }

        private void _EnableAllExceptPressed(object iObject)
        {
            var enabledElements = _LeftMenuGroup.FindAll(
                element => 
               (element != (CustomFrontLineButton)iObject)).Cast<CustomFrontLineButton>();
            foreach (var button in enabledElements)
                button.Enabled = true;
            var currentButton = (CustomFrontLineButton)iObject;
            if (currentButton != null)
                currentButton.Enabled = false;
        }

        #region Appearing

        Timer tmOpacity = new Timer();
        protected override void  OnShown(EventArgs e)
        {
            tmOpacity.Interval = 20;
            tmOpacity.Tick += new EventHandler(tmOpacity_Tick);
 	         base.OnShown(e);
             tmOpacity.Start();
        }

        void tmOpacity_Tick(object sender, EventArgs e)
        {
            this.Opacity += 0.05;
            if (this.Opacity >= 1)
            {
                splash.Close();
                this.Opacity = 1;
                tmOpacity.Stop();
            }
        }

        #endregion

        void wizControl_Deselecting(object sender, TabControlCancelEventArgs e)
          {
          e.TabPage.Hide();
          }

        void wizControl_SelectedIndexChanged(object sender, EventArgs e)
          {
          GC.Collect();
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

        private void MainWindow_Resize(object sender, EventArgs e)
        {
        }

        #region ButtonsHandlers

        private void ButtonStatistics_MouseClick(object sender, MouseEventArgs e)
        {
            wizControl.SelectedIndex = 1;
            _EnableAllExceptPressed(sender);
        }

        private void ButtonRegistration_Click(object sender, EventArgs e)
        {
            ShowRegisterScreen(this);
        }

        private void ButtonSettings_MouseClick(object sender, EventArgs e)
        {
            wizControl.SelectedIndex = 5;
        }

        private void ButtonExtras_Click(object sender, EventArgs e)
        {
            wizControl.SelectedIndex = 2;
            _EnableAllExceptPressed(sender);
        }

        private void ButtonScan_Click(object sender, EventArgs e)
        {
            if (Program.CurrentState == ApplicationState.eFixingErrors)
            {
                wizControl.SelectedIndex = 9;
            }
            else
                if (!CleanEngineClient.Instance().IsBusy())
                    wizControl.SelectedIndex = 0;
                else
                    wizControl.SelectedIndex = 8;
            _EnableAllExceptPressed(sender);
        }

        private void ButtonClose_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void ButtonMinimize_Click(object sender, EventArgs e)
        {
            this.WindowState = System.Windows.Forms.FormWindowState.Minimized;
        }

        public void ShowUpdateScreen(Form parent)
        {
            ShowBlackAlfaScreen();
            var UpdWnd = new UpdateWindow();
            UpdWnd.ShowDialog(parent);
            bkDlg.Hide();
            this.TopMost = true;
            this.TopMost = false;
        }

        public void ShowRegisterScreen(Form parent)
        {
            ShowBlackAlfaScreen();
            var regWnd = new RegisterWindow();
            regWnd.MainWindow = this;
            regWnd.ShowDialog(parent);
            bkDlg.Hide();
            this.TopMost = true;
            this.TopMost = false;
        }

        private void ButtonUpdate_Click(object sender, EventArgs e)
        {
            ShowUpdateScreen(this);
        }

        private void ButtonBackUp_Click(object sender, EventArgs e)
        {
            wizControl.SelectedIndex = 7;
            _EnableAllExceptPressed(sender);
        }

        private void ButtonPCUtils_Click(object sender, EventArgs e)
        {
            Process.Start("http://www.frontlinepcutilities.com");
        }

        private void ButtonSettings_Click(object sender, EventArgs e)
        {
            wizControl.SelectedIndex = 5;
        }
        
        private void ButtonNeedHelp_Click(object sender, EventArgs e)
        {
            ShowRegisterScreen(this);
        }

        private void ButtonHelp_Click(object sender, EventArgs e)
        {
            Process.Start("http://www.frontlinepcutilities.com/support/");
        }
        
        #endregion

        private BlackTransparentWindow bkDlg = new BlackTransparentWindow();
        private Panel blackPanel = new Panel();
        WizzardControl wzTmp = new WizzardControl();
        public void ShowBlackAlfaScreen()
        {
            var location = this.Location;
            bkDlg.Location = location;

            bkDlg.Owner = this;
            bkDlg.Show(this);
        }

        private void NeedHelp2Button_Click(object sender, EventArgs e)
        {
            Process.Start("http://www.frontlinepcutilities.com/support/");
        }

     
    }
}
