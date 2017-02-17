namespace FrontLineGUI
{
    partial class ScanningPanel
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ScanningPanel));
            this.ImageListStop = new System.Windows.Forms.ImageList(this.components);
            this.ImageListPause = new System.Windows.Forms.ImageList(this.components);
            this.ImageListNext = new System.Windows.Forms.ImageList(this.components);
            this.ImageListPrev = new System.Windows.Forms.ImageList(this.components);
            this.panelScanning = new System.Windows.Forms.Panel();
            this.ImageListResume = new System.Windows.Forms.ImageList(this.components);
            this.labelJunkSize = new System.Windows.Forms.Label();
            this.labelItems = new System.Windows.Forms.Label();
            this.ScanningNowLabel = new System.Windows.Forms.Label();
            this.ButtonResume = new FrontLineGUI.CustomFrontLineButton();
            this.FourStateRecents = new FrontLineGUI.FourStateControl();
            this.FourStateIEs = new FrontLineGUI.FourStateControl();
            this.FourStateTemps = new FrontLineGUI.FourStateControl();
            this.FourStateRecycle = new FrontLineGUI.FourStateControl();
            this.FourStateSound = new FrontLineGUI.FourStateControl();
            this.FourStateWinServices = new FrontLineGUI.FourStateControl();
            this.FourStateInvTypeLib = new FrontLineGUI.FourStateControl();
            this.FourStateAssociations = new FrontLineGUI.FourStateControl();
            this.FourStateEmptyRegKeys = new FrontLineGUI.FourStateControl();
            this.FourStateProgStrtUp = new FrontLineGUI.FourStateControl();
            this.FourStatePathRefs = new FrontLineGUI.FourStateControl();
            this.FourStateInvalidTypeLib = new FrontLineGUI.FourStateControl();
            this.FourStateHelpFilesInfo = new FrontLineGUI.FourStateControl();
            this.FourStateAppPaths = new FrontLineGUI.FourStateControl();
            this.FourStateSharedDlls = new FrontLineGUI.FourStateControl();
            this.FourStateFontEntries = new FrontLineGUI.FourStateControl();
            this.FourStateUninstEntries = new FrontLineGUI.FourStateControl();
            this.FourStateActiveX = new FrontLineGUI.FourStateControl();
            this.ButtonStop = new FrontLineGUI.CustomFrontLineButton();
            this.ButtonPause = new FrontLineGUI.CustomFrontLineButton();
            this.ButtonNExtBottom = new FrontLineGUI.CustomFrontLineButton();
            this.ButtonPrevBottom = new FrontLineGUI.CustomFrontLineButton();
            this.ButtonNextTop = new FrontLineGUI.CustomFrontLineButton();
            this.ProgessScan = new FrontLineGUI.CustomProgressBar();
            this.LabelPercents = new System.Windows.Forms.Label();
            this.ButtonPrevTop = new FrontLineGUI.CustomFrontLineButton();
            this.panelScanning.SuspendLayout();
            this.ProgessScan.SuspendLayout();
            this.SuspendLayout();
            // 
            // ImageListStop
            // 
            this.ImageListStop.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("ImageListStop.ImageStream")));
            this.ImageListStop.TransparentColor = System.Drawing.Color.Transparent;
            this.ImageListStop.Images.SetKeyName(0, "Stop_button.png");
            this.ImageListStop.Images.SetKeyName(1, "StopHover_button.png");
            // 
            // ImageListPause
            // 
            this.ImageListPause.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("ImageListPause.ImageStream")));
            this.ImageListPause.TransparentColor = System.Drawing.Color.Transparent;
            this.ImageListPause.Images.SetKeyName(0, "Pause_button.png");
            this.ImageListPause.Images.SetKeyName(1, "PauseHover_button.png");
            // 
            // ImageListNext
            // 
            this.ImageListNext.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("ImageListNext.ImageStream")));
            this.ImageListNext.TransparentColor = System.Drawing.Color.Transparent;
            this.ImageListNext.Images.SetKeyName(0, "Next_button.png");
            this.ImageListNext.Images.SetKeyName(1, "NextHover_button.png");
            // 
            // ImageListPrev
            // 
            this.ImageListPrev.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("ImageListPrev.ImageStream")));
            this.ImageListPrev.TransparentColor = System.Drawing.Color.Transparent;
            this.ImageListPrev.Images.SetKeyName(0, "Prev_button.png");
            this.ImageListPrev.Images.SetKeyName(1, "PrevHover_button.png");
            // 
            // panelScanning
            // 
            this.panelScanning.BackgroundImage = global::FrontLineGUI.Properties.Resources.BackgroundScan;
            this.panelScanning.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.panelScanning.Controls.Add(this.ButtonResume);
            this.panelScanning.Controls.Add(this.FourStateRecents);
            this.panelScanning.Controls.Add(this.FourStateIEs);
            this.panelScanning.Controls.Add(this.FourStateTemps);
            this.panelScanning.Controls.Add(this.FourStateRecycle);
            this.panelScanning.Controls.Add(this.FourStateSound);
            this.panelScanning.Controls.Add(this.FourStateWinServices);
            this.panelScanning.Controls.Add(this.FourStateInvTypeLib);
            this.panelScanning.Controls.Add(this.FourStateAssociations);
            this.panelScanning.Controls.Add(this.FourStateEmptyRegKeys);
            this.panelScanning.Controls.Add(this.FourStateProgStrtUp);
            this.panelScanning.Controls.Add(this.FourStatePathRefs);
            this.panelScanning.Controls.Add(this.FourStateInvalidTypeLib);
            this.panelScanning.Controls.Add(this.FourStateHelpFilesInfo);
            this.panelScanning.Controls.Add(this.FourStateAppPaths);
            this.panelScanning.Controls.Add(this.FourStateSharedDlls);
            this.panelScanning.Controls.Add(this.FourStateFontEntries);
            this.panelScanning.Controls.Add(this.FourStateUninstEntries);
            this.panelScanning.Controls.Add(this.FourStateActiveX);
            this.panelScanning.Controls.Add(this.ButtonStop);
            this.panelScanning.Controls.Add(this.ButtonPause);
            this.panelScanning.Controls.Add(this.ButtonNExtBottom);
            this.panelScanning.Controls.Add(this.ButtonPrevBottom);
            this.panelScanning.Controls.Add(this.ButtonNextTop);
            this.panelScanning.Controls.Add(this.labelJunkSize);
            this.panelScanning.Controls.Add(this.labelItems);
            this.panelScanning.Controls.Add(this.ScanningNowLabel);
            this.panelScanning.Controls.Add(this.ProgessScan);
            this.panelScanning.Controls.Add(this.ButtonPrevTop);
            this.panelScanning.Location = new System.Drawing.Point(0, 0);
            this.panelScanning.Name = "panelScanning";
            this.panelScanning.Size = new System.Drawing.Size(470, 444);
            this.panelScanning.TabIndex = 4;
            // 
            // ImageListResume
            // 
            this.ImageListResume.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("ImageListResume.ImageStream")));
            this.ImageListResume.TransparentColor = System.Drawing.Color.Transparent;
            this.ImageListResume.Images.SetKeyName(0, "Resume_button.png");
            this.ImageListResume.Images.SetKeyName(1, "ResumeHover_button.png");
            // 
            // labelJunkSize
            // 
            this.labelJunkSize.AutoSize = true;
            this.labelJunkSize.BackColor = System.Drawing.Color.Transparent;
            this.labelJunkSize.Font = new System.Drawing.Font("Arial Rounded MT Bold", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelJunkSize.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(157)))), ((int)(((byte)(1)))), ((int)(((byte)(1)))));
            this.labelJunkSize.Location = new System.Drawing.Point(121, 92);
            this.labelJunkSize.Name = "labelJunkSize";
            this.labelJunkSize.Size = new System.Drawing.Size(214, 22);
            this.labelJunkSize.TabIndex = 8;
            this.labelJunkSize.Text = "0MB Junk Files Found!";
            // 
            // labelItems
            // 
            this.labelItems.AutoSize = true;
            this.labelItems.BackColor = System.Drawing.Color.Transparent;
            this.labelItems.Font = new System.Drawing.Font("Arial Rounded MT Bold", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelItems.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(157)))), ((int)(((byte)(1)))), ((int)(((byte)(1)))));
            this.labelItems.Location = new System.Drawing.Point(144, 65);
            this.labelItems.Name = "labelItems";
            this.labelItems.Size = new System.Drawing.Size(151, 22);
            this.labelItems.TabIndex = 7;
            this.labelItems.Text = "0 Errors Found!";
            // 
            // ScanningNowLabel
            // 
            this.ScanningNowLabel.AutoSize = true;
            this.ScanningNowLabel.BackColor = System.Drawing.Color.Transparent;
            this.ScanningNowLabel.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.ScanningNowLabel.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(108)))), ((int)(((byte)(108)))), ((int)(((byte)(108)))));
            this.ScanningNowLabel.Location = new System.Drawing.Point(33, 245);
            this.ScanningNowLabel.Name = "ScanningNowLabel";
            this.ScanningNowLabel.Size = new System.Drawing.Size(151, 16);
            this.ScanningNowLabel.TabIndex = 6;
            this.ScanningNowLabel.Text = "C:\\Program Files\\Tools";
            // 
            // ButtonResume
            // 
            this.ButtonResume.BackColor = System.Drawing.Color.Transparent;
            this.ButtonResume.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ButtonResume.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.ButtonResume.FlatAppearance.BorderSize = 0;
            this.ButtonResume.FlatAppearance.CheckedBackColor = System.Drawing.Color.White;
            this.ButtonResume.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
            this.ButtonResume.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.ButtonResume.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonResume.ForeColor = System.Drawing.Color.Transparent;
            this.ButtonResume.ImageIndex = 0;
            this.ButtonResume.ImageList = this.ImageListResume;
            this.ButtonResume.Location = new System.Drawing.Point(332, 276);
            this.ButtonResume.Name = "ButtonResume";
            this.ButtonResume.Size = new System.Drawing.Size(59, 22);
            this.ButtonResume.TabIndex = 32;
            this.ButtonResume.UseVisualStyleBackColor = false;
            this.ButtonResume.Visible = false;
            this.ButtonResume.Click += new System.EventHandler(this.ButtonResume_Click);
            // 
            // FourStateRecents
            // 
            this.FourStateRecents.BackColor = System.Drawing.Color.Transparent;
            this.FourStateRecents.CurrentState = FrontLineGUI.ItemState.eErrorsFound;
            this.FourStateRecents.ErorrsCount = "0";
            this.FourStateRecents.ForeColor = System.Drawing.Color.Black;
            this.FourStateRecents.IsFileSacanner = false;
            this.FourStateRecents.JunksSize = ((long)(0));
            this.FourStateRecents.Location = new System.Drawing.Point(323, 412);
            this.FourStateRecents.Name = "FourStateRecents";
            this.FourStateRecents.ScannerId = "0";
            this.FourStateRecents.ScannerName = "ScannerName";
            this.FourStateRecents.Size = new System.Drawing.Size(144, 16);
            this.FourStateRecents.TabIndex = 31;
            this.FourStateRecents.Visible = false;
            // 
            // FourStateIEs
            // 
            this.FourStateIEs.BackColor = System.Drawing.Color.Transparent;
            this.FourStateIEs.CurrentState = FrontLineGUI.ItemState.eErrorsFound;
            this.FourStateIEs.ErorrsCount = "0";
            this.FourStateIEs.ForeColor = System.Drawing.Color.Black;
            this.FourStateIEs.IsFileSacanner = false;
            this.FourStateIEs.JunksSize = ((long)(0));
            this.FourStateIEs.Location = new System.Drawing.Point(323, 393);
            this.FourStateIEs.Name = "FourStateIEs";
            this.FourStateIEs.ScannerId = "20407";
            this.FourStateIEs.ScannerName = "Recent Documents";
            this.FourStateIEs.Size = new System.Drawing.Size(144, 16);
            this.FourStateIEs.TabIndex = 30;
            // 
            // FourStateTemps
            // 
            this.FourStateTemps.BackColor = System.Drawing.Color.Transparent;
            this.FourStateTemps.CurrentState = FrontLineGUI.ItemState.eErrorsFound;
            this.FourStateTemps.ErorrsCount = "0";
            this.FourStateTemps.ForeColor = System.Drawing.Color.Black;
            this.FourStateTemps.IsFileSacanner = false;
            this.FourStateTemps.JunksSize = ((long)(0));
            this.FourStateTemps.Location = new System.Drawing.Point(323, 374);
            this.FourStateTemps.Name = "FourStateTemps";
            this.FourStateTemps.ScannerId = "20503;20507";
            this.FourStateTemps.ScannerName = "Temp Installations";
            this.FourStateTemps.Size = new System.Drawing.Size(144, 16);
            this.FourStateTemps.TabIndex = 29;
            // 
            // FourStateRecycle
            // 
            this.FourStateRecycle.BackColor = System.Drawing.Color.Transparent;
            this.FourStateRecycle.CurrentState = FrontLineGUI.ItemState.eErrorsFound;
            this.FourStateRecycle.ErorrsCount = "0";
            this.FourStateRecycle.ForeColor = System.Drawing.Color.Black;
            this.FourStateRecycle.IsFileSacanner = false;
            this.FourStateRecycle.JunksSize = ((long)(0));
            this.FourStateRecycle.Location = new System.Drawing.Point(323, 356);
            this.FourStateRecycle.Name = "FourStateRecycle";
            this.FourStateRecycle.ScannerId = "20506;20406";
            this.FourStateRecycle.ScannerName = "Windows Update";
            this.FourStateRecycle.Size = new System.Drawing.Size(144, 16);
            this.FourStateRecycle.TabIndex = 28;
            // 
            // FourStateSound
            // 
            this.FourStateSound.BackColor = System.Drawing.Color.Transparent;
            this.FourStateSound.CurrentState = FrontLineGUI.ItemState.eErrorsFound;
            this.FourStateSound.ErorrsCount = "0";
            this.FourStateSound.ForeColor = System.Drawing.Color.Black;
            this.FourStateSound.IsFileSacanner = false;
            this.FourStateSound.JunksSize = ((long)(0));
            this.FourStateSound.Location = new System.Drawing.Point(323, 338);
            this.FourStateSound.Name = "FourStateSound";
            this.FourStateSound.ScannerId = "20403;20404;20501";
            this.FourStateSound.ScannerName = "IExplorer cache";
            this.FourStateSound.Size = new System.Drawing.Size(144, 16);
            this.FourStateSound.TabIndex = 27;
            // 
            // FourStateWinServices
            // 
            this.FourStateWinServices.BackColor = System.Drawing.Color.Transparent;
            this.FourStateWinServices.CurrentState = FrontLineGUI.ItemState.eErrorsFound;
            this.FourStateWinServices.ErorrsCount = "0";
            this.FourStateWinServices.ForeColor = System.Drawing.Color.Black;
            this.FourStateWinServices.IsFileSacanner = false;
            this.FourStateWinServices.JunksSize = ((long)(0));
            this.FourStateWinServices.Location = new System.Drawing.Point(323, 320);
            this.FourStateWinServices.Name = "FourStateWinServices";
            this.FourStateWinServices.ScannerId = "20502;20405";
            this.FourStateWinServices.ScannerName = "Recycle Bin";
            this.FourStateWinServices.Size = new System.Drawing.Size(144, 16);
            this.FourStateWinServices.TabIndex = 26;
            // 
            // FourStateInvTypeLib
            // 
            this.FourStateInvTypeLib.BackColor = System.Drawing.Color.Transparent;
            this.FourStateInvTypeLib.CurrentState = FrontLineGUI.ItemState.eErrorsFound;
            this.FourStateInvTypeLib.ErorrsCount = "0";
            this.FourStateInvTypeLib.ForeColor = System.Drawing.Color.Black;
            this.FourStateInvTypeLib.IsFileSacanner = false;
            this.FourStateInvTypeLib.JunksSize = ((long)(0));
            this.FourStateInvTypeLib.Location = new System.Drawing.Point(160, 413);
            this.FourStateInvTypeLib.Name = "FourStateInvTypeLib";
            this.FourStateInvTypeLib.ScannerId = "20505;20504";
            this.FourStateInvTypeLib.ScannerName = "Temp Files";
            this.FourStateInvTypeLib.Size = new System.Drawing.Size(157, 16);
            this.FourStateInvTypeLib.TabIndex = 25;
            // 
            // FourStateAssociations
            // 
            this.FourStateAssociations.BackColor = System.Drawing.Color.Transparent;
            this.FourStateAssociations.CurrentState = FrontLineGUI.ItemState.eErrorsFound;
            this.FourStateAssociations.ErorrsCount = "0";
            this.FourStateAssociations.ForeColor = System.Drawing.Color.Black;
            this.FourStateAssociations.IsFileSacanner = false;
            this.FourStateAssociations.JunksSize = ((long)(0));
            this.FourStateAssociations.Location = new System.Drawing.Point(160, 394);
            this.FourStateAssociations.Name = "FourStateAssociations";
            this.FourStateAssociations.ScannerId = "20313";
            this.FourStateAssociations.ScannerName = "Invalid TypeLib";
            this.FourStateAssociations.Size = new System.Drawing.Size(157, 16);
            this.FourStateAssociations.TabIndex = 24;
            // 
            // FourStateEmptyRegKeys
            // 
            this.FourStateEmptyRegKeys.BackColor = System.Drawing.Color.Transparent;
            this.FourStateEmptyRegKeys.CurrentState = FrontLineGUI.ItemState.eErrorsFound;
            this.FourStateEmptyRegKeys.ErorrsCount = "0";
            this.FourStateEmptyRegKeys.ForeColor = System.Drawing.Color.Black;
            this.FourStateEmptyRegKeys.IsFileSacanner = false;
            this.FourStateEmptyRegKeys.JunksSize = ((long)(0));
            this.FourStateEmptyRegKeys.Location = new System.Drawing.Point(160, 375);
            this.FourStateEmptyRegKeys.Name = "FourStateEmptyRegKeys";
            this.FourStateEmptyRegKeys.ScannerId = "20301";
            this.FourStateEmptyRegKeys.ScannerName = "File Associations";
            this.FourStateEmptyRegKeys.Size = new System.Drawing.Size(157, 16);
            this.FourStateEmptyRegKeys.TabIndex = 23;
            // 
            // FourStateProgStrtUp
            // 
            this.FourStateProgStrtUp.BackColor = System.Drawing.Color.Transparent;
            this.FourStateProgStrtUp.CurrentState = FrontLineGUI.ItemState.eErrorsFound;
            this.FourStateProgStrtUp.ErorrsCount = "0";
            this.FourStateProgStrtUp.ForeColor = System.Drawing.Color.Black;
            this.FourStateProgStrtUp.IsFileSacanner = false;
            this.FourStateProgStrtUp.JunksSize = ((long)(0));
            this.FourStateProgStrtUp.Location = new System.Drawing.Point(160, 356);
            this.FourStateProgStrtUp.Name = "FourStateProgStrtUp";
            this.FourStateProgStrtUp.ScannerId = "20311;20319;20316;20312;20601";
            this.FourStateProgStrtUp.ScannerName = "Empty Registry Keys";
            this.FourStateProgStrtUp.Size = new System.Drawing.Size(157, 16);
            this.FourStateProgStrtUp.TabIndex = 22;
            // 
            // FourStatePathRefs
            // 
            this.FourStatePathRefs.BackColor = System.Drawing.Color.Transparent;
            this.FourStatePathRefs.CurrentState = FrontLineGUI.ItemState.eErrorsFound;
            this.FourStatePathRefs.ErorrsCount = "0";
            this.FourStatePathRefs.ForeColor = System.Drawing.Color.Black;
            this.FourStatePathRefs.IsFileSacanner = false;
            this.FourStatePathRefs.JunksSize = ((long)(0));
            this.FourStatePathRefs.Location = new System.Drawing.Point(160, 339);
            this.FourStatePathRefs.Name = "FourStatePathRefs";
            this.FourStatePathRefs.ScannerId = "20318";
            this.FourStatePathRefs.ScannerName = "Files Paths";
            this.FourStatePathRefs.Size = new System.Drawing.Size(157, 16);
            this.FourStatePathRefs.TabIndex = 21;
            // 
            // FourStateInvalidTypeLib
            // 
            this.FourStateInvalidTypeLib.BackColor = System.Drawing.Color.Transparent;
            this.FourStateInvalidTypeLib.CurrentState = FrontLineGUI.ItemState.eErrorsFound;
            this.FourStateInvalidTypeLib.ErorrsCount = "0";
            this.FourStateInvalidTypeLib.ForeColor = System.Drawing.Color.Black;
            this.FourStateInvalidTypeLib.IsFileSacanner = false;
            this.FourStateInvalidTypeLib.JunksSize = ((long)(0));
            this.FourStateInvalidTypeLib.Location = new System.Drawing.Point(160, 321);
            this.FourStateInvalidTypeLib.Name = "FourStateInvalidTypeLib";
            this.FourStateInvalidTypeLib.ScannerId = "20402;20309";
            this.FourStateInvalidTypeLib.ScannerName = "Windows Startup";
            this.FourStateInvalidTypeLib.Size = new System.Drawing.Size(157, 16);
            this.FourStateInvalidTypeLib.TabIndex = 20;
            // 
            // FourStateHelpFilesInfo
            // 
            this.FourStateHelpFilesInfo.BackColor = System.Drawing.Color.Transparent;
            this.FourStateHelpFilesInfo.CurrentState = FrontLineGUI.ItemState.eErrorsFound;
            this.FourStateHelpFilesInfo.ErorrsCount = "0";
            this.FourStateHelpFilesInfo.ForeColor = System.Drawing.Color.Black;
            this.FourStateHelpFilesInfo.IsFileSacanner = false;
            this.FourStateHelpFilesInfo.JunksSize = ((long)(0));
            this.FourStateHelpFilesInfo.Location = new System.Drawing.Point(6, 412);
            this.FourStateHelpFilesInfo.Name = "FourStateHelpFilesInfo";
            this.FourStateHelpFilesInfo.ScannerId = "20310";
            this.FourStateHelpFilesInfo.ScannerName = "Help Files";
            this.FourStateHelpFilesInfo.Size = new System.Drawing.Size(148, 16);
            this.FourStateHelpFilesInfo.TabIndex = 19;
            // 
            // FourStateAppPaths
            // 
            this.FourStateAppPaths.BackColor = System.Drawing.Color.Transparent;
            this.FourStateAppPaths.CurrentState = FrontLineGUI.ItemState.eErrorsFound;
            this.FourStateAppPaths.ErorrsCount = "0";
            this.FourStateAppPaths.ForeColor = System.Drawing.Color.Black;
            this.FourStateAppPaths.IsFileSacanner = false;
            this.FourStateAppPaths.JunksSize = ((long)(0));
            this.FourStateAppPaths.Location = new System.Drawing.Point(6, 393);
            this.FourStateAppPaths.Name = "FourStateAppPaths";
            this.FourStateAppPaths.ScannerId = "20306";
            this.FourStateAppPaths.ScannerName = "Application Paths";
            this.FourStateAppPaths.Size = new System.Drawing.Size(148, 16);
            this.FourStateAppPaths.TabIndex = 18;
            // 
            // FourStateSharedDlls
            // 
            this.FourStateSharedDlls.BackColor = System.Drawing.Color.Transparent;
            this.FourStateSharedDlls.CurrentState = FrontLineGUI.ItemState.eErrorsFound;
            this.FourStateSharedDlls.ErorrsCount = "0";
            this.FourStateSharedDlls.ForeColor = System.Drawing.Color.Black;
            this.FourStateSharedDlls.IsFileSacanner = false;
            this.FourStateSharedDlls.JunksSize = ((long)(0));
            this.FourStateSharedDlls.Location = new System.Drawing.Point(6, 375);
            this.FourStateSharedDlls.Name = "FourStateSharedDlls";
            this.FourStateSharedDlls.ScannerId = "20315";
            this.FourStateSharedDlls.ScannerName = "Shared Dlls";
            this.FourStateSharedDlls.Size = new System.Drawing.Size(148, 16);
            this.FourStateSharedDlls.TabIndex = 17;
            // 
            // FourStateFontEntries
            // 
            this.FourStateFontEntries.BackColor = System.Drawing.Color.Transparent;
            this.FourStateFontEntries.CurrentState = FrontLineGUI.ItemState.eErrorsFound;
            this.FourStateFontEntries.ErorrsCount = "0";
            this.FourStateFontEntries.ForeColor = System.Drawing.Color.Black;
            this.FourStateFontEntries.IsFileSacanner = false;
            this.FourStateFontEntries.JunksSize = ((long)(0));
            this.FourStateFontEntries.Location = new System.Drawing.Point(6, 356);
            this.FourStateFontEntries.Name = "FourStateFontEntries";
            this.FourStateFontEntries.ScannerId = "20314;20317";
            this.FourStateFontEntries.ScannerName = "IExplorer Add-Ons";
            this.FourStateFontEntries.Size = new System.Drawing.Size(148, 16);
            this.FourStateFontEntries.TabIndex = 16;
            // 
            // FourStateUninstEntries
            // 
            this.FourStateUninstEntries.BackColor = System.Drawing.Color.Transparent;
            this.FourStateUninstEntries.CurrentState = FrontLineGUI.ItemState.eErrorsFound;
            this.FourStateUninstEntries.ErorrsCount = "0";
            this.FourStateUninstEntries.ForeColor = System.Drawing.Color.Black;
            this.FourStateUninstEntries.IsFileSacanner = false;
            this.FourStateUninstEntries.JunksSize = ((long)(0));
            this.FourStateUninstEntries.Location = new System.Drawing.Point(6, 338);
            this.FourStateUninstEntries.Name = "FourStateUninstEntries";
            this.FourStateUninstEntries.ScannerId = "20307";
            this.FourStateUninstEntries.ScannerName = "Uninstall Entries";
            this.FourStateUninstEntries.Size = new System.Drawing.Size(148, 16);
            this.FourStateUninstEntries.TabIndex = 15;
            this.FourStateUninstEntries.Tag = "";
            // 
            // FourStateActiveX
            // 
            this.FourStateActiveX.BackColor = System.Drawing.Color.Transparent;
            this.FourStateActiveX.CurrentState = FrontLineGUI.ItemState.eErrorsFound;
            this.FourStateActiveX.ErorrsCount = "0";
            this.FourStateActiveX.ForeColor = System.Drawing.Color.Black;
            this.FourStateActiveX.IsFileSacanner = false;
            this.FourStateActiveX.JunksSize = ((long)(0));
            this.FourStateActiveX.Location = new System.Drawing.Point(6, 320);
            this.FourStateActiveX.Name = "FourStateActiveX";
            this.FourStateActiveX.ScannerId = "20320;20308";
            this.FourStateActiveX.ScannerName = "ActiveX&&Com Errors";
            this.FourStateActiveX.Size = new System.Drawing.Size(148, 17);
            this.FourStateActiveX.TabIndex = 14;
            this.FourStateActiveX.Tag = "";
            // 
            // ButtonStop
            // 
            this.ButtonStop.BackColor = System.Drawing.Color.Transparent;
            this.ButtonStop.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ButtonStop.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.ButtonStop.FlatAppearance.BorderSize = 0;
            this.ButtonStop.FlatAppearance.CheckedBackColor = System.Drawing.Color.White;
            this.ButtonStop.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
            this.ButtonStop.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.ButtonStop.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonStop.ForeColor = System.Drawing.Color.Transparent;
            this.ButtonStop.ImageIndex = 0;
            this.ButtonStop.ImageList = this.ImageListStop;
            this.ButtonStop.Location = new System.Drawing.Point(395, 276);
            this.ButtonStop.Name = "ButtonStop";
            this.ButtonStop.Size = new System.Drawing.Size(59, 22);
            this.ButtonStop.TabIndex = 13;
            this.ButtonStop.UseVisualStyleBackColor = false;
            this.ButtonStop.Click += new System.EventHandler(this.ButtonStop_Click);
            // 
            // ButtonPause
            // 
            this.ButtonPause.BackColor = System.Drawing.Color.Transparent;
            this.ButtonPause.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ButtonPause.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.ButtonPause.FlatAppearance.BorderSize = 0;
            this.ButtonPause.FlatAppearance.CheckedBackColor = System.Drawing.Color.White;
            this.ButtonPause.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
            this.ButtonPause.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.ButtonPause.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonPause.ForeColor = System.Drawing.Color.Transparent;
            this.ButtonPause.ImageIndex = 0;
            this.ButtonPause.ImageList = this.ImageListPause;
            this.ButtonPause.Location = new System.Drawing.Point(332, 276);
            this.ButtonPause.Name = "ButtonPause";
            this.ButtonPause.Size = new System.Drawing.Size(59, 22);
            this.ButtonPause.TabIndex = 12;
            this.ButtonPause.UseVisualStyleBackColor = false;
            this.ButtonPause.Click += new System.EventHandler(this.ButtonPause_Click);
            // 
            // ButtonNExtBottom
            // 
            this.ButtonNExtBottom.BackColor = System.Drawing.Color.Transparent;
            this.ButtonNExtBottom.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ButtonNExtBottom.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.ButtonNExtBottom.FlatAppearance.BorderSize = 0;
            this.ButtonNExtBottom.FlatAppearance.CheckedBackColor = System.Drawing.Color.White;
            this.ButtonNExtBottom.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
            this.ButtonNExtBottom.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.ButtonNExtBottom.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonNExtBottom.ForeColor = System.Drawing.Color.Transparent;
            this.ButtonNExtBottom.ImageIndex = 0;
            this.ButtonNExtBottom.ImageList = this.ImageListNext;
            this.ButtonNExtBottom.Location = new System.Drawing.Point(395, 276);
            this.ButtonNExtBottom.Name = "ButtonNExtBottom";
            this.ButtonNExtBottom.Size = new System.Drawing.Size(59, 22);
            this.ButtonNExtBottom.TabIndex = 11;
            this.ButtonNExtBottom.UseVisualStyleBackColor = false;
            this.ButtonNExtBottom.Visible = false;
            this.ButtonNExtBottom.Click += new System.EventHandler(this.ButtonNextTop_Click);
            // 
            // ButtonPrevBottom
            // 
            this.ButtonPrevBottom.BackColor = System.Drawing.Color.Transparent;
            this.ButtonPrevBottom.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ButtonPrevBottom.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.ButtonPrevBottom.FlatAppearance.BorderSize = 0;
            this.ButtonPrevBottom.FlatAppearance.CheckedBackColor = System.Drawing.Color.White;
            this.ButtonPrevBottom.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
            this.ButtonPrevBottom.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.ButtonPrevBottom.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonPrevBottom.ForeColor = System.Drawing.Color.Transparent;
            this.ButtonPrevBottom.ImageIndex = 0;
            this.ButtonPrevBottom.ImageList = this.ImageListPrev;
            this.ButtonPrevBottom.Location = new System.Drawing.Point(332, 276);
            this.ButtonPrevBottom.Name = "ButtonPrevBottom";
            this.ButtonPrevBottom.Size = new System.Drawing.Size(59, 22);
            this.ButtonPrevBottom.TabIndex = 10;
            this.ButtonPrevBottom.UseVisualStyleBackColor = false;
            this.ButtonPrevBottom.Visible = false;
            this.ButtonPrevBottom.Click += new System.EventHandler(this.ButtonPrevTop_Click);
            // 
            // ButtonNextTop
            // 
            this.ButtonNextTop.BackColor = System.Drawing.Color.Transparent;
            this.ButtonNextTop.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ButtonNextTop.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.ButtonNextTop.FlatAppearance.BorderSize = 0;
            this.ButtonNextTop.FlatAppearance.CheckedBackColor = System.Drawing.Color.White;
            this.ButtonNextTop.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
            this.ButtonNextTop.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.ButtonNextTop.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonNextTop.ForeColor = System.Drawing.Color.Transparent;
            this.ButtonNextTop.ImageIndex = 0;
            this.ButtonNextTop.ImageList = this.ImageListNext;
            this.ButtonNextTop.Location = new System.Drawing.Point(234, 127);
            this.ButtonNextTop.Name = "ButtonNextTop";
            this.ButtonNextTop.Size = new System.Drawing.Size(59, 22);
            this.ButtonNextTop.TabIndex = 9;
            this.ButtonNextTop.UseVisualStyleBackColor = false;
            this.ButtonNextTop.Visible = false;
            this.ButtonNextTop.Click += new System.EventHandler(this.ButtonNextTop_Click);
            // 
            // ProgessScan
            // 
            this.ProgessScan.BackColor = System.Drawing.Color.White;
            this.ProgessScan.BackgroundDrawMethod = FrontLineGUI.CustomProgressBar.DrawMethod.Stretch;
            this.ProgessScan.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.ProgessScan.BackgroundLeadingSize = 16;
            this.ProgessScan.BackgroundPicture = global::FrontLineGUI.Properties.Resources.ProgressScanningBackground;
            this.ProgessScan.BackgroundTrailingSize = 16;
            this.ProgessScan.Controls.Add(this.LabelPercents);
            this.ProgessScan.ForegroundDrawMethod = FrontLineGUI.CustomProgressBar.DrawMethod.Tile;
            this.ProgessScan.ForegroundLeadingSize = 3;
            this.ProgessScan.ForegroundPicture = global::FrontLineGUI.Properties.Resources.ProgressScanningForeground;
            this.ProgessScan.ForegroundTrailingSize = 3;
            this.ProgessScan.Location = new System.Drawing.Point(13, 172);
            this.ProgessScan.Marquee = FrontLineGUI.CustomProgressBar.MarqueeStyle.Wave;
            this.ProgessScan.MarqueeWidth = 10;
            this.ProgessScan.Maximum = 100;
            this.ProgessScan.Minimum = 0;
            this.ProgessScan.Name = "ProgessScan";
            this.ProgessScan.OverlayDrawMethod = FrontLineGUI.CustomProgressBar.DrawMethod.Stretch;
            this.ProgessScan.OverlayLeadingSize = 2;
            this.ProgessScan.OverlayPicture = global::FrontLineGUI.Properties.Resources.ProgressScanningOverlay;
            this.ProgessScan.OverlayTrailingSize = 3;
            this.ProgessScan.Size = new System.Drawing.Size(443, 38);
            this.ProgessScan.TabIndex = 5;
            this.ProgessScan.Type = FrontLineGUI.CustomProgressBar.BarType.Progress;
            this.ProgessScan.Value = 0;
            // 
            // LabelPercents
            // 
            this.LabelPercents.AutoSize = true;
            this.LabelPercents.BackColor = System.Drawing.Color.Transparent;
            this.LabelPercents.Font = new System.Drawing.Font("Arial Rounded MT Bold", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.LabelPercents.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(9)))), ((int)(((byte)(118)))), ((int)(((byte)(6)))));
            this.LabelPercents.Location = new System.Drawing.Point(198, 6);
            this.LabelPercents.Name = "LabelPercents";
            this.LabelPercents.Size = new System.Drawing.Size(40, 24);
            this.LabelPercents.TabIndex = 0;
            this.LabelPercents.Text = "0%";
            // 
            // ButtonPrevTop
            // 
            this.ButtonPrevTop.BackColor = System.Drawing.Color.Transparent;
            this.ButtonPrevTop.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ButtonPrevTop.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.ButtonPrevTop.FlatAppearance.BorderSize = 0;
            this.ButtonPrevTop.FlatAppearance.CheckedBackColor = System.Drawing.Color.White;
            this.ButtonPrevTop.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
            this.ButtonPrevTop.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.ButtonPrevTop.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonPrevTop.ForeColor = System.Drawing.Color.Transparent;
            this.ButtonPrevTop.ImageIndex = 0;
            this.ButtonPrevTop.ImageList = this.ImageListPrev;
            this.ButtonPrevTop.Location = new System.Drawing.Point(172, 127);
            this.ButtonPrevTop.Name = "ButtonPrevTop";
            this.ButtonPrevTop.Size = new System.Drawing.Size(59, 22);
            this.ButtonPrevTop.TabIndex = 0;
            this.ButtonPrevTop.UseVisualStyleBackColor = false;
            this.ButtonPrevTop.Visible = false;
            this.ButtonPrevTop.Click += new System.EventHandler(this.ButtonPrevTop_Click);
            // 
            // ScanningPanel
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.panelScanning);
            this.Name = "ScanningPanel";
            this.Size = new System.Drawing.Size(470, 444);
            this.panelScanning.ResumeLayout(false);
            this.panelScanning.PerformLayout();
            this.ProgessScan.ResumeLayout(false);
            this.ProgessScan.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panelScanning;
        public CustomProgressBar ProgessScan;
        private CustomFrontLineButton ButtonPrevTop;
        private System.Windows.Forms.Label ScanningNowLabel;
        private System.Windows.Forms.ImageList ImageListPrev;
        private System.Windows.Forms.ImageList ImageListNext;
        private System.Windows.Forms.ImageList ImageListPause;
        private System.Windows.Forms.ImageList ImageListStop;
        public System.Windows.Forms.Label labelJunkSize;
        private System.Windows.Forms.Label labelItems;
        private CustomFrontLineButton ButtonNextTop;
        private CustomFrontLineButton ButtonNExtBottom;
        private CustomFrontLineButton ButtonPrevBottom;
        private CustomFrontLineButton ButtonStop;
        private CustomFrontLineButton ButtonPause;
        public System.Windows.Forms.Label LabelPercents;
        //private FourStateControl ActiveX4State;
        private FourStateControl FourStateActiveX;
        private FourStateControl FourStateUninstEntries;
        private FourStateControl FourStateRecents;
        private FourStateControl FourStateIEs;
        private FourStateControl FourStateTemps;
        private FourStateControl FourStateRecycle;
        private FourStateControl FourStateSound;
        private FourStateControl FourStateWinServices;
        private FourStateControl FourStateInvTypeLib;
        private FourStateControl FourStateAssociations;
        private FourStateControl FourStateEmptyRegKeys;
        private FourStateControl FourStateProgStrtUp;
        private FourStateControl FourStatePathRefs;
        private FourStateControl FourStateInvalidTypeLib;
        private FourStateControl FourStateHelpFilesInfo;
        private FourStateControl FourStateAppPaths;
        private FourStateControl FourStateSharedDlls;
        private FourStateControl FourStateFontEntries;
        private CustomFrontLineButton ButtonResume;
        private System.Windows.Forms.ImageList ImageListResume;
    }
}
