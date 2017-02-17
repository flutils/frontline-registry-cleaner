namespace FrontLineGUI
{
    partial class BackUpPanel
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(BackUpPanel));
            this.ImageListDeleteBackUp = new System.Windows.Forms.ImageList(this.components);
            this.ImageListLoadBackup = new System.Windows.Forms.ImageList(this.components);
            this.ImageListBackUpBig = new System.Windows.Forms.ImageList(this.components);
            this.panelBackUp = new System.Windows.Forms.Panel();
            this.ListViewBackUps = new System.Windows.Forms.ListView();
            this.Backups = new System.Windows.Forms.ColumnHeader();
            this.ProgessBackUp = new FrontLineGUI.CustomProgressBar();
            this.ButtonDeleteBackUp = new FrontLineGUI.CustomFrontLineButton();
            this.ButtonLoadBackUp = new FrontLineGUI.CustomFrontLineButton();
            this.ButtonBackUpBackUp = new FrontLineGUI.CustomFrontLineButton();
            this.panelBackUp.SuspendLayout();
            this.SuspendLayout();
            // 
            // ImageListDeleteBackUp
            // 
            this.ImageListDeleteBackUp.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("ImageListDeleteBackUp.ImageStream")));
            this.ImageListDeleteBackUp.TransparentColor = System.Drawing.Color.White;
            this.ImageListDeleteBackUp.Images.SetKeyName(0, "DeletebackUp_button.png");
            this.ImageListDeleteBackUp.Images.SetKeyName(1, "DeletebackUpHover_button.png");
            // 
            // ImageListLoadBackup
            // 
            this.ImageListLoadBackup.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("ImageListLoadBackup.ImageStream")));
            this.ImageListLoadBackup.TransparentColor = System.Drawing.Color.White;
            this.ImageListLoadBackup.Images.SetKeyName(0, "LoadBackUp_button.png");
            this.ImageListLoadBackup.Images.SetKeyName(1, "LoadBackUpHover_button.png");
            // 
            // ImageListBackUpBig
            // 
            this.ImageListBackUpBig.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("ImageListBackUpBig.ImageStream")));
            this.ImageListBackUpBig.TransparentColor = System.Drawing.Color.White;
            this.ImageListBackUpBig.Images.SetKeyName(0, "backUpNow_button.png");
            this.ImageListBackUpBig.Images.SetKeyName(1, "backUpNowHover_button.png");
            // 
            // panelBackUp
            // 
            this.panelBackUp.BackgroundImage = global::FrontLineGUI.Properties.Resources.BackgroundBackUp;
            this.panelBackUp.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.panelBackUp.Controls.Add(this.ProgessBackUp);
            this.panelBackUp.Controls.Add(this.ListViewBackUps);
            this.panelBackUp.Controls.Add(this.ButtonDeleteBackUp);
            this.panelBackUp.Controls.Add(this.ButtonLoadBackUp);
            this.panelBackUp.Controls.Add(this.ButtonBackUpBackUp);
            this.panelBackUp.Location = new System.Drawing.Point(0, 0);
            this.panelBackUp.Name = "panelBackUp";
            this.panelBackUp.Size = new System.Drawing.Size(470, 444);
            this.panelBackUp.TabIndex = 3;
            // 
            // ListViewBackUps
            // 
            this.ListViewBackUps.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.ListViewBackUps.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.Backups});
            this.ListViewBackUps.FullRowSelect = true;
            this.ListViewBackUps.Location = new System.Drawing.Point(17, 166);
            this.ListViewBackUps.MultiSelect = false;
            this.ListViewBackUps.Name = "ListViewBackUps";
            this.ListViewBackUps.Size = new System.Drawing.Size(433, 224);
            this.ListViewBackUps.TabIndex = 4;
            this.ListViewBackUps.UseCompatibleStateImageBehavior = false;
            this.ListViewBackUps.View = System.Windows.Forms.View.Details;
            // 
            // Backups
            // 
            this.Backups.Text = "Backup(s)";
            this.Backups.Width = 430;
            // 
            // ProgessBackUp
            // 
            this.ProgessBackUp.BackColor = System.Drawing.Color.White;
            this.ProgessBackUp.BackgroundDrawMethod = FrontLineGUI.CustomProgressBar.DrawMethod.Stretch;
            this.ProgessBackUp.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.ProgessBackUp.BackgroundLeadingSize = 16;
            this.ProgessBackUp.BackgroundPicture = global::FrontLineGUI.Properties.Resources.ProgressOverlay;
            this.ProgessBackUp.BackgroundTrailingSize = 16;
            this.ProgessBackUp.ForegroundDrawMethod = FrontLineGUI.CustomProgressBar.DrawMethod.Tile;
            this.ProgessBackUp.ForegroundLeadingSize = 4;
            this.ProgessBackUp.ForegroundPicture = global::FrontLineGUI.Properties.Resources.ProgressForeground1;
            this.ProgessBackUp.ForegroundTrailingSize = 4;
            this.ProgessBackUp.Location = new System.Drawing.Point(13, 123);
            this.ProgessBackUp.Marquee = FrontLineGUI.CustomProgressBar.MarqueeStyle.Wave;
            this.ProgessBackUp.MarqueeWidth = 10;
            this.ProgessBackUp.Maximum = 100;
            this.ProgessBackUp.Minimum = 0;
            this.ProgessBackUp.Name = "ProgessBackUp";
            this.ProgessBackUp.OverlayDrawMethod = FrontLineGUI.CustomProgressBar.DrawMethod.Stretch;
            this.ProgessBackUp.OverlayLeadingSize = 2;
            this.ProgessBackUp.OverlayPicture = global::FrontLineGUI.Properties.Resources.ProgressTrueOverlay;
            this.ProgessBackUp.OverlayTrailingSize = 3;
            this.ProgessBackUp.Size = new System.Drawing.Size(445, 26);
            this.ProgessBackUp.TabIndex = 5;
            this.ProgessBackUp.Type = FrontLineGUI.CustomProgressBar.BarType.Progress;
            this.ProgessBackUp.Value = 0;
            // 
            // ButtonDeleteBackUp
            // 
            this.ButtonDeleteBackUp.BackColor = System.Drawing.Color.White;
            this.ButtonDeleteBackUp.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ButtonDeleteBackUp.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.ButtonDeleteBackUp.FlatAppearance.BorderSize = 0;
            this.ButtonDeleteBackUp.FlatAppearance.CheckedBackColor = System.Drawing.Color.White;
            this.ButtonDeleteBackUp.FlatAppearance.MouseDownBackColor = System.Drawing.Color.White;
            this.ButtonDeleteBackUp.FlatAppearance.MouseOverBackColor = System.Drawing.Color.White;
            this.ButtonDeleteBackUp.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonDeleteBackUp.ForeColor = System.Drawing.Color.White;
            this.ButtonDeleteBackUp.ImageIndex = 0;
            this.ButtonDeleteBackUp.ImageList = this.ImageListDeleteBackUp;
            this.ButtonDeleteBackUp.Location = new System.Drawing.Point(331, 398);
            this.ButtonDeleteBackUp.Name = "ButtonDeleteBackUp";
            this.ButtonDeleteBackUp.Size = new System.Drawing.Size(123, 39);
            this.ButtonDeleteBackUp.TabIndex = 3;
            this.ButtonDeleteBackUp.UseVisualStyleBackColor = false;
            this.ButtonDeleteBackUp.Click += new System.EventHandler(this.ButtonDeleteBackUp_Click);
            // 
            // ButtonLoadBackUp
            // 
            this.ButtonLoadBackUp.BackColor = System.Drawing.Color.White;
            this.ButtonLoadBackUp.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ButtonLoadBackUp.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.ButtonLoadBackUp.FlatAppearance.BorderSize = 0;
            this.ButtonLoadBackUp.FlatAppearance.CheckedBackColor = System.Drawing.Color.White;
            this.ButtonLoadBackUp.FlatAppearance.MouseDownBackColor = System.Drawing.Color.White;
            this.ButtonLoadBackUp.FlatAppearance.MouseOverBackColor = System.Drawing.Color.White;
            this.ButtonLoadBackUp.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonLoadBackUp.ForeColor = System.Drawing.Color.White;
            this.ButtonLoadBackUp.ImageIndex = 0;
            this.ButtonLoadBackUp.ImageList = this.ImageListLoadBackup;
            this.ButtonLoadBackUp.Location = new System.Drawing.Point(199, 398);
            this.ButtonLoadBackUp.Name = "ButtonLoadBackUp";
            this.ButtonLoadBackUp.Size = new System.Drawing.Size(123, 39);
            this.ButtonLoadBackUp.TabIndex = 2;
            this.ButtonLoadBackUp.UseVisualStyleBackColor = false;
            this.ButtonLoadBackUp.Click += new System.EventHandler(this.ButtonLoadBackUp_Click);
            // 
            // ButtonBackUpBackUp
            // 
            this.ButtonBackUpBackUp.BackColor = System.Drawing.Color.White;
            this.ButtonBackUpBackUp.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ButtonBackUpBackUp.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.ButtonBackUpBackUp.FlatAppearance.BorderSize = 0;
            this.ButtonBackUpBackUp.FlatAppearance.CheckedBackColor = System.Drawing.Color.White;
            this.ButtonBackUpBackUp.FlatAppearance.MouseDownBackColor = System.Drawing.Color.White;
            this.ButtonBackUpBackUp.FlatAppearance.MouseOverBackColor = System.Drawing.Color.White;
            this.ButtonBackUpBackUp.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonBackUpBackUp.ForeColor = System.Drawing.Color.White;
            this.ButtonBackUpBackUp.ImageIndex = 0;
            this.ButtonBackUpBackUp.ImageList = this.ImageListBackUpBig;
            this.ButtonBackUpBackUp.Location = new System.Drawing.Point(163, 69);
            this.ButtonBackUpBackUp.Name = "ButtonBackUpBackUp";
            this.ButtonBackUpBackUp.Size = new System.Drawing.Size(123, 37);
            this.ButtonBackUpBackUp.TabIndex = 0;
            this.ButtonBackUpBackUp.UseVisualStyleBackColor = false;
            this.ButtonBackUpBackUp.Click += new System.EventHandler(this.ButtonBackUpBackUp_Click);
            // 
            // BackUpPanel
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.panelBackUp);
            this.Name = "BackUpPanel";
            this.Size = new System.Drawing.Size(470, 440);
            this.panelBackUp.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panelBackUp;
        private CustomProgressBar ProgessBackUp;
        private System.Windows.Forms.ListView ListViewBackUps;
        private System.Windows.Forms.ColumnHeader Backups;
        private CustomFrontLineButton ButtonDeleteBackUp;
        private CustomFrontLineButton ButtonLoadBackUp;
        private CustomFrontLineButton ButtonBackUpBackUp;
        private System.Windows.Forms.ImageList ImageListLoadBackup;
        private System.Windows.Forms.ImageList ImageListBackUpBig;
        private System.Windows.Forms.ImageList ImageListDeleteBackUp;
    }
}
