namespace FrontLineGUI
{
    partial class UpdateWindow
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(UpdateWindow));
            this.ImageListDownloadUpdate = new System.Windows.Forms.ImageList(this.components);
            this.pictureBoxUpdate = new System.Windows.Forms.PictureBox();
            this.ButtonDownloadUpdate = new FrontLineGUI.CustomFrontLineButton();
            this.ButtonClose = new FrontLineGUI.CustomFrontLineButton();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxUpdate)).BeginInit();
            this.SuspendLayout();
            // 
            // ImageListDownloadUpdate
            // 
            this.ImageListDownloadUpdate.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("ImageListDownloadUpdate.ImageStream")));
            this.ImageListDownloadUpdate.TransparentColor = System.Drawing.Color.Transparent;
            this.ImageListDownloadUpdate.Images.SetKeyName(0, "DownloadUpdate_button.png");
            this.ImageListDownloadUpdate.Images.SetKeyName(1, "DownloadUpdateHover_button.png");
            // 
            // pictureBoxUpdate
            // 
            this.pictureBoxUpdate.BackColor = System.Drawing.Color.Transparent;
            this.pictureBoxUpdate.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.pictureBoxUpdate.ErrorImage = global::FrontLineGUI.Properties.Resources.update;
            this.pictureBoxUpdate.Image = global::FrontLineGUI.Properties.Resources.update;
            this.pictureBoxUpdate.Location = new System.Drawing.Point(194, 90);
            this.pictureBoxUpdate.Name = "pictureBoxUpdate";
            this.pictureBoxUpdate.Size = new System.Drawing.Size(140, 18);
            this.pictureBoxUpdate.TabIndex = 5;
            this.pictureBoxUpdate.TabStop = false;
            // 
            // ButtonDownloadUpdate
            // 
            this.ButtonDownloadUpdate.BackColor = System.Drawing.Color.Transparent;
            this.ButtonDownloadUpdate.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ButtonDownloadUpdate.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.ButtonDownloadUpdate.FlatAppearance.BorderSize = 0;
            this.ButtonDownloadUpdate.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
            this.ButtonDownloadUpdate.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.ButtonDownloadUpdate.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonDownloadUpdate.ImageIndex = 0;
            this.ButtonDownloadUpdate.ImageList = this.ImageListDownloadUpdate;
            this.ButtonDownloadUpdate.Location = new System.Drawing.Point(193, 114);
            this.ButtonDownloadUpdate.Name = "ButtonDownloadUpdate";
            this.ButtonDownloadUpdate.Size = new System.Drawing.Size(132, 40);
            this.ButtonDownloadUpdate.TabIndex = 4;
            this.ButtonDownloadUpdate.UseVisualStyleBackColor = false;
            this.ButtonDownloadUpdate.Visible = false;
            this.ButtonDownloadUpdate.Click += new System.EventHandler(this.ButtonDownloadUpdate_Click);
            // 
            // ButtonClose
            // 
            this.ButtonClose.BackColor = System.Drawing.Color.Transparent;
            this.ButtonClose.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("ButtonClose.BackgroundImage")));
            this.ButtonClose.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.ButtonClose.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ButtonClose.FlatAppearance.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(64)))), ((int)(((byte)(64)))));
            this.ButtonClose.FlatAppearance.BorderSize = 0;
            this.ButtonClose.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
            this.ButtonClose.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.ButtonClose.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonClose.Location = new System.Drawing.Point(490, 4);
            this.ButtonClose.Name = "ButtonClose";
            this.ButtonClose.Size = new System.Drawing.Size(24, 24);
            this.ButtonClose.TabIndex = 1;
            this.ButtonClose.UseVisualStyleBackColor = false;
            this.ButtonClose.Click += new System.EventHandler(this.ButtonClose_Click);
            // 
            // UpdateWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackgroundImage = global::FrontLineGUI.Properties.Resources.BackgroundUpdate;
            this.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.ClientSize = new System.Drawing.Size(518, 171);
            this.Controls.Add(this.pictureBoxUpdate);
            this.Controls.Add(this.ButtonDownloadUpdate);
            this.Controls.Add(this.ButtonClose);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "UpdateWindow";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "UpdateWindow";
            this.TransparencyKey = System.Drawing.Color.Magenta;
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxUpdate)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ImageList ImageListDownloadUpdate;
        private CustomFrontLineButton ButtonClose;
        private CustomFrontLineButton ButtonDownloadUpdate;
        private System.Windows.Forms.PictureBox pictureBoxUpdate;
    }
}