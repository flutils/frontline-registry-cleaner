using System.Windows.Forms;
using FrontLineGUI;

namespace FrontLineGUI
{
    partial class MainWindow
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainWindow));
            this.ScanButtonImageList = new System.Windows.Forms.ImageList(this.components);
            this.ExtrasImageList = new System.Windows.Forms.ImageList(this.components);
            this.StatisticsImageList = new System.Windows.Forms.ImageList(this.components);
            this.BackUpImageList = new System.Windows.Forms.ImageList(this.components);
            this.MinImageList = new System.Windows.Forms.ImageList(this.components);
            this.CloseImageList = new System.Windows.Forms.ImageList(this.components);
            this.StartScanImageList = new System.Windows.Forms.ImageList(this.components);
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.PCUtilsImageList = new System.Windows.Forms.ImageList(this.components);
            this.SettingsLinkImageList = new System.Windows.Forms.ImageList(this.components);
            this.UpdateImageList = new System.Windows.Forms.ImageList(this.components);
            this.HelpImageList = new System.Windows.Forms.ImageList(this.components);
            this.NeedHelp2ImageList = new System.Windows.Forms.ImageList(this.components);
            this.NeedHelp2Button = new FrontLineGUI.CustomFrontLineButton();
            this.wizControl = new FrontLineGUI.WizzardControl();
            this.ScanPage = new System.Windows.Forms.TabPage();
            this.StatisticsPage = new System.Windows.Forms.TabPage();
            this.ExtrasPage = new System.Windows.Forms.TabPage();
            this.DefragStep1 = new System.Windows.Forms.TabPage();
            this.DefragStep2 = new System.Windows.Forms.TabPage();
            this.SettingsPage = new System.Windows.Forms.TabPage();
            this.StartupPage = new System.Windows.Forms.TabPage();
            this.BackUpPage = new System.Windows.Forms.TabPage();
            this.ScanningPage = new System.Windows.Forms.TabPage();
            this.FixingPage = new System.Windows.Forms.TabPage();
            this.ButtonHelp = new FrontLineGUI.CustomFrontLineButton();
            this.ButtonUpdate = new FrontLineGUI.CustomFrontLineButton();
            this.ButtonSettings = new FrontLineGUI.CustomFrontLineButton();
            this.ButtonPCUtils = new FrontLineGUI.CustomFrontLineButton();
            this.ButtonNeedHelp = new FrontLineGUI.CustomFrontLineButton();
            this.ButtonClose = new FrontLineGUI.CustomFrontLineButton();
            this.ButtonMinimize = new FrontLineGUI.CustomFrontLineButton();
            this.ButtonStatistics = new FrontLineGUI.CustomFrontLineButton();
            this.ButtonBackUp = new FrontLineGUI.CustomFrontLineButton();
            this.ButtonExtras = new FrontLineGUI.CustomFrontLineButton();
            this.ButtonScan = new FrontLineGUI.CustomFrontLineButton();
            this.wizControl.SuspendLayout();
            this.SuspendLayout();
            // 
            // ScanButtonImageList
            // 
            this.ScanButtonImageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("ScanButtonImageList.ImageStream")));
            this.ScanButtonImageList.TransparentColor = global::FrontLineGUI.Properties.Settings.Default.lol;
            this.ScanButtonImageList.Images.SetKeyName(0, "Scan_button.png");
            this.ScanButtonImageList.Images.SetKeyName(1, "ScanHover_button.png");
            this.ScanButtonImageList.Images.SetKeyName(2, "ScanDisabled_button.png");
            // 
            // ExtrasImageList
            // 
            this.ExtrasImageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("ExtrasImageList.ImageStream")));
            this.ExtrasImageList.TransparentColor = System.Drawing.Color.White;
            this.ExtrasImageList.Images.SetKeyName(0, "Extras_button.png");
            this.ExtrasImageList.Images.SetKeyName(1, "ExtrasHover_button.png");
            this.ExtrasImageList.Images.SetKeyName(2, "ExtrasDisabled_button.png");
            // 
            // StatisticsImageList
            // 
            this.StatisticsImageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("StatisticsImageList.ImageStream")));
            this.StatisticsImageList.TransparentColor = System.Drawing.Color.White;
            this.StatisticsImageList.Images.SetKeyName(0, "Statistics_button.png");
            this.StatisticsImageList.Images.SetKeyName(1, "StatisticsHover_button.png");
            this.StatisticsImageList.Images.SetKeyName(2, "StatisticsDisabled_button.png");
            // 
            // BackUpImageList
            // 
            this.BackUpImageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("BackUpImageList.ImageStream")));
            this.BackUpImageList.TransparentColor = System.Drawing.Color.White;
            this.BackUpImageList.Images.SetKeyName(0, "BackUp_button.png");
            this.BackUpImageList.Images.SetKeyName(1, "BackUpHover_button.png");
            this.BackUpImageList.Images.SetKeyName(2, "BackUpDisabled_button.png");
            // 
            // MinImageList
            // 
            this.MinImageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("MinImageList.ImageStream")));
            this.MinImageList.TransparentColor = System.Drawing.Color.DarkViolet;
            this.MinImageList.Images.SetKeyName(0, "Minimize_button.png");
            this.MinImageList.Images.SetKeyName(1, "MinimizeHover_button.png");
            // 
            // CloseImageList
            // 
            this.CloseImageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("CloseImageList.ImageStream")));
            this.CloseImageList.TransparentColor = System.Drawing.Color.Magenta;
            this.CloseImageList.Images.SetKeyName(0, "Close_button.png");
            this.CloseImageList.Images.SetKeyName(1, "CloseHover_button.png");
            // 
            // StartScanImageList
            // 
            this.StartScanImageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("StartScanImageList.ImageStream")));
            this.StartScanImageList.TransparentColor = System.Drawing.Color.White;
            this.StartScanImageList.Images.SetKeyName(0, "Statistics_button.png");
            this.StartScanImageList.Images.SetKeyName(1, "StatisticsHover_button.png");
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.BackColor = System.Drawing.Color.Transparent;
            this.label2.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.label2.Font = new System.Drawing.Font("Calibri", 9F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.label2.Location = new System.Drawing.Point(587, 56);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(13, 14);
            this.label2.TabIndex = 27;
            this.label2.Text = "|";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.BackColor = System.Drawing.Color.Transparent;
            this.label3.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.label3.Font = new System.Drawing.Font("Calibri", 9F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.label3.Location = new System.Drawing.Point(637, 56);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(13, 14);
            this.label3.TabIndex = 28;
            this.label3.Text = "|";
            // 
            // PCUtilsImageList
            // 
            this.PCUtilsImageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("PCUtilsImageList.ImageStream")));
            this.PCUtilsImageList.TransparentColor = System.Drawing.Color.White;
            this.PCUtilsImageList.Images.SetKeyName(0, "PCUtilities_button.png");
            this.PCUtilsImageList.Images.SetKeyName(1, "PCUtilitiesHover_button.png");
            // 
            // SettingsLinkImageList
            // 
            this.SettingsLinkImageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("SettingsLinkImageList.ImageStream")));
            this.SettingsLinkImageList.TransparentColor = System.Drawing.Color.White;
            this.SettingsLinkImageList.Images.SetKeyName(0, "Settings_button.png");
            this.SettingsLinkImageList.Images.SetKeyName(1, "SettingsHover_button.png");
            // 
            // UpdateImageList
            // 
            this.UpdateImageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("UpdateImageList.ImageStream")));
            this.UpdateImageList.TransparentColor = System.Drawing.Color.White;
            this.UpdateImageList.Images.SetKeyName(0, "Update_button.png");
            this.UpdateImageList.Images.SetKeyName(1, "UpdateHover_button.png");
            // 
            // HelpImageList
            // 
            this.HelpImageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("HelpImageList.ImageStream")));
            this.HelpImageList.TransparentColor = System.Drawing.Color.White;
            this.HelpImageList.Images.SetKeyName(0, "Help_button.png");
            this.HelpImageList.Images.SetKeyName(1, "HelpHover_button.png");
            // 
            // NeedHelp2ImageList
            // 
            this.NeedHelp2ImageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("NeedHelp2ImageList.ImageStream")));
            this.NeedHelp2ImageList.TransparentColor = System.Drawing.Color.White;
            this.NeedHelp2ImageList.Images.SetKeyName(0, "NeedHelp2_button.png");
            this.NeedHelp2ImageList.Images.SetKeyName(1, "NeedHelp2Hover_button.png");
            // 
            // NeedHelp2Button
            // 
            this.NeedHelp2Button.BackColor = System.Drawing.Color.Transparent;
            this.NeedHelp2Button.Cursor = System.Windows.Forms.Cursors.Hand;
            this.NeedHelp2Button.FlatAppearance.BorderSize = 0;
            this.NeedHelp2Button.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
            this.NeedHelp2Button.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.NeedHelp2Button.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.NeedHelp2Button.ForeColor = System.Drawing.Color.Transparent;
            this.NeedHelp2Button.ImageIndex = 0;
            this.NeedHelp2Button.ImageList = this.NeedHelp2ImageList;
            this.NeedHelp2Button.Location = new System.Drawing.Point(14, 475);
            this.NeedHelp2Button.Name = "NeedHelp2Button";
            this.NeedHelp2Button.Size = new System.Drawing.Size(188, 46);
            this.NeedHelp2Button.TabIndex = 41;
            this.NeedHelp2Button.UseVisualStyleBackColor = false;
            this.NeedHelp2Button.Click += new System.EventHandler(this.NeedHelp2Button_Click);
            // 
            // wizControl
            // 
            this.wizControl.Appearance = System.Windows.Forms.TabAppearance.Buttons;
            this.wizControl.Controls.Add(this.ScanPage);
            this.wizControl.Controls.Add(this.StatisticsPage);
            this.wizControl.Controls.Add(this.ExtrasPage);
            this.wizControl.Controls.Add(this.DefragStep1);
            this.wizControl.Controls.Add(this.DefragStep2);
            this.wizControl.Controls.Add(this.SettingsPage);
            this.wizControl.Controls.Add(this.StartupPage);
            this.wizControl.Controls.Add(this.BackUpPage);
            this.wizControl.Controls.Add(this.ScanningPage);
            this.wizControl.Controls.Add(this.FixingPage);
            this.wizControl.ItemSize = new System.Drawing.Size(44, 20);
            this.wizControl.Location = new System.Drawing.Point(204, 76);
            this.wizControl.Margin = new System.Windows.Forms.Padding(45);
            this.wizControl.Name = "wizControl";
            this.wizControl.Padding = new System.Drawing.Point(14, 1);
            this.wizControl.SelectedIndex = 0;
            this.wizControl.Size = new System.Drawing.Size(476, 444);
            this.wizControl.SizeMode = System.Windows.Forms.TabSizeMode.Fixed;
            this.wizControl.TabIndex = 40;
            // 
            // ScanPage
            // 
            this.ScanPage.Cursor = System.Windows.Forms.Cursors.Default;
            this.ScanPage.Location = new System.Drawing.Point(4, 24);
            this.ScanPage.Name = "ScanPage";
            this.ScanPage.Padding = new System.Windows.Forms.Padding(3);
            this.ScanPage.Size = new System.Drawing.Size(468, 416);
            this.ScanPage.TabIndex = 0;
            this.ScanPage.Text = "ScanPage";
            this.ScanPage.UseVisualStyleBackColor = true;
            // 
            // StatisticsPage
            // 
            this.StatisticsPage.Location = new System.Drawing.Point(4, 24);
            this.StatisticsPage.Name = "StatisticsPage";
            this.StatisticsPage.Padding = new System.Windows.Forms.Padding(3);
            this.StatisticsPage.Size = new System.Drawing.Size(468, 416);
            this.StatisticsPage.TabIndex = 1;
            this.StatisticsPage.Text = "StatisticsPage";
            this.StatisticsPage.UseVisualStyleBackColor = true;
            // 
            // ExtrasPage
            // 
            this.ExtrasPage.Location = new System.Drawing.Point(4, 24);
            this.ExtrasPage.Name = "ExtrasPage";
            this.ExtrasPage.Size = new System.Drawing.Size(468, 416);
            this.ExtrasPage.TabIndex = 2;
            this.ExtrasPage.Text = "ExtrasPage";
            this.ExtrasPage.UseVisualStyleBackColor = true;
            // 
            // DefragStep1
            // 
            this.DefragStep1.Location = new System.Drawing.Point(4, 24);
            this.DefragStep1.Name = "DefragStep1";
            this.DefragStep1.Size = new System.Drawing.Size(468, 416);
            this.DefragStep1.TabIndex = 3;
            this.DefragStep1.Text = "DefragStep1";
            this.DefragStep1.UseVisualStyleBackColor = true;
            // 
            // DefragStep2
            // 
            this.DefragStep2.Location = new System.Drawing.Point(4, 24);
            this.DefragStep2.Name = "DefragStep2";
            this.DefragStep2.Size = new System.Drawing.Size(468, 416);
            this.DefragStep2.TabIndex = 4;
            this.DefragStep2.Text = "DefragStep2";
            this.DefragStep2.UseVisualStyleBackColor = true;
            // 
            // SettingsPage
            // 
            this.SettingsPage.Location = new System.Drawing.Point(4, 24);
            this.SettingsPage.Name = "SettingsPage";
            this.SettingsPage.Size = new System.Drawing.Size(468, 416);
            this.SettingsPage.TabIndex = 6;
            this.SettingsPage.Text = "SettingsPage";
            this.SettingsPage.UseVisualStyleBackColor = true;
            // 
            // StartupPage
            // 
            this.StartupPage.Location = new System.Drawing.Point(4, 24);
            this.StartupPage.Name = "StartupPage";
            this.StartupPage.Size = new System.Drawing.Size(468, 416);
            this.StartupPage.TabIndex = 7;
            this.StartupPage.Text = "StartupPage";
            this.StartupPage.UseVisualStyleBackColor = true;
            // 
            // BackUpPage
            // 
            this.BackUpPage.Location = new System.Drawing.Point(4, 24);
            this.BackUpPage.Name = "BackUpPage";
            this.BackUpPage.Padding = new System.Windows.Forms.Padding(3);
            this.BackUpPage.Size = new System.Drawing.Size(468, 416);
            this.BackUpPage.TabIndex = 8;
            this.BackUpPage.Text = "BackUpPage";
            this.BackUpPage.UseVisualStyleBackColor = true;
            // 
            // ScanningPage
            // 
            this.ScanningPage.Location = new System.Drawing.Point(4, 24);
            this.ScanningPage.Name = "ScanningPage";
            this.ScanningPage.Size = new System.Drawing.Size(468, 416);
            this.ScanningPage.TabIndex = 9;
            this.ScanningPage.Text = "ScanningPage";
            this.ScanningPage.UseVisualStyleBackColor = true;
            // 
            // FixingPage
            // 
            this.FixingPage.Location = new System.Drawing.Point(4, 24);
            this.FixingPage.Name = "FixingPage";
            this.FixingPage.Size = new System.Drawing.Size(468, 416);
            this.FixingPage.TabIndex = 10;
            this.FixingPage.Text = "FixingPage";
            this.FixingPage.UseVisualStyleBackColor = true;
            // 
            // ButtonHelp
            // 
            this.ButtonHelp.BackColor = System.Drawing.Color.Transparent;
            this.ButtonHelp.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.ButtonHelp.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ButtonHelp.FlatAppearance.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(207)))), ((int)(((byte)(187)))), ((int)(((byte)(44)))));
            this.ButtonHelp.FlatAppearance.BorderSize = 0;
            this.ButtonHelp.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
            this.ButtonHelp.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.ButtonHelp.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonHelp.ForeColor = System.Drawing.Color.Black;
            this.ButtonHelp.ImageIndex = 0;
            this.ButtonHelp.ImageList = this.HelpImageList;
            this.ButtonHelp.Location = new System.Drawing.Point(649, 57);
            this.ButtonHelp.Name = "ButtonHelp";
            this.ButtonHelp.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.ButtonHelp.Size = new System.Drawing.Size(25, 14);
            this.ButtonHelp.TabIndex = 37;
            this.ButtonHelp.UseVisualStyleBackColor = false;
            this.ButtonHelp.Click += new System.EventHandler(this.ButtonHelp_Click);
            // 
            // ButtonUpdate
            // 
            this.ButtonUpdate.BackColor = System.Drawing.Color.Transparent;
            this.ButtonUpdate.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.ButtonUpdate.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ButtonUpdate.FlatAppearance.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(207)))), ((int)(((byte)(187)))), ((int)(((byte)(44)))));
            this.ButtonUpdate.FlatAppearance.BorderSize = 0;
            this.ButtonUpdate.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
            this.ButtonUpdate.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.ButtonUpdate.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonUpdate.ForeColor = System.Drawing.Color.Black;
            this.ButtonUpdate.ImageIndex = 0;
            this.ButtonUpdate.ImageList = this.UpdateImageList;
            this.ButtonUpdate.Location = new System.Drawing.Point(596, 57);
            this.ButtonUpdate.Name = "ButtonUpdate";
            this.ButtonUpdate.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.ButtonUpdate.Size = new System.Drawing.Size(44, 14);
            this.ButtonUpdate.TabIndex = 36;
            this.ButtonUpdate.UseVisualStyleBackColor = false;
            // 
            // ButtonSettings
            // 
            this.ButtonSettings.BackColor = System.Drawing.Color.Transparent;
            this.ButtonSettings.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.ButtonSettings.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ButtonSettings.FlatAppearance.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(207)))), ((int)(((byte)(187)))), ((int)(((byte)(44)))));
            this.ButtonSettings.FlatAppearance.BorderSize = 0;
            this.ButtonSettings.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
            this.ButtonSettings.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.ButtonSettings.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonSettings.ForeColor = System.Drawing.Color.Black;
            this.ButtonSettings.ImageIndex = 0;
            this.ButtonSettings.ImageList = this.SettingsLinkImageList;
            this.ButtonSettings.Location = new System.Drawing.Point(544, 57);
            this.ButtonSettings.Name = "ButtonSettings";
            this.ButtonSettings.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.ButtonSettings.Size = new System.Drawing.Size(44, 14);
            this.ButtonSettings.TabIndex = 35;
            this.ButtonSettings.UseVisualStyleBackColor = false;
            this.ButtonSettings.Click += new System.EventHandler(this.ButtonSettings_Click);
            // 
            // ButtonPCUtils
            // 
            this.ButtonPCUtils.BackColor = System.Drawing.Color.Transparent;
            this.ButtonPCUtils.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.ButtonPCUtils.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ButtonPCUtils.FlatAppearance.BorderSize = 0;
            this.ButtonPCUtils.FlatAppearance.CheckedBackColor = System.Drawing.Color.Transparent;
            this.ButtonPCUtils.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
            this.ButtonPCUtils.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.ButtonPCUtils.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonPCUtils.ForeColor = System.Drawing.Color.Transparent;
            this.ButtonPCUtils.ImageIndex = 0;
            this.ButtonPCUtils.ImageList = this.PCUtilsImageList;
            this.ButtonPCUtils.Location = new System.Drawing.Point(32, 453);
            this.ButtonPCUtils.Name = "ButtonPCUtils";
            this.ButtonPCUtils.Size = new System.Drawing.Size(147, 16);
            this.ButtonPCUtils.TabIndex = 29;
            this.ButtonPCUtils.UseVisualStyleBackColor = false;
            this.ButtonPCUtils.Click += new System.EventHandler(this.ButtonPCUtils_Click);
            // 
            // ButtonNeedHelp
            // 
            this.ButtonNeedHelp.BackColor = System.Drawing.Color.Transparent;
            this.ButtonNeedHelp.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ButtonNeedHelp.FlatAppearance.BorderSize = 0;
            this.ButtonNeedHelp.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
            this.ButtonNeedHelp.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.ButtonNeedHelp.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonNeedHelp.ForeColor = System.Drawing.Color.Transparent;
            this.ButtonNeedHelp.ImageIndex = 0;
            this.ButtonNeedHelp.Location = new System.Drawing.Point(12, 473);
            this.ButtonNeedHelp.Name = "ButtonNeedHelp";
            this.ButtonNeedHelp.Size = new System.Drawing.Size(188, 46);
            this.ButtonNeedHelp.TabIndex = 25;
            this.ButtonNeedHelp.UseVisualStyleBackColor = false;
            // 
            // ButtonClose
            // 
            this.ButtonClose.BackColor = System.Drawing.Color.Transparent;
            this.ButtonClose.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.ButtonClose.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ButtonClose.FlatAppearance.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(41)))), ((int)(((byte)(51)))), ((int)(((byte)(61)))));
            this.ButtonClose.FlatAppearance.BorderSize = 0;
            this.ButtonClose.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
            this.ButtonClose.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.ButtonClose.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonClose.ForeColor = System.Drawing.Color.Black;
            this.ButtonClose.ImageIndex = 0;
            this.ButtonClose.ImageList = this.CloseImageList;
            this.ButtonClose.Location = new System.Drawing.Point(655, 15);
            this.ButtonClose.Name = "ButtonClose";
            this.ButtonClose.Size = new System.Drawing.Size(21, 21);
            this.ButtonClose.TabIndex = 9;
            this.ButtonClose.UseVisualStyleBackColor = true;
            this.ButtonClose.Click += new System.EventHandler(this.ButtonClose_Click);
            // 
            // ButtonMinimize
            // 
            this.ButtonMinimize.BackColor = System.Drawing.Color.Transparent;
            this.ButtonMinimize.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.ButtonMinimize.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ButtonMinimize.FlatAppearance.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(41)))), ((int)(((byte)(51)))), ((int)(((byte)(61)))));
            this.ButtonMinimize.FlatAppearance.BorderSize = 0;
            this.ButtonMinimize.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
            this.ButtonMinimize.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.ButtonMinimize.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonMinimize.ForeColor = System.Drawing.Color.White;
            this.ButtonMinimize.ImageIndex = 0;
            this.ButtonMinimize.ImageList = this.MinImageList;
            this.ButtonMinimize.Location = new System.Drawing.Point(631, 15);
            this.ButtonMinimize.Name = "ButtonMinimize";
            this.ButtonMinimize.Size = new System.Drawing.Size(21, 21);
            this.ButtonMinimize.TabIndex = 8;
            this.ButtonMinimize.UseVisualStyleBackColor = false;
            this.ButtonMinimize.Click += new System.EventHandler(this.ButtonMinimize_Click);
            // 
            // ButtonStatistics
            // 
            this.ButtonStatistics.BackColor = System.Drawing.Color.Transparent;
            this.ButtonStatistics.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.ButtonStatistics.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ButtonStatistics.FlatAppearance.BorderColor = System.Drawing.SystemColors.MenuBar;
            this.ButtonStatistics.FlatAppearance.BorderSize = 0;
            this.ButtonStatistics.FlatAppearance.CheckedBackColor = System.Drawing.Color.Transparent;
            this.ButtonStatistics.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
            this.ButtonStatistics.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.ButtonStatistics.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonStatistics.ForeColor = System.Drawing.Color.Transparent;
            this.ButtonStatistics.ImageIndex = 0;
            this.ButtonStatistics.ImageList = this.StatisticsImageList;
            this.ButtonStatistics.Location = new System.Drawing.Point(7, 230);
            this.ButtonStatistics.Name = "ButtonStatistics";
            this.ButtonStatistics.Size = new System.Drawing.Size(144, 51);
            this.ButtonStatistics.TabIndex = 3;
            this.ButtonStatistics.UseVisualStyleBackColor = false;
            this.ButtonStatistics.MouseClick += new System.Windows.Forms.MouseEventHandler(this.ButtonStatistics_MouseClick);
            // 
            // ButtonBackUp
            // 
            this.ButtonBackUp.BackColor = System.Drawing.SystemColors.MenuBar;
            this.ButtonBackUp.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.ButtonBackUp.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ButtonBackUp.FlatAppearance.BorderColor = System.Drawing.SystemColors.MenuBar;
            this.ButtonBackUp.FlatAppearance.BorderSize = 0;
            this.ButtonBackUp.FlatAppearance.CheckedBackColor = System.Drawing.Color.Transparent;
            this.ButtonBackUp.FlatAppearance.MouseDownBackColor = System.Drawing.SystemColors.MenuBar;
            this.ButtonBackUp.FlatAppearance.MouseOverBackColor = System.Drawing.SystemColors.MenuBar;
            this.ButtonBackUp.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonBackUp.Font = new System.Drawing.Font("Microsoft Sans Serif", 20.25F, System.Drawing.FontStyle.Bold);
            this.ButtonBackUp.ForeColor = System.Drawing.Color.Transparent;
            this.ButtonBackUp.ImageIndex = 0;
            this.ButtonBackUp.ImageList = this.BackUpImageList;
            this.ButtonBackUp.Location = new System.Drawing.Point(8, 183);
            this.ButtonBackUp.Name = "ButtonBackUp";
            this.ButtonBackUp.Size = new System.Drawing.Size(144, 51);
            this.ButtonBackUp.TabIndex = 2;
            this.ButtonBackUp.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.ButtonBackUp.TextImageRelation = System.Windows.Forms.TextImageRelation.TextBeforeImage;
            this.ButtonBackUp.UseVisualStyleBackColor = false;
            this.ButtonBackUp.Click += new System.EventHandler(this.ButtonBackUp_Click);
            // 
            // ButtonExtras
            // 
            this.ButtonExtras.BackColor = System.Drawing.Color.Transparent;
            this.ButtonExtras.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.ButtonExtras.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ButtonExtras.FlatAppearance.BorderColor = System.Drawing.SystemColors.MenuBar;
            this.ButtonExtras.FlatAppearance.BorderSize = 0;
            this.ButtonExtras.FlatAppearance.CheckedBackColor = System.Drawing.Color.Transparent;
            this.ButtonExtras.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
            this.ButtonExtras.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.ButtonExtras.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonExtras.Font = new System.Drawing.Font("Microsoft Sans Serif", 20.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.ButtonExtras.ForeColor = System.Drawing.Color.Transparent;
            this.ButtonExtras.ImageIndex = 0;
            this.ButtonExtras.ImageList = this.ExtrasImageList;
            this.ButtonExtras.Location = new System.Drawing.Point(7, 131);
            this.ButtonExtras.Name = "ButtonExtras";
            this.ButtonExtras.Size = new System.Drawing.Size(144, 51);
            this.ButtonExtras.TabIndex = 1;
            this.ButtonExtras.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.ButtonExtras.TextImageRelation = System.Windows.Forms.TextImageRelation.TextBeforeImage;
            this.ButtonExtras.UseVisualStyleBackColor = false;
            this.ButtonExtras.Click += new System.EventHandler(this.ButtonExtras_Click);
            // 
            // ButtonScan
            // 
            this.ButtonScan.BackColor = System.Drawing.Color.Transparent;
            this.ButtonScan.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.ButtonScan.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ButtonScan.FlatAppearance.BorderColor = System.Drawing.SystemColors.MenuBar;
            this.ButtonScan.FlatAppearance.BorderSize = 0;
            this.ButtonScan.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
            this.ButtonScan.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.ButtonScan.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonScan.ForeColor = System.Drawing.Color.Transparent;
            this.ButtonScan.ImageIndex = 0;
            this.ButtonScan.ImageList = this.ScanButtonImageList;
            this.ButtonScan.Location = new System.Drawing.Point(7, 80);
            this.ButtonScan.Margin = new System.Windows.Forms.Padding(0);
            this.ButtonScan.Name = "ButtonScan";
            this.ButtonScan.Size = new System.Drawing.Size(144, 51);
            this.ButtonScan.TabIndex = 0;
            this.ButtonScan.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.ButtonScan.TextImageRelation = System.Windows.Forms.TextImageRelation.TextBeforeImage;
            this.ButtonScan.UseVisualStyleBackColor = false;
            this.ButtonScan.Click += new System.EventHandler(this.ButtonScan_Click);
            // 
            // MainWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.Magenta;
            this.BackgroundImage = global::FrontLineGUI.Properties.Resources.Background;
            this.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.ClientSize = new System.Drawing.Size(686, 534);
            this.Controls.Add(this.NeedHelp2Button);
            this.Controls.Add(this.wizControl);
            this.Controls.Add(this.ButtonHelp);
            this.Controls.Add(this.ButtonUpdate);
            this.Controls.Add(this.ButtonSettings);
            this.Controls.Add(this.ButtonPCUtils);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.ButtonNeedHelp);
            this.Controls.Add(this.ButtonClose);
            this.Controls.Add(this.ButtonMinimize);
            this.Controls.Add(this.ButtonStatistics);
            this.Controls.Add(this.ButtonBackUp);
            this.Controls.Add(this.ButtonExtras);
            this.Controls.Add(this.ButtonScan);
            this.Cursor = System.Windows.Forms.Cursors.Default;
            this.DoubleBuffered = true;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MaximumSize = new System.Drawing.Size(686, 534);
            this.Name = "MainWindow";
            this.Opacity = 0.03D;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "FrontlineRegistryCleaner";
            this.TopMost = true;
            this.TransparencyKey = System.Drawing.Color.Magenta;
            this.Resize += new System.EventHandler(this.MainWindow_Resize);
            this.wizControl.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private CustomFrontLineButton ButtonScan;
        private CustomFrontLineButton ButtonExtras;
        private CustomFrontLineButton ButtonStatistics;
        private CustomFrontLineButton ButtonBackUp;
        private CustomFrontLineButton ButtonMinimize;
        private CustomFrontLineButton ButtonClose;
        private CustomFrontLineButton ButtonNeedHelp;
        private System.Windows.Forms.ImageList ScanButtonImageList;
        private System.Windows.Forms.ImageList ExtrasImageList;
        private System.Windows.Forms.ImageList BackUpImageList;
        private System.Windows.Forms.ImageList StatisticsImageList;
        private System.Windows.Forms.ImageList StartScanImageList;
        private System.Windows.Forms.ImageList MinImageList;
        private System.Windows.Forms.ImageList CloseImageList;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private CustomFrontLineButton ButtonPCUtils;
        private System.Windows.Forms.ImageList PCUtilsImageList;
        private System.Windows.Forms.ImageList SettingsLinkImageList;
        private System.Windows.Forms.ImageList UpdateImageList;
        private System.Windows.Forms.ImageList HelpImageList;
        private CustomFrontLineButton ButtonSettings;
        private CustomFrontLineButton ButtonUpdate;
        private CustomFrontLineButton ButtonHelp;
        private WizzardControl wizControl;
        private System.Windows.Forms.TabPage ScanPage;
        private System.Windows.Forms.TabPage StatisticsPage;
        private System.Windows.Forms.TabPage ExtrasPage;
        private System.Windows.Forms.TabPage DefragStep1;
        private System.Windows.Forms.TabPage DefragStep2;
        private System.Windows.Forms.TabPage SettingsPage;
        private System.Windows.Forms.TabPage StartupPage;
        private TabPage BackUpPage;
        private TabPage ScanningPage;
        private ImageList NeedHelp2ImageList;
        private CustomFrontLineButton NeedHelp2Button;
        private TabPage FixingPage;
    }
}

