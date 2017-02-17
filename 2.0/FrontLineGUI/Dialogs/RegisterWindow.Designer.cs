namespace FrontLineGUI
{
    partial class RegisterWindow
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(RegisterWindow));
            this.ButtonClose = new System.Windows.Forms.Button();
            this.TextBoxEmail = new System.Windows.Forms.TextBox();
            this.TextBoxLicense = new System.Windows.Forms.TextBox();
            this.ImageListClickForKey = new System.Windows.Forms.ImageList(this.components);
            this.ButtonClickForKey = new System.Windows.Forms.Button();
            this.linkLabel1 = new System.Windows.Forms.LinkLabel();
            this.ImageListDontHave = new System.Windows.Forms.ImageList(this.components);
            this.ImageListSubmit = new System.Windows.Forms.ImageList(this.components);
            this.ButtonDontHave = new System.Windows.Forms.Button();
            this.ButtonSubmit = new System.Windows.Forms.Button();
            this.LinkDontHave = new System.Windows.Forms.LinkLabel();
            this.SuspendLayout();
            // 
            // ButtonClose
            // 
            this.ButtonClose.BackColor = System.Drawing.Color.Transparent;
            this.ButtonClose.BackgroundImage = global::FrontLineGUI.Properties.Resources.CloseRegister_button;
            this.ButtonClose.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.ButtonClose.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ButtonClose.FlatAppearance.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(64)))), ((int)(((byte)(64)))));
            this.ButtonClose.FlatAppearance.BorderSize = 0;
            this.ButtonClose.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
            this.ButtonClose.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.ButtonClose.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonClose.Location = new System.Drawing.Point(556, 5);
            this.ButtonClose.Name = "ButtonClose";
            this.ButtonClose.Size = new System.Drawing.Size(24, 24);
            this.ButtonClose.TabIndex = 0;
            this.ButtonClose.UseVisualStyleBackColor = false;
            this.ButtonClose.Click += new System.EventHandler(this.button1_Click);
            // 
            // TextBoxEmail
            // 
            this.TextBoxEmail.BackColor = System.Drawing.Color.White;
            this.TextBoxEmail.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.TextBoxEmail.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.TextBoxEmail.Location = new System.Drawing.Point(267, 148);
            this.TextBoxEmail.Name = "TextBoxEmail";
            this.TextBoxEmail.Size = new System.Drawing.Size(275, 29);
            this.TextBoxEmail.TabIndex = 1;
            // 
            // TextBoxLicense
            // 
            this.TextBoxLicense.BackColor = System.Drawing.Color.White;
            this.TextBoxLicense.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.TextBoxLicense.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.TextBoxLicense.Location = new System.Drawing.Point(267, 189);
            this.TextBoxLicense.Name = "TextBoxLicense";
            this.TextBoxLicense.Size = new System.Drawing.Size(275, 29);
            this.TextBoxLicense.TabIndex = 2;
            // 
            // ImageListClickForKey
            // 
            this.ImageListClickForKey.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("ImageListClickForKey.ImageStream")));
            this.ImageListClickForKey.TransparentColor = System.Drawing.Color.Transparent;
            this.ImageListClickForKey.Images.SetKeyName(0, "ClickForKey_button.png");
            this.ImageListClickForKey.Images.SetKeyName(1, "ClickForKeyHover_button.png");
            // 
            // ButtonClickForKey
            // 
            this.ButtonClickForKey.BackColor = System.Drawing.Color.Transparent;
            this.ButtonClickForKey.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ButtonClickForKey.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.ButtonClickForKey.FlatAppearance.BorderSize = 0;
            this.ButtonClickForKey.FlatAppearance.CheckedBackColor = System.Drawing.Color.Transparent;
            this.ButtonClickForKey.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
            this.ButtonClickForKey.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.ButtonClickForKey.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonClickForKey.ImageIndex = 0;
            this.ButtonClickForKey.ImageList = this.ImageListClickForKey;
            this.ButtonClickForKey.Location = new System.Drawing.Point(69, 344);
            this.ButtonClickForKey.Name = "ButtonClickForKey";
            this.ButtonClickForKey.Size = new System.Drawing.Size(132, 40);
            this.ButtonClickForKey.TabIndex = 3;
            this.ButtonClickForKey.UseVisualStyleBackColor = false;
            this.ButtonClickForKey.MouseLeave += new System.EventHandler(this.ButtonClickForKey_MouseLeave);
            this.ButtonClickForKey.Click += new System.EventHandler(this.ButtonClickForKey_Click);
            this.ButtonClickForKey.MouseEnter += new System.EventHandler(this.ButtonClickForKey_MouseEnter);
            // 
            // linkLabel1
            // 
            this.linkLabel1.ActiveLinkColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(162)))), ((int)(((byte)(255)))));
            this.linkLabel1.AutoSize = true;
            this.linkLabel1.BackColor = System.Drawing.Color.Transparent;
            this.linkLabel1.Cursor = System.Windows.Forms.Cursors.Hand;
            this.linkLabel1.Font = new System.Drawing.Font("Arial Rounded MT Bold", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.linkLabel1.LinkBehavior = System.Windows.Forms.LinkBehavior.NeverUnderline;
            this.linkLabel1.LinkColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(162)))), ((int)(((byte)(255)))));
            this.linkLabel1.Location = new System.Drawing.Point(27, 399);
            this.linkLabel1.Name = "linkLabel1";
            this.linkLabel1.Size = new System.Drawing.Size(222, 15);
            this.linkLabel1.TabIndex = 4;
            this.linkLabel1.TabStop = true;
            this.linkLabel1.Text = "Click Here For A License Key";
            this.linkLabel1.VisitedLinkColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(162)))), ((int)(((byte)(255)))));
            this.linkLabel1.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.linkLabel1_LinkClicked);
            // 
            // ImageListDontHave
            // 
            this.ImageListDontHave.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("ImageListDontHave.ImageStream")));
            this.ImageListDontHave.TransparentColor = System.Drawing.Color.Transparent;
            this.ImageListDontHave.Images.SetKeyName(0, "DontHave_button.png");
            this.ImageListDontHave.Images.SetKeyName(1, "DontHaveHover_button.png");
            // 
            // ImageListSubmit
            // 
            this.ImageListSubmit.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("ImageListSubmit.ImageStream")));
            this.ImageListSubmit.TransparentColor = System.Drawing.Color.Transparent;
            this.ImageListSubmit.Images.SetKeyName(0, "SubmitLicense_button.png");
            this.ImageListSubmit.Images.SetKeyName(1, "SubmitLicenseHover_button.png");
            // 
            // ButtonDontHave
            // 
            this.ButtonDontHave.BackColor = System.Drawing.Color.Transparent;
            this.ButtonDontHave.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ButtonDontHave.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.ButtonDontHave.FlatAppearance.BorderSize = 0;
            this.ButtonDontHave.FlatAppearance.CheckedBackColor = System.Drawing.Color.Transparent;
            this.ButtonDontHave.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
            this.ButtonDontHave.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.ButtonDontHave.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonDontHave.ImageIndex = 0;
            this.ButtonDontHave.ImageList = this.ImageListDontHave;
            this.ButtonDontHave.Location = new System.Drawing.Point(267, 233);
            this.ButtonDontHave.Name = "ButtonDontHave";
            this.ButtonDontHave.Size = new System.Drawing.Size(107, 33);
            this.ButtonDontHave.TabIndex = 5;
            this.ButtonDontHave.UseVisualStyleBackColor = false;
            this.ButtonDontHave.MouseLeave += new System.EventHandler(this.ButtonSubmit_MouseLeave);
            this.ButtonDontHave.Click += new System.EventHandler(this.ButtonDontHave_Click);
            this.ButtonDontHave.MouseEnter += new System.EventHandler(this.button2_MouseEnter);
            // 
            // ButtonSubmit
            // 
            this.ButtonSubmit.BackColor = System.Drawing.Color.Transparent;
            this.ButtonSubmit.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ButtonSubmit.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.ButtonSubmit.FlatAppearance.BorderSize = 0;
            this.ButtonSubmit.FlatAppearance.CheckedBackColor = System.Drawing.Color.Transparent;
            this.ButtonSubmit.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
            this.ButtonSubmit.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.ButtonSubmit.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonSubmit.ImageIndex = 0;
            this.ButtonSubmit.ImageList = this.ImageListSubmit;
            this.ButtonSubmit.Location = new System.Drawing.Point(410, 233);
            this.ButtonSubmit.Name = "ButtonSubmit";
            this.ButtonSubmit.Size = new System.Drawing.Size(107, 33);
            this.ButtonSubmit.TabIndex = 6;
            this.ButtonSubmit.UseVisualStyleBackColor = false;
            this.ButtonSubmit.MouseLeave += new System.EventHandler(this.ButtonSubmit_MouseLeave);
            this.ButtonSubmit.Click += new System.EventHandler(this.ButtonSubmit_Click);
            this.ButtonSubmit.MouseEnter += new System.EventHandler(this.button2_MouseEnter);
            // 
            // LinkDontHave
            // 
            this.LinkDontHave.ActiveLinkColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(162)))), ((int)(((byte)(255)))));
            this.LinkDontHave.AutoSize = true;
            this.LinkDontHave.BackColor = System.Drawing.Color.Transparent;
            this.LinkDontHave.Cursor = System.Windows.Forms.Cursors.Hand;
            this.LinkDontHave.Font = new System.Drawing.Font("Arial Rounded MT Bold", 7F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.LinkDontHave.LinkBehavior = System.Windows.Forms.LinkBehavior.NeverUnderline;
            this.LinkDontHave.LinkColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(162)))), ((int)(((byte)(255)))));
            this.LinkDontHave.Location = new System.Drawing.Point(315, 123);
            this.LinkDontHave.Name = "LinkDontHave";
            this.LinkDontHave.Size = new System.Drawing.Size(161, 11);
            this.LinkDontHave.TabIndex = 7;
            this.LinkDontHave.TabStop = true;
            this.LinkDontHave.Text = "Don\'t Have A License Key?";
            this.LinkDontHave.VisitedLinkColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(162)))), ((int)(((byte)(255)))));
            this.LinkDontHave.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.LinkDontHave_LinkClicked);
            // 
            // RegisterWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.Magenta;
            this.BackgroundImage = global::FrontLineGUI.Properties.Resources.Background_RegisterWnd;
            this.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.ClientSize = new System.Drawing.Size(583, 445);
            this.ControlBox = false;
            this.Controls.Add(this.LinkDontHave);
            this.Controls.Add(this.ButtonSubmit);
            this.Controls.Add(this.ButtonDontHave);
            this.Controls.Add(this.linkLabel1);
            this.Controls.Add(this.ButtonClickForKey);
            this.Controls.Add(this.TextBoxLicense);
            this.Controls.Add(this.TextBoxEmail);
            this.Controls.Add(this.ButtonClose);
            this.DoubleBuffered = true;
            this.ForeColor = System.Drawing.Color.Transparent;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "RegisterWindow";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "StatisticsWindow";
            this.TransparencyKey = System.Drawing.Color.Magenta;
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button ButtonClose;
        private System.Windows.Forms.TextBox TextBoxEmail;
        private System.Windows.Forms.TextBox TextBoxLicense;
        private System.Windows.Forms.ImageList ImageListClickForKey;
        private System.Windows.Forms.Button ButtonClickForKey;
        private System.Windows.Forms.LinkLabel linkLabel1;
        private System.Windows.Forms.ImageList ImageListDontHave;
        private System.Windows.Forms.ImageList ImageListSubmit;
        private System.Windows.Forms.Button ButtonDontHave;
        private System.Windows.Forms.Button ButtonSubmit;
        private System.Windows.Forms.LinkLabel LinkDontHave;

    }
}