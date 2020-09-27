namespace FrontLineGUI
{
    partial class StartUpPanel
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(StartUpPanel));
            this.panelStartUp = new System.Windows.Forms.Panel();
            this.ListViewStartup = new System.Windows.Forms.ListView();
            this.ProgramName = new System.Windows.Forms.ColumnHeader();
            this.Status = new System.Windows.Forms.ColumnHeader();
            this.FilePath = new System.Windows.Forms.ColumnHeader();
            this.ImageListDeleteSelected = new System.Windows.Forms.ImageList(this.components);
            this.ImageListEnableSelected = new System.Windows.Forms.ImageList(this.components);
            this.ImageListDisableAll = new System.Windows.Forms.ImageList(this.components);
            this.ImageListEnableAll = new System.Windows.Forms.ImageList(this.components);
            this.ImageListDisableSelected = new System.Windows.Forms.ImageList(this.components);
            this.ButtonDeleteSelectedStartup = new FrontLineGUI.CustomFrontLineButton();
            this.ButtonEnableSelectedStartUp = new FrontLineGUI.CustomFrontLineButton();
            this.ButtonDisableAll = new FrontLineGUI.CustomFrontLineButton();
            this.ButtonEnableAll = new FrontLineGUI.CustomFrontLineButton();
            this.ButtonDisableSelected = new FrontLineGUI.CustomFrontLineButton();
            this.panelStartUp.SuspendLayout();
            this.SuspendLayout();
            // 
            // panelStartUp
            // 
            this.panelStartUp.BackgroundImage = global::FrontLineGUI.Properties.Resources.BackgroundStartUp;
            this.panelStartUp.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.panelStartUp.Controls.Add(this.ListViewStartup);
            this.panelStartUp.Controls.Add(this.ButtonDeleteSelectedStartup);
            this.panelStartUp.Controls.Add(this.ButtonDisableAll);
            this.panelStartUp.Controls.Add(this.ButtonEnableAll);
            this.panelStartUp.Controls.Add(this.ButtonDisableSelected);
            this.panelStartUp.Controls.Add(this.ButtonEnableSelectedStartUp);
            this.panelStartUp.Location = new System.Drawing.Point(0, 0);
            this.panelStartUp.Name = "panelStartUp";
            this.panelStartUp.Size = new System.Drawing.Size(470, 444);
            this.panelStartUp.TabIndex = 2;
            // 
            // ListViewStartup
            // 
            this.ListViewStartup.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.ProgramName,
            this.Status,
            this.FilePath});
            this.ListViewStartup.FullRowSelect = true;
            this.ListViewStartup.Location = new System.Drawing.Point(18, 84);
            this.ListViewStartup.MultiSelect = false;
            this.ListViewStartup.Name = "ListViewStartup";
            this.ListViewStartup.Size = new System.Drawing.Size(439, 285);
            this.ListViewStartup.Sorting = System.Windows.Forms.SortOrder.Ascending;
            this.ListViewStartup.TabIndex = 4;
            this.ListViewStartup.UseCompatibleStateImageBehavior = false;
            this.ListViewStartup.View = System.Windows.Forms.View.Details;
            this.ListViewStartup.ItemSelectionChanged += new System.Windows.Forms.ListViewItemSelectionChangedEventHandler(this.ListViewStartup_ItemSelectionChanged);
            // 
            // ProgramName
            // 
            this.ProgramName.Text = "Program Name";
            this.ProgramName.Width = 124;
            // 
            // Status
            // 
            this.Status.Text = "Status";
            this.Status.Width = 81;
            // 
            // FilePath
            // 
            this.FilePath.Text = "File Path";
            this.FilePath.Width = 227;
            // 
            // ImageListDeleteSelected
            // 
            this.ImageListDeleteSelected.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("ImageListDeleteSelected.ImageStream")));
            this.ImageListDeleteSelected.TransparentColor = System.Drawing.Color.White;
            this.ImageListDeleteSelected.Images.SetKeyName(0, "DeleteSelected_button.png");
            this.ImageListDeleteSelected.Images.SetKeyName(1, "DeleteSelectedHover_button.png");
            // 
            // ImageListEnableSelected
            // 
            this.ImageListEnableSelected.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("ImageListEnableSelected.ImageStream")));
            this.ImageListEnableSelected.TransparentColor = System.Drawing.Color.White;
            this.ImageListEnableSelected.Images.SetKeyName(0, "EnableSelected_button.png");
            this.ImageListEnableSelected.Images.SetKeyName(1, "EnableSelectedHover_button.png");
            // 
            // ImageListDisableAll
            // 
            this.ImageListDisableAll.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("ImageListDisableAll.ImageStream")));
            this.ImageListDisableAll.TransparentColor = System.Drawing.Color.White;
            this.ImageListDisableAll.Images.SetKeyName(0, "DisableAll_button.png");
            this.ImageListDisableAll.Images.SetKeyName(1, "DisableAllHover_button.png");
            // 
            // ImageListEnableAll
            // 
            this.ImageListEnableAll.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("ImageListEnableAll.ImageStream")));
            this.ImageListEnableAll.TransparentColor = System.Drawing.Color.White;
            this.ImageListEnableAll.Images.SetKeyName(0, "EnableAll_button.png");
            this.ImageListEnableAll.Images.SetKeyName(1, "EnableAllHover_button.png");
            // 
            // ImageListDisableSelected
            // 
            this.ImageListDisableSelected.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("ImageListDisableSelected.ImageStream")));
            this.ImageListDisableSelected.TransparentColor = System.Drawing.Color.White;
            this.ImageListDisableSelected.Images.SetKeyName(0, "DisableSelected_button.png");
            this.ImageListDisableSelected.Images.SetKeyName(1, "DisableSelectedHover_button.png");
            // 
            // ButtonDeleteSelectedStartup
            // 
            this.ButtonDeleteSelectedStartup.BackColor = System.Drawing.Color.White;
            this.ButtonDeleteSelectedStartup.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ButtonDeleteSelectedStartup.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.ButtonDeleteSelectedStartup.FlatAppearance.BorderSize = 0;
            this.ButtonDeleteSelectedStartup.FlatAppearance.CheckedBackColor = System.Drawing.Color.White;
            this.ButtonDeleteSelectedStartup.FlatAppearance.MouseDownBackColor = System.Drawing.Color.White;
            this.ButtonDeleteSelectedStartup.FlatAppearance.MouseOverBackColor = System.Drawing.Color.White;
            this.ButtonDeleteSelectedStartup.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonDeleteSelectedStartup.ForeColor = System.Drawing.Color.White;
            this.ButtonDeleteSelectedStartup.ImageIndex = 0;
            this.ButtonDeleteSelectedStartup.ImageList = this.ImageListDeleteSelected;
            this.ButtonDeleteSelectedStartup.Location = new System.Drawing.Point(350, 398);
            this.ButtonDeleteSelectedStartup.Name = "ButtonDeleteSelectedStartup";
            this.ButtonDeleteSelectedStartup.Size = new System.Drawing.Size(106, 39);
            this.ButtonDeleteSelectedStartup.TabIndex = 3;
            this.ButtonDeleteSelectedStartup.UseVisualStyleBackColor = false;
            this.ButtonDeleteSelectedStartup.Click += new System.EventHandler(this.ButtonDeleteSelectedStartup_Click);
            // 
            // ButtonEnableSelectedStartUp
            // 
            this.ButtonEnableSelectedStartUp.BackColor = System.Drawing.Color.White;
            this.ButtonEnableSelectedStartUp.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ButtonEnableSelectedStartUp.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.ButtonEnableSelectedStartUp.FlatAppearance.BorderSize = 0;
            this.ButtonEnableSelectedStartUp.FlatAppearance.CheckedBackColor = System.Drawing.Color.White;
            this.ButtonEnableSelectedStartUp.FlatAppearance.MouseDownBackColor = System.Drawing.Color.White;
            this.ButtonEnableSelectedStartUp.FlatAppearance.MouseOverBackColor = System.Drawing.Color.White;
            this.ButtonEnableSelectedStartUp.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonEnableSelectedStartUp.ForeColor = System.Drawing.Color.White;
            this.ButtonEnableSelectedStartUp.ImageIndex = 0;
            this.ButtonEnableSelectedStartUp.ImageList = this.ImageListEnableSelected;
            this.ButtonEnableSelectedStartUp.Location = new System.Drawing.Point(238, 398);
            this.ButtonEnableSelectedStartUp.Name = "ButtonEnableSelectedStartUp";
            this.ButtonEnableSelectedStartUp.Size = new System.Drawing.Size(106, 39);
            this.ButtonEnableSelectedStartUp.TabIndex = 2;
            this.ButtonEnableSelectedStartUp.UseVisualStyleBackColor = false;
            this.ButtonEnableSelectedStartUp.Click += new System.EventHandler(this.ButtonEnableSelectedStartUp_Click);
            // 
            // ButtonDisableAll
            // 
            this.ButtonDisableAll.BackColor = System.Drawing.Color.White;
            this.ButtonDisableAll.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ButtonDisableAll.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.ButtonDisableAll.FlatAppearance.BorderSize = 0;
            this.ButtonDisableAll.FlatAppearance.CheckedBackColor = System.Drawing.Color.White;
            this.ButtonDisableAll.FlatAppearance.MouseDownBackColor = System.Drawing.Color.White;
            this.ButtonDisableAll.FlatAppearance.MouseOverBackColor = System.Drawing.Color.White;
            this.ButtonDisableAll.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonDisableAll.ForeColor = System.Drawing.Color.White;
            this.ButtonDisableAll.ImageIndex = 0;
            this.ButtonDisableAll.ImageList = this.ImageListDisableAll;
            this.ButtonDisableAll.Location = new System.Drawing.Point(126, 398);
            this.ButtonDisableAll.Name = "ButtonDisableAll";
            this.ButtonDisableAll.Size = new System.Drawing.Size(106, 39);
            this.ButtonDisableAll.TabIndex = 1;
            this.ButtonDisableAll.UseVisualStyleBackColor = false;
            this.ButtonDisableAll.Click += new System.EventHandler(this.ButtonDisableAll_Click);
            // 
            // ButtonEnableAll
            // 
            this.ButtonEnableAll.BackColor = System.Drawing.Color.White;
            this.ButtonEnableAll.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ButtonEnableAll.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.ButtonEnableAll.FlatAppearance.BorderSize = 0;
            this.ButtonEnableAll.FlatAppearance.CheckedBackColor = System.Drawing.Color.White;
            this.ButtonEnableAll.FlatAppearance.MouseDownBackColor = System.Drawing.Color.White;
            this.ButtonEnableAll.FlatAppearance.MouseOverBackColor = System.Drawing.Color.White;
            this.ButtonEnableAll.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonEnableAll.ForeColor = System.Drawing.Color.White;
            this.ButtonEnableAll.ImageIndex = 0;
            this.ButtonEnableAll.ImageList = this.ImageListEnableAll;
            this.ButtonEnableAll.Location = new System.Drawing.Point(14, 398);
            this.ButtonEnableAll.Name = "ButtonEnableAll";
            this.ButtonEnableAll.Size = new System.Drawing.Size(106, 39);
            this.ButtonEnableAll.TabIndex = 0;
            this.ButtonEnableAll.UseVisualStyleBackColor = false;
            this.ButtonEnableAll.Click += new System.EventHandler(this.ButtonEnableAll_Click);
            // 
            // ButtonDisableSelected
            // 
            this.ButtonDisableSelected.BackColor = System.Drawing.Color.White;
            this.ButtonDisableSelected.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ButtonDisableSelected.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.ButtonDisableSelected.FlatAppearance.BorderSize = 0;
            this.ButtonDisableSelected.FlatAppearance.CheckedBackColor = System.Drawing.Color.White;
            this.ButtonDisableSelected.FlatAppearance.MouseDownBackColor = System.Drawing.Color.White;
            this.ButtonDisableSelected.FlatAppearance.MouseOverBackColor = System.Drawing.Color.White;
            this.ButtonDisableSelected.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonDisableSelected.ForeColor = System.Drawing.Color.White;
            this.ButtonDisableSelected.ImageIndex = 0;
            this.ButtonDisableSelected.ImageList = this.ImageListDisableSelected;
            this.ButtonDisableSelected.Location = new System.Drawing.Point(238, 398);
            this.ButtonDisableSelected.Name = "ButtonDisableSelected";
            this.ButtonDisableSelected.Size = new System.Drawing.Size(106, 39);
            this.ButtonDisableSelected.TabIndex = 5;
            this.ButtonDisableSelected.UseVisualStyleBackColor = false;
            this.ButtonDisableSelected.Visible = false;
            this.ButtonDisableSelected.Click += new System.EventHandler(this.ButtonDisableSelected_Click);
            // 
            // StartUpPanel
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.panelStartUp);
            this.Name = "StartUpPanel";
            this.Size = new System.Drawing.Size(470, 444);
            this.panelStartUp.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panelStartUp;
        private System.Windows.Forms.ListView ListViewStartup;
        private System.Windows.Forms.ColumnHeader ProgramName;
        private System.Windows.Forms.ColumnHeader Status;
        private System.Windows.Forms.ColumnHeader FilePath;
        private CustomFrontLineButton ButtonDeleteSelectedStartup;
        private CustomFrontLineButton ButtonEnableSelectedStartUp;
        private CustomFrontLineButton ButtonDisableAll;
        private CustomFrontLineButton ButtonEnableAll;
        private System.Windows.Forms.ImageList ImageListDeleteSelected;
        private System.Windows.Forms.ImageList ImageListDisableAll;
        private System.Windows.Forms.ImageList ImageListEnableSelected;
        private System.Windows.Forms.ImageList ImageListEnableAll;
        private System.Windows.Forms.ImageList ImageListDisableSelected;
        private CustomFrontLineButton ButtonDisableSelected;
    }
}
