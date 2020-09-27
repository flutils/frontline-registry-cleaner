namespace FrontLineGUI
{
    partial class StartScanPanel
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(StartScanPanel));
            this.panelStartScan = new System.Windows.Forms.Panel();
            this.LabelStartYourScan = new System.Windows.Forms.Label();
            this.labelLastScanPerformed = new System.Windows.Forms.Label();
            this.CheckButtonRecycleBin = new FrontLineGUI.CustomFrontLineCheckButton();
            this.CheckedUncheckedImageList = new System.Windows.Forms.ImageList(this.components);
            this.CheckButtonTempInternet = new FrontLineGUI.CustomFrontLineCheckButton();
            this.LabelEditStartUp = new System.Windows.Forms.Label();
            this.CheckButtonDLLErrors = new FrontLineGUI.CustomFrontLineCheckButton();
            this.LabelDefragReg = new System.Windows.Forms.Label();
            this.CheckButtonTempFiles = new FrontLineGUI.CustomFrontLineCheckButton();
            this.LabelRemoveJunks = new System.Windows.Forms.Label();
            this.CheckButtonInvalidFiles = new FrontLineGUI.CustomFrontLineCheckButton();
            this.LabelCleanReg = new System.Windows.Forms.Label();
            this.CheckButtonAppErrors = new FrontLineGUI.CustomFrontLineCheckButton();
            this.CheckButtonWinErrors = new FrontLineGUI.CustomFrontLineCheckButton();
            this.CheckButtonOCXErrors = new FrontLineGUI.CustomFrontLineCheckButton();
            this.CheckButtonRegErrors = new FrontLineGUI.CustomFrontLineCheckButton();
            this.ButtonStartScanMain = new FrontLineGUI.CustomFrontLineButton();
            this.ButtonRemoveJunks = new FrontLineGUI.CustomFrontLineButton();
            this.RemoveJunksImageList = new System.Windows.Forms.ImageList(this.components);
            this.ButtonCleanRegistry = new FrontLineGUI.CustomFrontLineButton();
            this.CleanTheRegistryImageList = new System.Windows.Forms.ImageList(this.components);
            this.ButtonDefragRegistry = new FrontLineGUI.CustomFrontLineButton();
            this.DefragImageList = new System.Windows.Forms.ImageList(this.components);
            this.ButtonEditStartUp = new FrontLineGUI.CustomFrontLineButton();
            this.EditStartupImageList = new System.Windows.Forms.ImageList(this.components);
            this.panelStartScan.SuspendLayout();
            this.SuspendLayout();
            // 
            // panelStartScan
            // 
            this.panelStartScan.BackColor = System.Drawing.Color.Transparent;
            this.panelStartScan.BackgroundImage = global::FrontLineGUI.Properties.Resources.ScanPanelBackground;
            this.panelStartScan.Controls.Add(this.LabelStartYourScan);
            this.panelStartScan.Controls.Add(this.labelLastScanPerformed);
            this.panelStartScan.Controls.Add(this.CheckButtonRecycleBin);
            this.panelStartScan.Controls.Add(this.CheckButtonTempInternet);
            this.panelStartScan.Controls.Add(this.LabelEditStartUp);
            this.panelStartScan.Controls.Add(this.CheckButtonDLLErrors);
            this.panelStartScan.Controls.Add(this.LabelDefragReg);
            this.panelStartScan.Controls.Add(this.CheckButtonTempFiles);
            this.panelStartScan.Controls.Add(this.LabelRemoveJunks);
            this.panelStartScan.Controls.Add(this.CheckButtonInvalidFiles);
            this.panelStartScan.Controls.Add(this.LabelCleanReg);
            this.panelStartScan.Controls.Add(this.CheckButtonAppErrors);
            this.panelStartScan.Controls.Add(this.CheckButtonWinErrors);
            this.panelStartScan.Controls.Add(this.CheckButtonOCXErrors);
            this.panelStartScan.Controls.Add(this.CheckButtonRegErrors);
            this.panelStartScan.Controls.Add(this.ButtonStartScanMain);
            this.panelStartScan.Controls.Add(this.ButtonRemoveJunks);
            this.panelStartScan.Controls.Add(this.ButtonCleanRegistry);
            this.panelStartScan.Controls.Add(this.ButtonDefragRegistry);
            this.panelStartScan.Controls.Add(this.ButtonEditStartUp);
            this.panelStartScan.Location = new System.Drawing.Point(0, 0);
            this.panelStartScan.Name = "panelStartScan";
            this.panelStartScan.Size = new System.Drawing.Size(470, 444);
            this.panelStartScan.TabIndex = 40;
            // 
            // LabelStartYourScan
            // 
            this.LabelStartYourScan.AutoSize = true;
            this.LabelStartYourScan.BackColor = System.Drawing.Color.White;
            this.LabelStartYourScan.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.LabelStartYourScan.ForeColor = System.Drawing.SystemColors.ControlDarkDark;
            this.LabelStartYourScan.Image = global::FrontLineGUI.Properties.Resources.SartYourScanImg;
            this.LabelStartYourScan.Location = new System.Drawing.Point(56, 25);
            this.LabelStartYourScan.Name = "LabelStartYourScan";
            this.LabelStartYourScan.Size = new System.Drawing.Size(335, 24);
            this.LabelStartYourScan.TabIndex = 30;
            this.LabelStartYourScan.Text = "                                                                 ";
            // 
            // labelLastScanPerformed
            // 
            this.labelLastScanPerformed.AutoSize = true;
            this.labelLastScanPerformed.BackColor = System.Drawing.Color.White;
            this.labelLastScanPerformed.Font = new System.Drawing.Font("Arial", 9F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))));
            this.labelLastScanPerformed.ForeColor = System.Drawing.SystemColors.ControlDarkDark;
            this.labelLastScanPerformed.Location = new System.Drawing.Point(125, 169);
            this.labelLastScanPerformed.Name = "labelLastScanPerformed";
            this.labelLastScanPerformed.Size = new System.Drawing.Size(185, 15);
            this.labelLastScanPerformed.TabIndex = 24;
            this.labelLastScanPerformed.Text = "Last scan performed on: Never";
            // 
            // CheckButtonRecycleBin
            // 
            this.CheckButtonRecycleBin.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
            this.CheckButtonRecycleBin.BackColor = System.Drawing.Color.White;
            this.CheckButtonRecycleBin.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.CheckButtonRecycleBin.Cursor = System.Windows.Forms.Cursors.Hand;
            this.CheckButtonRecycleBin.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.CheckButtonRecycleBin.FlatAppearance.BorderSize = 0;
            this.CheckButtonRecycleBin.FlatAppearance.CheckedBackColor = System.Drawing.Color.White;
            this.CheckButtonRecycleBin.FlatAppearance.MouseDownBackColor = System.Drawing.Color.White;
            this.CheckButtonRecycleBin.FlatAppearance.MouseOverBackColor = System.Drawing.Color.White;
            this.CheckButtonRecycleBin.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.CheckButtonRecycleBin.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.CheckButtonRecycleBin.ForeColor = System.Drawing.Color.Gray;
            this.CheckButtonRecycleBin.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.CheckButtonRecycleBin.ImageIndex = 1;
            this.CheckButtonRecycleBin.ImageList = this.CheckedUncheckedImageList;
            this.CheckButtonRecycleBin.Location = new System.Drawing.Point(321, 263);
            this.CheckButtonRecycleBin.Name = "CheckButtonRecycleBin";
            this.CheckButtonRecycleBin.ScannersToActivate = "20502;20405;";
            this.CheckButtonRecycleBin.Size = new System.Drawing.Size(132, 27);
            this.CheckButtonRecycleBin.TabIndex = 23;
            this.CheckButtonRecycleBin.Text = "  Recycle Bin";
            this.CheckButtonRecycleBin.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.CheckButtonRecycleBin.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.CheckButtonRecycleBin.UseVisualStyleBackColor = false;
            // 
            // CheckedUncheckedImageList
            // 
            this.CheckedUncheckedImageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("CheckedUncheckedImageList.ImageStream")));
            this.CheckedUncheckedImageList.TransparentColor = System.Drawing.Color.White;
            this.CheckedUncheckedImageList.Images.SetKeyName(0, "Unchecked.png");
            this.CheckedUncheckedImageList.Images.SetKeyName(1, "Checked.png");
            // 
            // CheckButtonTempInternet
            // 
            this.CheckButtonTempInternet.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
            this.CheckButtonTempInternet.BackColor = System.Drawing.Color.White;
            this.CheckButtonTempInternet.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.CheckButtonTempInternet.Cursor = System.Windows.Forms.Cursors.Hand;
            this.CheckButtonTempInternet.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.CheckButtonTempInternet.FlatAppearance.BorderSize = 0;
            this.CheckButtonTempInternet.FlatAppearance.CheckedBackColor = System.Drawing.Color.White;
            this.CheckButtonTempInternet.FlatAppearance.MouseDownBackColor = System.Drawing.Color.White;
            this.CheckButtonTempInternet.FlatAppearance.MouseOverBackColor = System.Drawing.Color.White;
            this.CheckButtonTempInternet.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.CheckButtonTempInternet.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.CheckButtonTempInternet.ForeColor = System.Drawing.Color.Gray;
            this.CheckButtonTempInternet.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.CheckButtonTempInternet.ImageIndex = 1;
            this.CheckButtonTempInternet.ImageList = this.CheckedUncheckedImageList;
            this.CheckButtonTempInternet.Location = new System.Drawing.Point(321, 285);
            this.CheckButtonTempInternet.Name = "CheckButtonTempInternet";
            this.CheckButtonTempInternet.ScannersToActivate = "20314;20317;20403;20404;20501;";
            this.CheckButtonTempInternet.Size = new System.Drawing.Size(138, 27);
            this.CheckButtonTempInternet.TabIndex = 22;
            this.CheckButtonTempInternet.Text = "  Internet Cache";
            this.CheckButtonTempInternet.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.CheckButtonTempInternet.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.CheckButtonTempInternet.UseVisualStyleBackColor = false;
            // 
            // LabelEditStartUp
            // 
            this.LabelEditStartUp.AutoSize = true;
            this.LabelEditStartUp.BackColor = System.Drawing.Color.Transparent;
            this.LabelEditStartUp.Font = new System.Drawing.Font("Arial", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.LabelEditStartUp.ForeColor = System.Drawing.SystemColors.ControlDarkDark;
            this.LabelEditStartUp.Location = new System.Drawing.Point(389, 421);
            this.LabelEditStartUp.Name = "LabelEditStartUp";
            this.LabelEditStartUp.Size = new System.Drawing.Size(54, 12);
            this.LabelEditStartUp.TabIndex = 34;
            this.LabelEditStartUp.Text = "Edit Startup";
            // 
            // CheckButtonDLLErrors
            // 
            this.CheckButtonDLLErrors.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
            this.CheckButtonDLLErrors.BackColor = System.Drawing.Color.White;
            this.CheckButtonDLLErrors.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.CheckButtonDLLErrors.Cursor = System.Windows.Forms.Cursors.Hand;
            this.CheckButtonDLLErrors.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.CheckButtonDLLErrors.FlatAppearance.BorderSize = 0;
            this.CheckButtonDLLErrors.FlatAppearance.CheckedBackColor = System.Drawing.Color.White;
            this.CheckButtonDLLErrors.FlatAppearance.MouseDownBackColor = System.Drawing.Color.White;
            this.CheckButtonDLLErrors.FlatAppearance.MouseOverBackColor = System.Drawing.Color.White;
            this.CheckButtonDLLErrors.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.CheckButtonDLLErrors.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.CheckButtonDLLErrors.ForeColor = System.Drawing.Color.Gray;
            this.CheckButtonDLLErrors.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.CheckButtonDLLErrors.ImageIndex = 1;
            this.CheckButtonDLLErrors.ImageList = this.CheckedUncheckedImageList;
            this.CheckButtonDLLErrors.Location = new System.Drawing.Point(180, 240);
            this.CheckButtonDLLErrors.Name = "CheckButtonDLLErrors";
            this.CheckButtonDLLErrors.ScannersToActivate = "20315;";
            this.CheckButtonDLLErrors.Size = new System.Drawing.Size(135, 28);
            this.CheckButtonDLLErrors.TabIndex = 21;
            this.CheckButtonDLLErrors.Text = "  DLL Errors";
            this.CheckButtonDLLErrors.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.CheckButtonDLLErrors.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.CheckButtonDLLErrors.UseVisualStyleBackColor = false;
            // 
            // LabelDefragReg
            // 
            this.LabelDefragReg.AutoSize = true;
            this.LabelDefragReg.BackColor = System.Drawing.Color.Transparent;
            this.LabelDefragReg.Font = new System.Drawing.Font("Arial", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.LabelDefragReg.ForeColor = System.Drawing.SystemColors.ControlDarkDark;
            this.LabelDefragReg.Location = new System.Drawing.Point(254, 421);
            this.LabelDefragReg.Name = "LabelDefragReg";
            this.LabelDefragReg.Size = new System.Drawing.Size(112, 12);
            this.LabelDefragReg.TabIndex = 33;
            this.LabelDefragReg.Text = "Defragment The Registry";
            // 
            // CheckButtonTempFiles
            // 
            this.CheckButtonTempFiles.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
            this.CheckButtonTempFiles.BackColor = System.Drawing.Color.White;
            this.CheckButtonTempFiles.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.CheckButtonTempFiles.Cursor = System.Windows.Forms.Cursors.Hand;
            this.CheckButtonTempFiles.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.CheckButtonTempFiles.FlatAppearance.BorderSize = 0;
            this.CheckButtonTempFiles.FlatAppearance.CheckedBackColor = System.Drawing.Color.White;
            this.CheckButtonTempFiles.FlatAppearance.MouseDownBackColor = System.Drawing.Color.White;
            this.CheckButtonTempFiles.FlatAppearance.MouseOverBackColor = System.Drawing.Color.White;
            this.CheckButtonTempFiles.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.CheckButtonTempFiles.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.CheckButtonTempFiles.ForeColor = System.Drawing.Color.Gray;
            this.CheckButtonTempFiles.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.CheckButtonTempFiles.ImageIndex = 1;
            this.CheckButtonTempFiles.ImageList = this.CheckedUncheckedImageList;
            this.CheckButtonTempFiles.Location = new System.Drawing.Point(321, 240);
            this.CheckButtonTempFiles.Name = "CheckButtonTempFiles";
            this.CheckButtonTempFiles.ScannersToActivate = "20505;20504;20506;20406;20503;20507;20407;";
            this.CheckButtonTempFiles.Size = new System.Drawing.Size(132, 28);
            this.CheckButtonTempFiles.TabIndex = 20;
            this.CheckButtonTempFiles.Text = "  Temp Files";
            this.CheckButtonTempFiles.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.CheckButtonTempFiles.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.CheckButtonTempFiles.UseVisualStyleBackColor = false;
            // 
            // LabelRemoveJunks
            // 
            this.LabelRemoveJunks.AutoSize = true;
            this.LabelRemoveJunks.BackColor = System.Drawing.Color.Transparent;
            this.LabelRemoveJunks.Font = new System.Drawing.Font("Arial", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.LabelRemoveJunks.ForeColor = System.Drawing.SystemColors.ControlDarkDark;
            this.LabelRemoveJunks.Location = new System.Drawing.Point(144, 421);
            this.LabelRemoveJunks.Name = "LabelRemoveJunks";
            this.LabelRemoveJunks.Size = new System.Drawing.Size(87, 12);
            this.LabelRemoveJunks.TabIndex = 32;
            this.LabelRemoveJunks.Text = "Remove Junk Files";
            // 
            // CheckButtonInvalidFiles
            // 
            this.CheckButtonInvalidFiles.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
            this.CheckButtonInvalidFiles.BackColor = System.Drawing.Color.White;
            this.CheckButtonInvalidFiles.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.CheckButtonInvalidFiles.Cursor = System.Windows.Forms.Cursors.Hand;
            this.CheckButtonInvalidFiles.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.CheckButtonInvalidFiles.FlatAppearance.BorderSize = 0;
            this.CheckButtonInvalidFiles.FlatAppearance.CheckedBackColor = System.Drawing.Color.White;
            this.CheckButtonInvalidFiles.FlatAppearance.MouseDownBackColor = System.Drawing.Color.White;
            this.CheckButtonInvalidFiles.FlatAppearance.MouseOverBackColor = System.Drawing.Color.White;
            this.CheckButtonInvalidFiles.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.CheckButtonInvalidFiles.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.CheckButtonInvalidFiles.ForeColor = System.Drawing.Color.Gray;
            this.CheckButtonInvalidFiles.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.CheckButtonInvalidFiles.ImageIndex = 1;
            this.CheckButtonInvalidFiles.ImageList = this.CheckedUncheckedImageList;
            this.CheckButtonInvalidFiles.Location = new System.Drawing.Point(29, 263);
            this.CheckButtonInvalidFiles.Name = "CheckButtonInvalidFiles";
            this.CheckButtonInvalidFiles.ScannersToActivate = "20310;20301;20313;";
            this.CheckButtonInvalidFiles.Size = new System.Drawing.Size(149, 27);
            this.CheckButtonInvalidFiles.TabIndex = 19;
            this.CheckButtonInvalidFiles.Text = "  Invalid Files";
            this.CheckButtonInvalidFiles.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.CheckButtonInvalidFiles.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.CheckButtonInvalidFiles.UseVisualStyleBackColor = false;
            // 
            // LabelCleanReg
            // 
            this.LabelCleanReg.AutoSize = true;
            this.LabelCleanReg.BackColor = System.Drawing.Color.Transparent;
            this.LabelCleanReg.Font = new System.Drawing.Font("Arial", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.LabelCleanReg.ForeColor = System.Drawing.SystemColors.ControlDarkDark;
            this.LabelCleanReg.Location = new System.Drawing.Point(29, 421);
            this.LabelCleanReg.Name = "LabelCleanReg";
            this.LabelCleanReg.Size = new System.Drawing.Size(86, 12);
            this.LabelCleanReg.TabIndex = 31;
            this.LabelCleanReg.Text = "Clean The Registry";
            // 
            // CheckButtonAppErrors
            // 
            this.CheckButtonAppErrors.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
            this.CheckButtonAppErrors.BackColor = System.Drawing.Color.White;
            this.CheckButtonAppErrors.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.CheckButtonAppErrors.Cursor = System.Windows.Forms.Cursors.Hand;
            this.CheckButtonAppErrors.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.CheckButtonAppErrors.FlatAppearance.BorderSize = 0;
            this.CheckButtonAppErrors.FlatAppearance.CheckedBackColor = System.Drawing.Color.White;
            this.CheckButtonAppErrors.FlatAppearance.MouseDownBackColor = System.Drawing.Color.White;
            this.CheckButtonAppErrors.FlatAppearance.MouseOverBackColor = System.Drawing.Color.White;
            this.CheckButtonAppErrors.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.CheckButtonAppErrors.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.CheckButtonAppErrors.ForeColor = System.Drawing.Color.Gray;
            this.CheckButtonAppErrors.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.CheckButtonAppErrors.ImageIndex = 1;
            this.CheckButtonAppErrors.ImageList = this.CheckedUncheckedImageList;
            this.CheckButtonAppErrors.Location = new System.Drawing.Point(29, 285);
            this.CheckButtonAppErrors.Name = "CheckButtonAppErrors";
            this.CheckButtonAppErrors.ScannersToActivate = "20320;20308;20306;20312;";
            this.CheckButtonAppErrors.Size = new System.Drawing.Size(156, 27);
            this.CheckButtonAppErrors.TabIndex = 18;
            this.CheckButtonAppErrors.Text = "  Application Errors";
            this.CheckButtonAppErrors.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.CheckButtonAppErrors.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.CheckButtonAppErrors.UseVisualStyleBackColor = false;
            // 
            // CheckButtonWinErrors
            // 
            this.CheckButtonWinErrors.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
            this.CheckButtonWinErrors.BackColor = System.Drawing.Color.White;
            this.CheckButtonWinErrors.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.CheckButtonWinErrors.Cursor = System.Windows.Forms.Cursors.Hand;
            this.CheckButtonWinErrors.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.CheckButtonWinErrors.FlatAppearance.BorderSize = 0;
            this.CheckButtonWinErrors.FlatAppearance.CheckedBackColor = System.Drawing.Color.White;
            this.CheckButtonWinErrors.FlatAppearance.MouseDownBackColor = System.Drawing.Color.White;
            this.CheckButtonWinErrors.FlatAppearance.MouseOverBackColor = System.Drawing.Color.White;
            this.CheckButtonWinErrors.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.CheckButtonWinErrors.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.CheckButtonWinErrors.ForeColor = System.Drawing.Color.Gray;
            this.CheckButtonWinErrors.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.CheckButtonWinErrors.ImageIndex = 1;
            this.CheckButtonWinErrors.ImageList = this.CheckedUncheckedImageList;
            this.CheckButtonWinErrors.Location = new System.Drawing.Point(180, 285);
            this.CheckButtonWinErrors.Name = "CheckButtonWinErrors";
            this.CheckButtonWinErrors.ScannersToActivate = "20307;20316;20402;20309;";
            this.CheckButtonWinErrors.Size = new System.Drawing.Size(145, 27);
            this.CheckButtonWinErrors.TabIndex = 17;
            this.CheckButtonWinErrors.Text = "  Windows Errors";
            this.CheckButtonWinErrors.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.CheckButtonWinErrors.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.CheckButtonWinErrors.UseVisualStyleBackColor = false;
            // 
            // CheckButtonOCXErrors
            // 
            this.CheckButtonOCXErrors.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
            this.CheckButtonOCXErrors.BackColor = System.Drawing.Color.White;
            this.CheckButtonOCXErrors.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.CheckButtonOCXErrors.Cursor = System.Windows.Forms.Cursors.Hand;
            this.CheckButtonOCXErrors.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.CheckButtonOCXErrors.FlatAppearance.BorderSize = 0;
            this.CheckButtonOCXErrors.FlatAppearance.CheckedBackColor = System.Drawing.Color.White;
            this.CheckButtonOCXErrors.FlatAppearance.MouseDownBackColor = System.Drawing.Color.White;
            this.CheckButtonOCXErrors.FlatAppearance.MouseOverBackColor = System.Drawing.Color.White;
            this.CheckButtonOCXErrors.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.CheckButtonOCXErrors.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.CheckButtonOCXErrors.ForeColor = System.Drawing.Color.Gray;
            this.CheckButtonOCXErrors.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.CheckButtonOCXErrors.ImageIndex = 1;
            this.CheckButtonOCXErrors.ImageList = this.CheckedUncheckedImageList;
            this.CheckButtonOCXErrors.Location = new System.Drawing.Point(180, 263);
            this.CheckButtonOCXErrors.Name = "CheckButtonOCXErrors";
            this.CheckButtonOCXErrors.ScannersToActivate = "";
            this.CheckButtonOCXErrors.Size = new System.Drawing.Size(144, 27);
            this.CheckButtonOCXErrors.TabIndex = 16;
            this.CheckButtonOCXErrors.Text = "  Audio Problems";
            this.CheckButtonOCXErrors.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.CheckButtonOCXErrors.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.CheckButtonOCXErrors.UseVisualStyleBackColor = false;
            // 
            // CheckButtonRegErrors
            // 
            this.CheckButtonRegErrors.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
            this.CheckButtonRegErrors.BackColor = System.Drawing.Color.White;
            this.CheckButtonRegErrors.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.CheckButtonRegErrors.Cursor = System.Windows.Forms.Cursors.Hand;
            this.CheckButtonRegErrors.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.CheckButtonRegErrors.FlatAppearance.BorderSize = 0;
            this.CheckButtonRegErrors.FlatAppearance.CheckedBackColor = System.Drawing.Color.White;
            this.CheckButtonRegErrors.FlatAppearance.MouseDownBackColor = System.Drawing.Color.White;
            this.CheckButtonRegErrors.FlatAppearance.MouseOverBackColor = System.Drawing.Color.White;
            this.CheckButtonRegErrors.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.CheckButtonRegErrors.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.CheckButtonRegErrors.ForeColor = System.Drawing.Color.Gray;
            this.CheckButtonRegErrors.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.CheckButtonRegErrors.ImageIndex = 1;
            this.CheckButtonRegErrors.ImageList = this.CheckedUncheckedImageList;
            this.CheckButtonRegErrors.Location = new System.Drawing.Point(29, 240);
            this.CheckButtonRegErrors.Name = "CheckButtonRegErrors";
            this.CheckButtonRegErrors.ScannersToActivate = "20318;20311;20319;";
            this.CheckButtonRegErrors.Size = new System.Drawing.Size(149, 28);
            this.CheckButtonRegErrors.TabIndex = 15;
            this.CheckButtonRegErrors.Text = "  Registry Errors";
            this.CheckButtonRegErrors.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.CheckButtonRegErrors.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.CheckButtonRegErrors.UseVisualStyleBackColor = false;
            // 
            // ButtonStartScanMain
            // 
            this.ButtonStartScanMain.BackColor = System.Drawing.Color.White;
            this.ButtonStartScanMain.BackgroundImage = global::FrontLineGUI.Properties.Resources.StatScanMain_button;
            this.ButtonStartScanMain.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.ButtonStartScanMain.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ButtonStartScanMain.FlatAppearance.BorderSize = 0;
            this.ButtonStartScanMain.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
            this.ButtonStartScanMain.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.ButtonStartScanMain.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonStartScanMain.ForeColor = System.Drawing.Color.White;
            this.ButtonStartScanMain.Location = new System.Drawing.Point(61, 61);
            this.ButtonStartScanMain.Name = "ButtonStartScanMain";
            this.ButtonStartScanMain.Size = new System.Drawing.Size(378, 111);
            this.ButtonStartScanMain.TabIndex = 14;
            this.ButtonStartScanMain.UseVisualStyleBackColor = false;
            this.ButtonStartScanMain.Click += new System.EventHandler(this.ButtonStartScanMain_Click);
            // 
            // ButtonRemoveJunks
            // 
            this.ButtonRemoveJunks.BackColor = System.Drawing.Color.White;
            this.ButtonRemoveJunks.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.ButtonRemoveJunks.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ButtonRemoveJunks.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.ButtonRemoveJunks.FlatAppearance.BorderSize = 0;
            this.ButtonRemoveJunks.FlatAppearance.CheckedBackColor = System.Drawing.Color.White;
            this.ButtonRemoveJunks.FlatAppearance.MouseDownBackColor = System.Drawing.Color.White;
            this.ButtonRemoveJunks.FlatAppearance.MouseOverBackColor = System.Drawing.Color.White;
            this.ButtonRemoveJunks.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonRemoveJunks.ForeColor = System.Drawing.Color.White;
            this.ButtonRemoveJunks.ImageAlign = System.Drawing.ContentAlignment.TopCenter;
            this.ButtonRemoveJunks.ImageIndex = 0;
            this.ButtonRemoveJunks.ImageList = this.RemoveJunksImageList;
            this.ButtonRemoveJunks.Location = new System.Drawing.Point(143, 374);
            this.ButtonRemoveJunks.Name = "ButtonRemoveJunks";
            this.ButtonRemoveJunks.Size = new System.Drawing.Size(91, 38);
            this.ButtonRemoveJunks.TabIndex = 5;
            this.ButtonRemoveJunks.UseVisualStyleBackColor = false;
            this.ButtonRemoveJunks.Click += new System.EventHandler(this.ButtonRemoveJunks_Click);
            // 
            // RemoveJunksImageList
            // 
            this.RemoveJunksImageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("RemoveJunksImageList.ImageStream")));
            this.RemoveJunksImageList.TransparentColor = System.Drawing.Color.White;
            this.RemoveJunksImageList.Images.SetKeyName(0, "RemoveJunks_button.png");
            this.RemoveJunksImageList.Images.SetKeyName(1, "RemoveJunksHover_button.png");
            // 
            // ButtonCleanRegistry
            // 
            this.ButtonCleanRegistry.BackColor = System.Drawing.Color.White;
            this.ButtonCleanRegistry.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.ButtonCleanRegistry.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ButtonCleanRegistry.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.ButtonCleanRegistry.FlatAppearance.BorderSize = 0;
            this.ButtonCleanRegistry.FlatAppearance.CheckedBackColor = System.Drawing.Color.White;
            this.ButtonCleanRegistry.FlatAppearance.MouseDownBackColor = System.Drawing.Color.White;
            this.ButtonCleanRegistry.FlatAppearance.MouseOverBackColor = System.Drawing.Color.White;
            this.ButtonCleanRegistry.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonCleanRegistry.ForeColor = System.Drawing.Color.White;
            this.ButtonCleanRegistry.ImageAlign = System.Drawing.ContentAlignment.TopCenter;
            this.ButtonCleanRegistry.ImageIndex = 0;
            this.ButtonCleanRegistry.ImageList = this.CleanTheRegistryImageList;
            this.ButtonCleanRegistry.Location = new System.Drawing.Point(34, 375);
            this.ButtonCleanRegistry.Name = "ButtonCleanRegistry";
            this.ButtonCleanRegistry.Size = new System.Drawing.Size(81, 37);
            this.ButtonCleanRegistry.TabIndex = 4;
            this.ButtonCleanRegistry.UseVisualStyleBackColor = false;
            this.ButtonCleanRegistry.Click += new System.EventHandler(this.ButtonCleanRegistry_Click);
            // 
            // CleanTheRegistryImageList
            // 
            this.CleanTheRegistryImageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("CleanTheRegistryImageList.ImageStream")));
            this.CleanTheRegistryImageList.TransparentColor = System.Drawing.Color.White;
            this.CleanTheRegistryImageList.Images.SetKeyName(0, "ClenRegistry_button.png");
            this.CleanTheRegistryImageList.Images.SetKeyName(1, "ClenRegistryHover_button.png");
            // 
            // ButtonDefragRegistry
            // 
            this.ButtonDefragRegistry.BackColor = System.Drawing.Color.White;
            this.ButtonDefragRegistry.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.ButtonDefragRegistry.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ButtonDefragRegistry.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.ButtonDefragRegistry.FlatAppearance.BorderSize = 0;
            this.ButtonDefragRegistry.FlatAppearance.CheckedBackColor = System.Drawing.Color.White;
            this.ButtonDefragRegistry.FlatAppearance.MouseDownBackColor = System.Drawing.Color.White;
            this.ButtonDefragRegistry.FlatAppearance.MouseOverBackColor = System.Drawing.Color.White;
            this.ButtonDefragRegistry.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonDefragRegistry.ForeColor = System.Drawing.Color.White;
            this.ButtonDefragRegistry.ImageAlign = System.Drawing.ContentAlignment.TopCenter;
            this.ButtonDefragRegistry.ImageIndex = 0;
            this.ButtonDefragRegistry.ImageList = this.DefragImageList;
            this.ButtonDefragRegistry.Location = new System.Drawing.Point(261, 367);
            this.ButtonDefragRegistry.Name = "ButtonDefragRegistry";
            this.ButtonDefragRegistry.Size = new System.Drawing.Size(96, 45);
            this.ButtonDefragRegistry.TabIndex = 6;
            this.ButtonDefragRegistry.UseVisualStyleBackColor = false;
            this.ButtonDefragRegistry.Click += new System.EventHandler(this.ButtonDefragRegistry_Click);
            // 
            // DefragImageList
            // 
            this.DefragImageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("DefragImageList.ImageStream")));
            this.DefragImageList.TransparentColor = System.Drawing.Color.White;
            this.DefragImageList.Images.SetKeyName(0, "DefragmentRegistry_button.png");
            this.DefragImageList.Images.SetKeyName(1, "DefragmentRegistryHover_button.png");
            // 
            // ButtonEditStartUp
            // 
            this.ButtonEditStartUp.BackColor = System.Drawing.Color.White;
            this.ButtonEditStartUp.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.ButtonEditStartUp.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ButtonEditStartUp.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.ButtonEditStartUp.FlatAppearance.BorderSize = 0;
            this.ButtonEditStartUp.FlatAppearance.CheckedBackColor = System.Drawing.Color.White;
            this.ButtonEditStartUp.FlatAppearance.MouseDownBackColor = System.Drawing.Color.White;
            this.ButtonEditStartUp.FlatAppearance.MouseOverBackColor = System.Drawing.Color.White;
            this.ButtonEditStartUp.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonEditStartUp.ForeColor = System.Drawing.Color.White;
            this.ButtonEditStartUp.ImageAlign = System.Drawing.ContentAlignment.TopCenter;
            this.ButtonEditStartUp.ImageIndex = 0;
            this.ButtonEditStartUp.ImageList = this.EditStartupImageList;
            this.ButtonEditStartUp.Location = new System.Drawing.Point(384, 368);
            this.ButtonEditStartUp.Name = "ButtonEditStartUp";
            this.ButtonEditStartUp.Size = new System.Drawing.Size(61, 44);
            this.ButtonEditStartUp.TabIndex = 7;
            this.ButtonEditStartUp.UseVisualStyleBackColor = false;
            this.ButtonEditStartUp.Click += new System.EventHandler(this.ButtonEditStartUp_Click);
            // 
            // EditStartupImageList
            // 
            this.EditStartupImageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("EditStartupImageList.ImageStream")));
            this.EditStartupImageList.TransparentColor = System.Drawing.Color.White;
            this.EditStartupImageList.Images.SetKeyName(0, "EditStartup_button.png");
            this.EditStartupImageList.Images.SetKeyName(1, "EditStartupHover_button.png");
            // 
            // StartScanPanel
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.panelStartScan);
            this.Name = "StartScanPanel";
            this.Size = new System.Drawing.Size(470, 444);
            this.panelStartScan.ResumeLayout(false);
            this.panelStartScan.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panelStartScan;
        private System.Windows.Forms.Label LabelStartYourScan;
        private System.Windows.Forms.Label labelLastScanPerformed;
        private CustomFrontLineCheckButton CheckButtonRecycleBin;
        private CustomFrontLineCheckButton CheckButtonTempInternet;
        private System.Windows.Forms.Label LabelEditStartUp;
        private CustomFrontLineCheckButton CheckButtonDLLErrors;
        private System.Windows.Forms.Label LabelDefragReg;
        private CustomFrontLineCheckButton CheckButtonTempFiles;
        private System.Windows.Forms.Label LabelRemoveJunks;
        private CustomFrontLineCheckButton CheckButtonInvalidFiles;
        private System.Windows.Forms.Label LabelCleanReg;
        private CustomFrontLineCheckButton CheckButtonAppErrors;
        private CustomFrontLineCheckButton CheckButtonWinErrors;
        private CustomFrontLineCheckButton CheckButtonOCXErrors;
        private CustomFrontLineCheckButton CheckButtonRegErrors;
        private CustomFrontLineButton ButtonStartScanMain;
        private CustomFrontLineButton ButtonRemoveJunks;
        private CustomFrontLineButton ButtonCleanRegistry;
        private CustomFrontLineButton ButtonDefragRegistry;
        private CustomFrontLineButton ButtonEditStartUp;
        private System.Windows.Forms.ImageList CheckedUncheckedImageList;
        private System.Windows.Forms.ImageList RemoveJunksImageList;
        private System.Windows.Forms.ImageList CleanTheRegistryImageList;
        private System.Windows.Forms.ImageList DefragImageList;
        private System.Windows.Forms.ImageList EditStartupImageList;
    }
}
