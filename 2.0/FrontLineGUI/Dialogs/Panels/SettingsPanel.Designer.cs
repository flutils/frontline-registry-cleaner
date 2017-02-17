namespace FrontLineGUI
{
    partial class SettingsPanel
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(SettingsPanel));
            this.panelSettings = new System.Windows.Forms.Panel();
            this.buttonRemoveIgnoreKey = new System.Windows.Forms.Button();
            this.ButtonSheduling = new FrontLineGUI.CustomFrontLineButton();
            this.ImageListScheduling = new System.Windows.Forms.ImageList(this.components);
            this.ButtonIgnoredKeys = new FrontLineGUI.CustomFrontLineButton();
            this.ImageListIgnoredKeys = new System.Windows.Forms.ImageList(this.components);
            this.ButtonGeneralOptions = new FrontLineGUI.CustomFrontLineButton();
            this.ImageListGenoptions = new System.Windows.Forms.ImageList(this.components);
            this.ButtonUpdateSettings = new FrontLineGUI.CustomFrontLineButton();
            this.ImageListUpdateSettings = new System.Windows.Forms.ImageList(this.components);
            this.label8 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
            this.ButtonRemoveJunksSettings = new FrontLineGUI.CustomFrontLineButton();
            this.RemoveJunksImageList = new System.Windows.Forms.ImageList(this.components);
            this.ButtonRegSettings = new FrontLineGUI.CustomFrontLineButton();
            this.CleanTheRegistryImageList = new System.Windows.Forms.ImageList(this.components);
            this.ButtonDefragSettings = new FrontLineGUI.CustomFrontLineButton();
            this.DefragImageList = new System.Windows.Forms.ImageList(this.components);
            this.ButtonStartupSettings = new FrontLineGUI.CustomFrontLineButton();
            this.EditStartupImageList = new System.Windows.Forms.ImageList(this.components);
            this._PanelSheduling = new System.Windows.Forms.Panel();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this._MinutesUpDown = new System.Windows.Forms.NumericUpDown();
            this._HoursUpDown = new System.Windows.Forms.NumericUpDown();
            this._ComboBoxDay = new System.Windows.Forms.ComboBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this._ComboScheduleType = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this._ButtonApplySchedule = new System.Windows.Forms.Button();
            this._PanelGeneralOptions = new System.Windows.Forms.Panel();
            this.checkBoxAutoShutDown = new System.Windows.Forms.CheckBox();
            this.checkBoxAutoUpdates = new System.Windows.Forms.CheckBox();
            this.checkBoxAutomaticallyRepair = new System.Windows.Forms.CheckBox();
            this.checkBoxSetRestore = new System.Windows.Forms.CheckBox();
            this.checkBoxOnStartUp = new System.Windows.Forms.CheckBox();
            this.listViewIgnoreKeys = new System.Windows.Forms.ListView();
            this.IgnoredKeys = new System.Windows.Forms.ColumnHeader();
            this.panelSettings.SuspendLayout();
            this._PanelSheduling.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this._MinutesUpDown)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this._HoursUpDown)).BeginInit();
            this._PanelGeneralOptions.SuspendLayout();
            this.SuspendLayout();
            // 
            // panelSettings
            // 
            this.panelSettings.BackColor = System.Drawing.Color.White;
            this.panelSettings.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("panelSettings.BackgroundImage")));
            this.panelSettings.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.panelSettings.Controls.Add(this.buttonRemoveIgnoreKey);
            this.panelSettings.Controls.Add(this.ButtonSheduling);
            this.panelSettings.Controls.Add(this.ButtonIgnoredKeys);
            this.panelSettings.Controls.Add(this.ButtonGeneralOptions);
            this.panelSettings.Controls.Add(this.ButtonUpdateSettings);
            this.panelSettings.Controls.Add(this.label8);
            this.panelSettings.Controls.Add(this.label9);
            this.panelSettings.Controls.Add(this.label10);
            this.panelSettings.Controls.Add(this.label11);
            this.panelSettings.Controls.Add(this.ButtonRemoveJunksSettings);
            this.panelSettings.Controls.Add(this.ButtonRegSettings);
            this.panelSettings.Controls.Add(this.ButtonDefragSettings);
            this.panelSettings.Controls.Add(this.ButtonStartupSettings);
            this.panelSettings.Controls.Add(this._PanelSheduling);
            this.panelSettings.Controls.Add(this._PanelGeneralOptions);
            this.panelSettings.Location = new System.Drawing.Point(0, 0);
            this.panelSettings.Name = "panelSettings";
            this.panelSettings.Size = new System.Drawing.Size(470, 444);
            this.panelSettings.TabIndex = 2;
            // 
            // buttonRemoveIgnoreKey
            // 
            this.buttonRemoveIgnoreKey.Location = new System.Drawing.Point(373, 223);
            this.buttonRemoveIgnoreKey.Name = "buttonRemoveIgnoreKey";
            this.buttonRemoveIgnoreKey.Size = new System.Drawing.Size(81, 23);
            this.buttonRemoveIgnoreKey.TabIndex = 10;
            this.buttonRemoveIgnoreKey.Text = "Remove Key";
            this.buttonRemoveIgnoreKey.UseVisualStyleBackColor = true;
            this.buttonRemoveIgnoreKey.Visible = false;
            this.buttonRemoveIgnoreKey.Click += new System.EventHandler(this.buttonRemoveIgnoreKey_Click);
            // 
            // ButtonSheduling
            // 
            this.ButtonSheduling.BackColor = System.Drawing.Color.White;
            this.ButtonSheduling.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.ButtonSheduling.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ButtonSheduling.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.ButtonSheduling.FlatAppearance.BorderSize = 0;
            this.ButtonSheduling.FlatAppearance.CheckedBackColor = System.Drawing.Color.White;
            this.ButtonSheduling.FlatAppearance.MouseDownBackColor = System.Drawing.Color.White;
            this.ButtonSheduling.FlatAppearance.MouseOverBackColor = System.Drawing.Color.White;
            this.ButtonSheduling.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonSheduling.ForeColor = System.Drawing.Color.White;
            this.ButtonSheduling.ImageIndex = 0;
            this.ButtonSheduling.ImageList = this.ImageListScheduling;
            this.ButtonSheduling.Location = new System.Drawing.Point(302, 47);
            this.ButtonSheduling.Name = "ButtonSheduling";
            this.ButtonSheduling.Size = new System.Drawing.Size(107, 37);
            this.ButtonSheduling.TabIndex = 46;
            this.ButtonSheduling.UseVisualStyleBackColor = false;
            this.ButtonSheduling.Click += new System.EventHandler(this.ButtonSheduling_Click);
            // 
            // ImageListScheduling
            // 
            this.ImageListScheduling.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("ImageListScheduling.ImageStream")));
            this.ImageListScheduling.TransparentColor = System.Drawing.Color.White;
            this.ImageListScheduling.Images.SetKeyName(0, "Schedule_button.png");
            this.ImageListScheduling.Images.SetKeyName(1, "ScheduleHover_button.png");
            // 
            // ButtonIgnoredKeys
            // 
            this.ButtonIgnoredKeys.BackColor = System.Drawing.Color.White;
            this.ButtonIgnoredKeys.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.ButtonIgnoredKeys.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ButtonIgnoredKeys.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.ButtonIgnoredKeys.FlatAppearance.BorderSize = 0;
            this.ButtonIgnoredKeys.FlatAppearance.CheckedBackColor = System.Drawing.Color.White;
            this.ButtonIgnoredKeys.FlatAppearance.MouseDownBackColor = System.Drawing.Color.White;
            this.ButtonIgnoredKeys.FlatAppearance.MouseOverBackColor = System.Drawing.Color.White;
            this.ButtonIgnoredKeys.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonIgnoredKeys.ForeColor = System.Drawing.Color.White;
            this.ButtonIgnoredKeys.ImageIndex = 0;
            this.ButtonIgnoredKeys.ImageList = this.ImageListIgnoredKeys;
            this.ButtonIgnoredKeys.Location = new System.Drawing.Point(183, 47);
            this.ButtonIgnoredKeys.Name = "ButtonIgnoredKeys";
            this.ButtonIgnoredKeys.Size = new System.Drawing.Size(107, 37);
            this.ButtonIgnoredKeys.TabIndex = 45;
            this.ButtonIgnoredKeys.UseVisualStyleBackColor = false;
            this.ButtonIgnoredKeys.Click += new System.EventHandler(this.ButtonIgnoredKeys_Click);
            // 
            // ImageListIgnoredKeys
            // 
            this.ImageListIgnoredKeys.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("ImageListIgnoredKeys.ImageStream")));
            this.ImageListIgnoredKeys.TransparentColor = System.Drawing.Color.White;
            this.ImageListIgnoredKeys.Images.SetKeyName(0, "IgnoredKey_button.png");
            this.ImageListIgnoredKeys.Images.SetKeyName(1, "IgnoredKeyHover_button.png");
            // 
            // ButtonGeneralOptions
            // 
            this.ButtonGeneralOptions.BackColor = System.Drawing.Color.White;
            this.ButtonGeneralOptions.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.ButtonGeneralOptions.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ButtonGeneralOptions.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.ButtonGeneralOptions.FlatAppearance.BorderSize = 0;
            this.ButtonGeneralOptions.FlatAppearance.CheckedBackColor = System.Drawing.Color.White;
            this.ButtonGeneralOptions.FlatAppearance.MouseDownBackColor = System.Drawing.Color.White;
            this.ButtonGeneralOptions.FlatAppearance.MouseOverBackColor = System.Drawing.Color.White;
            this.ButtonGeneralOptions.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonGeneralOptions.ForeColor = System.Drawing.Color.White;
            this.ButtonGeneralOptions.ImageIndex = 0;
            this.ButtonGeneralOptions.ImageList = this.ImageListGenoptions;
            this.ButtonGeneralOptions.Location = new System.Drawing.Point(65, 47);
            this.ButtonGeneralOptions.Name = "ButtonGeneralOptions";
            this.ButtonGeneralOptions.Size = new System.Drawing.Size(107, 37);
            this.ButtonGeneralOptions.TabIndex = 44;
            this.ButtonGeneralOptions.UseVisualStyleBackColor = false;
            this.ButtonGeneralOptions.Click += new System.EventHandler(this.ButtonGeneralOptions_Click);
            // 
            // ImageListGenoptions
            // 
            this.ImageListGenoptions.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("ImageListGenoptions.ImageStream")));
            this.ImageListGenoptions.TransparentColor = System.Drawing.Color.White;
            this.ImageListGenoptions.Images.SetKeyName(0, "GeneralOptions_button.png");
            this.ImageListGenoptions.Images.SetKeyName(1, "GeneralOptionsHover_button.png");
            // 
            // ButtonUpdateSettings
            // 
            this.ButtonUpdateSettings.BackColor = System.Drawing.Color.White;
            this.ButtonUpdateSettings.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ButtonUpdateSettings.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.ButtonUpdateSettings.FlatAppearance.BorderSize = 0;
            this.ButtonUpdateSettings.FlatAppearance.CheckedBackColor = System.Drawing.Color.White;
            this.ButtonUpdateSettings.FlatAppearance.MouseDownBackColor = System.Drawing.Color.White;
            this.ButtonUpdateSettings.FlatAppearance.MouseOverBackColor = System.Drawing.Color.White;
            this.ButtonUpdateSettings.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonUpdateSettings.ForeColor = System.Drawing.Color.White;
            this.ButtonUpdateSettings.ImageIndex = 0;
            this.ButtonUpdateSettings.ImageList = this.ImageListUpdateSettings;
            this.ButtonUpdateSettings.Location = new System.Drawing.Point(176, 278);
            this.ButtonUpdateSettings.Name = "ButtonUpdateSettings";
            this.ButtonUpdateSettings.Size = new System.Drawing.Size(123, 41);
            this.ButtonUpdateSettings.TabIndex = 43;
            this.ButtonUpdateSettings.UseVisualStyleBackColor = false;
            this.ButtonUpdateSettings.Click += new System.EventHandler(this.ButtonUpdateSettings_Click);
            // 
            // ImageListUpdateSettings
            // 
            this.ImageListUpdateSettings.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("ImageListUpdateSettings.ImageStream")));
            this.ImageListUpdateSettings.TransparentColor = System.Drawing.Color.White;
            this.ImageListUpdateSettings.Images.SetKeyName(0, "UpdateBig_button.png");
            this.ImageListUpdateSettings.Images.SetKeyName(1, "UpdateBigHover_button.png");
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.BackColor = System.Drawing.Color.Transparent;
            this.label8.Font = new System.Drawing.Font("Arial", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.label8.ForeColor = System.Drawing.SystemColors.ControlDarkDark;
            this.label8.Location = new System.Drawing.Point(389, 421);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(54, 12);
            this.label8.TabIndex = 42;
            this.label8.Text = "Edit Startup";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.BackColor = System.Drawing.Color.Transparent;
            this.label9.Font = new System.Drawing.Font("Arial", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.label9.ForeColor = System.Drawing.SystemColors.ControlDarkDark;
            this.label9.Location = new System.Drawing.Point(254, 421);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(112, 12);
            this.label9.TabIndex = 41;
            this.label9.Text = "Defragment The Registry";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.BackColor = System.Drawing.Color.Transparent;
            this.label10.Font = new System.Drawing.Font("Arial", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.label10.ForeColor = System.Drawing.SystemColors.ControlDarkDark;
            this.label10.Location = new System.Drawing.Point(144, 421);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(87, 12);
            this.label10.TabIndex = 40;
            this.label10.Text = "Remove Junk Files";
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.BackColor = System.Drawing.Color.Transparent;
            this.label11.Font = new System.Drawing.Font("Arial", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.label11.ForeColor = System.Drawing.SystemColors.ControlDarkDark;
            this.label11.Location = new System.Drawing.Point(29, 421);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(86, 12);
            this.label11.TabIndex = 39;
            this.label11.Text = "Clean The Registry";
            // 
            // ButtonRemoveJunksSettings
            // 
            this.ButtonRemoveJunksSettings.BackColor = System.Drawing.Color.White;
            this.ButtonRemoveJunksSettings.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.ButtonRemoveJunksSettings.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ButtonRemoveJunksSettings.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.ButtonRemoveJunksSettings.FlatAppearance.BorderSize = 0;
            this.ButtonRemoveJunksSettings.FlatAppearance.CheckedBackColor = System.Drawing.Color.White;
            this.ButtonRemoveJunksSettings.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
            this.ButtonRemoveJunksSettings.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.ButtonRemoveJunksSettings.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonRemoveJunksSettings.ForeColor = System.Drawing.Color.White;
            this.ButtonRemoveJunksSettings.ImageAlign = System.Drawing.ContentAlignment.TopCenter;
            this.ButtonRemoveJunksSettings.ImageIndex = 0;
            this.ButtonRemoveJunksSettings.ImageList = this.RemoveJunksImageList;
            this.ButtonRemoveJunksSettings.Location = new System.Drawing.Point(143, 374);
            this.ButtonRemoveJunksSettings.Name = "ButtonRemoveJunksSettings";
            this.ButtonRemoveJunksSettings.Size = new System.Drawing.Size(91, 38);
            this.ButtonRemoveJunksSettings.TabIndex = 36;
            this.ButtonRemoveJunksSettings.UseVisualStyleBackColor = false;
            this.ButtonRemoveJunksSettings.Click += new System.EventHandler(this.ButtonRemoveJunksSettings_Click);
            // 
            // RemoveJunksImageList
            // 
            this.RemoveJunksImageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("RemoveJunksImageList.ImageStream")));
            this.RemoveJunksImageList.TransparentColor = System.Drawing.Color.White;
            this.RemoveJunksImageList.Images.SetKeyName(0, "RemoveJunks_button.png");
            this.RemoveJunksImageList.Images.SetKeyName(1, "RemoveJunksHover_button.png");
            // 
            // ButtonRegSettings
            // 
            this.ButtonRegSettings.BackColor = System.Drawing.Color.White;
            this.ButtonRegSettings.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.ButtonRegSettings.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ButtonRegSettings.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.ButtonRegSettings.FlatAppearance.BorderSize = 0;
            this.ButtonRegSettings.FlatAppearance.CheckedBackColor = System.Drawing.Color.White;
            this.ButtonRegSettings.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
            this.ButtonRegSettings.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.ButtonRegSettings.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonRegSettings.ForeColor = System.Drawing.Color.White;
            this.ButtonRegSettings.ImageAlign = System.Drawing.ContentAlignment.TopCenter;
            this.ButtonRegSettings.ImageIndex = 0;
            this.ButtonRegSettings.ImageList = this.CleanTheRegistryImageList;
            this.ButtonRegSettings.Location = new System.Drawing.Point(34, 375);
            this.ButtonRegSettings.Name = "ButtonRegSettings";
            this.ButtonRegSettings.Size = new System.Drawing.Size(81, 37);
            this.ButtonRegSettings.TabIndex = 35;
            this.ButtonRegSettings.UseVisualStyleBackColor = false;
            this.ButtonRegSettings.Click += new System.EventHandler(this.ButtonRegSettings_Click);
            // 
            // CleanTheRegistryImageList
            // 
            this.CleanTheRegistryImageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("CleanTheRegistryImageList.ImageStream")));
            this.CleanTheRegistryImageList.TransparentColor = System.Drawing.Color.White;
            this.CleanTheRegistryImageList.Images.SetKeyName(0, "ClenRegistry_button.png");
            this.CleanTheRegistryImageList.Images.SetKeyName(1, "ClenRegistryHover_button.png");
            // 
            // ButtonDefragSettings
            // 
            this.ButtonDefragSettings.BackColor = System.Drawing.Color.White;
            this.ButtonDefragSettings.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.ButtonDefragSettings.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ButtonDefragSettings.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.ButtonDefragSettings.FlatAppearance.BorderSize = 0;
            this.ButtonDefragSettings.FlatAppearance.CheckedBackColor = System.Drawing.Color.White;
            this.ButtonDefragSettings.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
            this.ButtonDefragSettings.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.ButtonDefragSettings.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonDefragSettings.ForeColor = System.Drawing.Color.White;
            this.ButtonDefragSettings.ImageAlign = System.Drawing.ContentAlignment.TopCenter;
            this.ButtonDefragSettings.ImageIndex = 0;
            this.ButtonDefragSettings.ImageList = this.DefragImageList;
            this.ButtonDefragSettings.Location = new System.Drawing.Point(261, 367);
            this.ButtonDefragSettings.Name = "ButtonDefragSettings";
            this.ButtonDefragSettings.Size = new System.Drawing.Size(96, 45);
            this.ButtonDefragSettings.TabIndex = 37;
            this.ButtonDefragSettings.UseVisualStyleBackColor = false;
            this.ButtonDefragSettings.Click += new System.EventHandler(this.ButtonDefragSettings_Click);
            // 
            // DefragImageList
            // 
            this.DefragImageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("DefragImageList.ImageStream")));
            this.DefragImageList.TransparentColor = System.Drawing.Color.White;
            this.DefragImageList.Images.SetKeyName(0, "DefragmentRegistry_button.png");
            this.DefragImageList.Images.SetKeyName(1, "DefragmentRegistryHover_button.png");
            // 
            // ButtonStartupSettings
            // 
            this.ButtonStartupSettings.BackColor = System.Drawing.Color.White;
            this.ButtonStartupSettings.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.ButtonStartupSettings.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ButtonStartupSettings.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.ButtonStartupSettings.FlatAppearance.BorderSize = 0;
            this.ButtonStartupSettings.FlatAppearance.CheckedBackColor = System.Drawing.Color.White;
            this.ButtonStartupSettings.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
            this.ButtonStartupSettings.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.ButtonStartupSettings.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonStartupSettings.ForeColor = System.Drawing.Color.White;
            this.ButtonStartupSettings.ImageAlign = System.Drawing.ContentAlignment.TopCenter;
            this.ButtonStartupSettings.ImageIndex = 0;
            this.ButtonStartupSettings.ImageList = this.EditStartupImageList;
            this.ButtonStartupSettings.Location = new System.Drawing.Point(384, 368);
            this.ButtonStartupSettings.Name = "ButtonStartupSettings";
            this.ButtonStartupSettings.Size = new System.Drawing.Size(61, 44);
            this.ButtonStartupSettings.TabIndex = 38;
            this.ButtonStartupSettings.UseVisualStyleBackColor = false;
            this.ButtonStartupSettings.Click += new System.EventHandler(this.ButtonStartupSettings_Click);
            // 
            // EditStartupImageList
            // 
            this.EditStartupImageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("EditStartupImageList.ImageStream")));
            this.EditStartupImageList.TransparentColor = System.Drawing.Color.White;
            this.EditStartupImageList.Images.SetKeyName(0, "EditStartup_button.png");
            this.EditStartupImageList.Images.SetKeyName(1, "EditStartupHover_button.png");
            // 
            // _PanelSheduling
            // 
            this._PanelSheduling.Controls.Add(this.label5);
            this._PanelSheduling.Controls.Add(this.label4);
            this._PanelSheduling.Controls.Add(this._MinutesUpDown);
            this._PanelSheduling.Controls.Add(this._HoursUpDown);
            this._PanelSheduling.Controls.Add(this._ComboBoxDay);
            this._PanelSheduling.Controls.Add(this.label3);
            this._PanelSheduling.Controls.Add(this.label2);
            this._PanelSheduling.Controls.Add(this._ComboScheduleType);
            this._PanelSheduling.Controls.Add(this.label1);
            this._PanelSheduling.Controls.Add(this._ButtonApplySchedule);
            this._PanelSheduling.Location = new System.Drawing.Point(43, 96);
            this._PanelSheduling.Name = "_PanelSheduling";
            this._PanelSheduling.Size = new System.Drawing.Size(389, 161);
            this._PanelSheduling.TabIndex = 49;
            this._PanelSheduling.Visible = false;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(345, 53);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(44, 13);
            this.label5.TabIndex = 9;
            this.label5.Text = "Minutes";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(265, 53);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(35, 13);
            this.label4.TabIndex = 8;
            this.label4.Text = "Hours";
            // 
            // _MinutesUpDown
            // 
            this._MinutesUpDown.Location = new System.Drawing.Point(301, 49);
            this._MinutesUpDown.Maximum = new decimal(new int[] {
            59,
            0,
            0,
            0});
            this._MinutesUpDown.Name = "_MinutesUpDown";
            this._MinutesUpDown.Size = new System.Drawing.Size(38, 20);
            this._MinutesUpDown.TabIndex = 7;
            // 
            // _HoursUpDown
            // 
            this._HoursUpDown.Location = new System.Drawing.Point(225, 49);
            this._HoursUpDown.Maximum = new decimal(new int[] {
            23,
            0,
            0,
            0});
            this._HoursUpDown.Name = "_HoursUpDown";
            this._HoursUpDown.Size = new System.Drawing.Size(38, 20);
            this._HoursUpDown.TabIndex = 6;
            // 
            // _ComboBoxDay
            // 
            this._ComboBoxDay.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this._ComboBoxDay.FormattingEnabled = true;
            this._ComboBoxDay.Items.AddRange(new object[] {
            "Sunday",
            "Monday",
            "Tuesday",
            "Wednesday",
            "Thursday",
            "Friday",
            "Saturday"});
            this._ComboBoxDay.Location = new System.Drawing.Point(225, 22);
            this._ComboBoxDay.Name = "_ComboBoxDay";
            this._ComboBoxDay.Size = new System.Drawing.Size(114, 21);
            this._ComboBoxDay.TabIndex = 5;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(185, 53);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(33, 13);
            this.label3.TabIndex = 4;
            this.label3.Text = "Time:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(185, 28);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(29, 13);
            this.label2.TabIndex = 3;
            this.label2.Text = "Day:";
            // 
            // _ComboScheduleType
            // 
            this._ComboScheduleType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this._ComboScheduleType.FormattingEnabled = true;
            this._ComboScheduleType.Items.AddRange(new object[] {
            "Weekly",
            "Daily"});
            this._ComboScheduleType.Location = new System.Drawing.Point(88, 22);
            this._ComboScheduleType.Name = "_ComboScheduleType";
            this._ComboScheduleType.Size = new System.Drawing.Size(92, 21);
            this._ComboScheduleType.TabIndex = 2;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 25);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(78, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Schedule type:";
            // 
            // _ButtonApplySchedule
            // 
            this._ButtonApplySchedule.Location = new System.Drawing.Point(264, 101);
            this._ButtonApplySchedule.Name = "_ButtonApplySchedule";
            this._ButtonApplySchedule.Size = new System.Drawing.Size(75, 23);
            this._ButtonApplySchedule.TabIndex = 0;
            this._ButtonApplySchedule.Text = "Apply";
            this._ButtonApplySchedule.UseVisualStyleBackColor = true;
            this._ButtonApplySchedule.Click += new System.EventHandler(this._ButtonApplySchedule_Click);
            // 
            // _PanelGeneralOptions
            // 
            this._PanelGeneralOptions.Controls.Add(this.checkBoxAutoShutDown);
            this._PanelGeneralOptions.Controls.Add(this.checkBoxAutoUpdates);
            this._PanelGeneralOptions.Controls.Add(this.checkBoxAutomaticallyRepair);
            this._PanelGeneralOptions.Controls.Add(this.checkBoxSetRestore);
            this._PanelGeneralOptions.Controls.Add(this.checkBoxOnStartUp);
            this._PanelGeneralOptions.Location = new System.Drawing.Point(43, 96);
            this._PanelGeneralOptions.Name = "_PanelGeneralOptions";
            this._PanelGeneralOptions.Size = new System.Drawing.Size(389, 150);
            this._PanelGeneralOptions.TabIndex = 47;
            // 
            // checkBoxAutoShutDown
            // 
            this.checkBoxAutoShutDown.AutoSize = true;
            this.checkBoxAutoShutDown.Location = new System.Drawing.Point(22, 107);
            this.checkBoxAutoShutDown.Name = "checkBoxAutoShutDown";
            this.checkBoxAutoShutDown.Size = new System.Drawing.Size(244, 17);
            this.checkBoxAutoShutDown.TabIndex = 4;
            this.checkBoxAutoShutDown.Text = "Automatically shutdown application after repair";
            this.checkBoxAutoShutDown.UseVisualStyleBackColor = true;
            this.checkBoxAutoShutDown.CheckedChanged += new System.EventHandler(this.checkBoxAutoShutDown_CheckedChanged);
            // 
            // checkBoxAutoUpdates
            // 
            this.checkBoxAutoUpdates.AutoSize = true;
            this.checkBoxAutoUpdates.Location = new System.Drawing.Point(22, 84);
            this.checkBoxAutoUpdates.Name = "checkBoxAutoUpdates";
            this.checkBoxAutoUpdates.Size = new System.Drawing.Size(200, 17);
            this.checkBoxAutoUpdates.TabIndex = 3;
            this.checkBoxAutoUpdates.Text = "Check automatically updates on start";
            this.checkBoxAutoUpdates.UseVisualStyleBackColor = true;
            this.checkBoxAutoUpdates.CheckedChanged += new System.EventHandler(this.checkBoxAutoUpdates_CheckedChanged);
            // 
            // checkBoxAutomaticallyRepair
            // 
            this.checkBoxAutomaticallyRepair.AutoSize = true;
            this.checkBoxAutomaticallyRepair.Location = new System.Drawing.Point(22, 61);
            this.checkBoxAutomaticallyRepair.Name = "checkBoxAutomaticallyRepair";
            this.checkBoxAutomaticallyRepair.Size = new System.Drawing.Size(172, 17);
            this.checkBoxAutomaticallyRepair.TabIndex = 2;
            this.checkBoxAutomaticallyRepair.Text = "Automatically repairs after scan";
            this.checkBoxAutomaticallyRepair.UseVisualStyleBackColor = true;
            this.checkBoxAutomaticallyRepair.CheckedChanged += new System.EventHandler(this.checkBoxAutomaticallyRepair_CheckedChanged);
            // 
            // checkBoxSetRestore
            // 
            this.checkBoxSetRestore.AutoSize = true;
            this.checkBoxSetRestore.Location = new System.Drawing.Point(22, 38);
            this.checkBoxSetRestore.Name = "checkBoxSetRestore";
            this.checkBoxSetRestore.Size = new System.Drawing.Size(165, 17);
            this.checkBoxSetRestore.TabIndex = 1;
            this.checkBoxSetRestore.Text = "Set restore point before repair";
            this.checkBoxSetRestore.UseVisualStyleBackColor = true;
            this.checkBoxSetRestore.CheckedChanged += new System.EventHandler(this.checkBoxSetRestore_CheckedChanged);
            // 
            // checkBoxOnStartUp
            // 
            this.checkBoxOnStartUp.AutoSize = true;
            this.checkBoxOnStartUp.Location = new System.Drawing.Point(22, 15);
            this.checkBoxOnStartUp.Name = "checkBoxOnStartUp";
            this.checkBoxOnStartUp.Size = new System.Drawing.Size(102, 17);
            this.checkBoxOnStartUp.TabIndex = 0;
            this.checkBoxOnStartUp.Text = "Start on StartUp";
            this.checkBoxOnStartUp.UseVisualStyleBackColor = true;
            this.checkBoxOnStartUp.CheckedChanged += new System.EventHandler(this.checkBoxOnStartUp_CheckedChanged);
            // 
            // listViewIgnoreKeys
            // 
            this.listViewIgnoreKeys.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.IgnoredKeys});
            this.listViewIgnoreKeys.FullRowSelect = true;
            this.listViewIgnoreKeys.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
            this.listViewIgnoreKeys.Location = new System.Drawing.Point(21, 99);
            this.listViewIgnoreKeys.Name = "listViewIgnoreKeys";
            this.listViewIgnoreKeys.Size = new System.Drawing.Size(339, 150);
            this.listViewIgnoreKeys.TabIndex = 0;
            this.listViewIgnoreKeys.UseCompatibleStateImageBehavior = false;
            this.listViewIgnoreKeys.View = System.Windows.Forms.View.Details;
            // 
            // IgnoredKeys
            // 
            this.IgnoredKeys.Text = "Ignored Keys";
            this.IgnoredKeys.Width = 360;
            // 
            // SettingsPanel
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.panelSettings);
            this.Controls.Add(this.listViewIgnoreKeys);
            this.Name = "SettingsPanel";
            this.Size = new System.Drawing.Size(470, 444);
            this.panelSettings.ResumeLayout(false);
            this.panelSettings.PerformLayout();
            this._PanelSheduling.ResumeLayout(false);
            this._PanelSheduling.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this._MinutesUpDown)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this._HoursUpDown)).EndInit();
            this._PanelGeneralOptions.ResumeLayout(false);
            this._PanelGeneralOptions.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panelSettings;
        private CustomFrontLineButton ButtonSheduling;
        private CustomFrontLineButton ButtonIgnoredKeys;
        private CustomFrontLineButton ButtonGeneralOptions;
        private CustomFrontLineButton ButtonUpdateSettings;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label label11;
        private CustomFrontLineButton ButtonRemoveJunksSettings;
        private CustomFrontLineButton ButtonRegSettings;
        private CustomFrontLineButton ButtonDefragSettings;
        private CustomFrontLineButton ButtonStartupSettings;
        private System.Windows.Forms.ImageList ImageListScheduling;
        private System.Windows.Forms.ImageList ImageListIgnoredKeys;
        private System.Windows.Forms.ImageList ImageListGenoptions;
        private System.Windows.Forms.ImageList ImageListUpdateSettings;
        private System.Windows.Forms.ImageList RemoveJunksImageList;
        private System.Windows.Forms.ImageList CleanTheRegistryImageList;
        private System.Windows.Forms.ImageList DefragImageList;
        private System.Windows.Forms.ImageList EditStartupImageList;
        private System.Windows.Forms.Panel _PanelGeneralOptions;
        private System.Windows.Forms.CheckBox checkBoxAutoUpdates;
        private System.Windows.Forms.CheckBox checkBoxAutomaticallyRepair;
        private System.Windows.Forms.CheckBox checkBoxSetRestore;
        private System.Windows.Forms.CheckBox checkBoxOnStartUp;
        private System.Windows.Forms.CheckBox checkBoxAutoShutDown;
        private System.Windows.Forms.Panel _PanelSheduling;
        private System.Windows.Forms.Button _ButtonApplySchedule;
        public System.Windows.Forms.ListView listViewIgnoreKeys;
        private System.Windows.Forms.ColumnHeader IgnoredKeys;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox _ComboBoxDay;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ComboBox _ComboScheduleType;
        private System.Windows.Forms.NumericUpDown _HoursUpDown;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.NumericUpDown _MinutesUpDown;
        private System.Windows.Forms.Button buttonRemoveIgnoreKey;
    }
}
