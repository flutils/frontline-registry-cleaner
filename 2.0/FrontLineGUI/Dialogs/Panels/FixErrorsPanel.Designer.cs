using FrontLineGUI.Dialogs.CustomControls;
namespace FrontLineGUI
{
    partial class FixErrorsPanel
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FixErrorsPanel));
            this.ImageListNext = new System.Windows.Forms.ImageList(this.components);
            this.ImageListPrev = new System.Windows.Forms.ImageList(this.components);
            this.panelScanning = new System.Windows.Forms.Panel();
            this.labelJunksFixing = new System.Windows.Forms.Label();
            this.labelErrorsFixing = new System.Windows.Forms.Label();
            this.ImageListFixErrors = new System.Windows.Forms.ImageList(this.components);
            this.ImageListIgnoreSelected = new System.Windows.Forms.ImageList(this.components);
            this.labelJunkSize = new System.Windows.Forms.Label();
            this.labelItems = new System.Windows.Forms.Label();
            this.TreeViewErrors = new FrontLineGUI.Dialogs.CustomControls.AntiDoubleClickTreeView();
            this.ButtonFixErrors = new FrontLineGUI.CustomFrontLineButton();
            this.ButtonIgnoreSelected = new FrontLineGUI.CustomFrontLineButton();
            this.ButtonNextTop = new FrontLineGUI.CustomFrontLineButton();
            this.ButtonPrevTop = new FrontLineGUI.CustomFrontLineButton();
            this.panelScanning.SuspendLayout();
            this.SuspendLayout();
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
            this.panelScanning.BackgroundImage = global::FrontLineGUI.Properties.Resources.BackgroundFixing;
            this.panelScanning.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.panelScanning.Controls.Add(this.labelJunksFixing);
            this.panelScanning.Controls.Add(this.labelErrorsFixing);
            this.panelScanning.Controls.Add(this.TreeViewErrors);
            this.panelScanning.Controls.Add(this.ButtonFixErrors);
            this.panelScanning.Controls.Add(this.ButtonIgnoreSelected);
            this.panelScanning.Controls.Add(this.ButtonNextTop);
            this.panelScanning.Controls.Add(this.labelJunkSize);
            this.panelScanning.Controls.Add(this.labelItems);
            this.panelScanning.Controls.Add(this.ButtonPrevTop);
            this.panelScanning.Location = new System.Drawing.Point(0, 0);
            this.panelScanning.Name = "panelScanning";
            this.panelScanning.Size = new System.Drawing.Size(470, 444);
            this.panelScanning.TabIndex = 4;
            // 
            // labelJunksFixing
            // 
            this.labelJunksFixing.AutoSize = true;
            this.labelJunksFixing.BackColor = System.Drawing.Color.Transparent;
            this.labelJunksFixing.Location = new System.Drawing.Point(4, 172);
            this.labelJunksFixing.Name = "labelJunksFixing";
            this.labelJunksFixing.Size = new System.Drawing.Size(168, 13);
            this.labelJunksFixing.TabIndex = 14;
            this.labelJunksFixing.Text = "0 Junk Files Selected for Removal";
            // 
            // labelErrorsFixing
            // 
            this.labelErrorsFixing.AutoSize = true;
            this.labelErrorsFixing.BackColor = System.Drawing.Color.Transparent;
            this.labelErrorsFixing.Location = new System.Drawing.Point(4, 156);
            this.labelErrorsFixing.Name = "labelErrorsFixing";
            this.labelErrorsFixing.Size = new System.Drawing.Size(137, 13);
            this.labelErrorsFixing.TabIndex = 13;
            this.labelErrorsFixing.Text = "0 Errors Selected for Repair";
            // 
            // ImageListFixErrors
            // 
            this.ImageListFixErrors.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("ImageListFixErrors.ImageStream")));
            this.ImageListFixErrors.TransparentColor = System.Drawing.Color.Transparent;
            this.ImageListFixErrors.Images.SetKeyName(0, "FixErrors_button.png");
            this.ImageListFixErrors.Images.SetKeyName(1, "FixErrorsHover_button.png");
            // 
            // ImageListIgnoreSelected
            // 
            this.ImageListIgnoreSelected.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("ImageListIgnoreSelected.ImageStream")));
            this.ImageListIgnoreSelected.TransparentColor = System.Drawing.Color.Transparent;
            this.ImageListIgnoreSelected.Images.SetKeyName(0, "IgnoreKey_button.png");
            this.ImageListIgnoreSelected.Images.SetKeyName(1, "IgnoreKeyHover_button.png");
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
            // TreeViewErrors
            // 
            this.TreeViewErrors.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.TreeViewErrors.CheckBoxes = true;
            this.TreeViewErrors.Location = new System.Drawing.Point(14, 197);
            this.TreeViewErrors.Name = "TreeViewErrors";
            this.TreeViewErrors.Size = new System.Drawing.Size(441, 202);
            this.TreeViewErrors.TabIndex = 12;
            this.TreeViewErrors.AfterCheck += new System.Windows.Forms.TreeViewEventHandler(this.TreeViewErrors_AfterCheck);
            this.TreeViewErrors.DoubleClick += new System.EventHandler(this.TreeViewErrors_DoubleClick);
            this.TreeViewErrors.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.TreeViewErrors_AfterSelect);
            // 
            // ButtonFixErrors
            // 
            this.ButtonFixErrors.BackColor = System.Drawing.Color.White;
            this.ButtonFixErrors.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ButtonFixErrors.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.ButtonFixErrors.FlatAppearance.BorderSize = 0;
            this.ButtonFixErrors.FlatAppearance.CheckedBackColor = System.Drawing.Color.White;
            this.ButtonFixErrors.FlatAppearance.MouseDownBackColor = System.Drawing.Color.White;
            this.ButtonFixErrors.FlatAppearance.MouseOverBackColor = System.Drawing.Color.White;
            this.ButtonFixErrors.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonFixErrors.ForeColor = System.Drawing.Color.White;
            this.ButtonFixErrors.ImageIndex = 0;
            this.ButtonFixErrors.ImageList = this.ImageListFixErrors;
            this.ButtonFixErrors.Location = new System.Drawing.Point(336, 146);
            this.ButtonFixErrors.Name = "ButtonFixErrors";
            this.ButtonFixErrors.Size = new System.Drawing.Size(127, 39);
            this.ButtonFixErrors.TabIndex = 11;
            this.ButtonFixErrors.UseVisualStyleBackColor = false;
            this.ButtonFixErrors.Click += new System.EventHandler(this.ButtonFixErrors_Click);
            // 
            // ButtonIgnoreSelected
            // 
            this.ButtonIgnoreSelected.BackColor = System.Drawing.Color.White;
            this.ButtonIgnoreSelected.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ButtonIgnoreSelected.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.ButtonIgnoreSelected.FlatAppearance.BorderSize = 0;
            this.ButtonIgnoreSelected.FlatAppearance.CheckedBackColor = System.Drawing.Color.White;
            this.ButtonIgnoreSelected.FlatAppearance.MouseDownBackColor = System.Drawing.Color.White;
            this.ButtonIgnoreSelected.FlatAppearance.MouseOverBackColor = System.Drawing.Color.White;
            this.ButtonIgnoreSelected.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonIgnoreSelected.ForeColor = System.Drawing.Color.White;
            this.ButtonIgnoreSelected.ImageIndex = 0;
            this.ButtonIgnoreSelected.ImageList = this.ImageListIgnoreSelected;
            this.ButtonIgnoreSelected.Location = new System.Drawing.Point(210, 146);
            this.ButtonIgnoreSelected.Name = "ButtonIgnoreSelected";
            this.ButtonIgnoreSelected.Size = new System.Drawing.Size(127, 39);
            this.ButtonIgnoreSelected.TabIndex = 10;
            this.ButtonIgnoreSelected.UseVisualStyleBackColor = false;
            this.ButtonIgnoreSelected.Click += new System.EventHandler(this.ButtonIgnoreSelected_Click);
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
            this.ButtonNextTop.Location = new System.Drawing.Point(393, 413);
            this.ButtonNextTop.Name = "ButtonNextTop";
            this.ButtonNextTop.Size = new System.Drawing.Size(59, 22);
            this.ButtonNextTop.TabIndex = 9;
            this.ButtonNextTop.UseVisualStyleBackColor = false;
            this.ButtonNextTop.Click += new System.EventHandler(this.ButtonNextTop_Click);
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
            this.ButtonPrevTop.Location = new System.Drawing.Point(12, 413);
            this.ButtonPrevTop.Name = "ButtonPrevTop";
            this.ButtonPrevTop.Size = new System.Drawing.Size(59, 22);
            this.ButtonPrevTop.TabIndex = 0;
            this.ButtonPrevTop.UseVisualStyleBackColor = false;
            this.ButtonPrevTop.Click += new System.EventHandler(this.ButtonPrevTop_Click);
            // 
            // FixErrorsPanel
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.panelScanning);
            this.Name = "FixErrorsPanel";
            this.Size = new System.Drawing.Size(470, 444);
            this.panelScanning.ResumeLayout(false);
            this.panelScanning.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panelScanning;
        private CustomFrontLineButton ButtonPrevTop;
        private System.Windows.Forms.ImageList ImageListPrev;
        private System.Windows.Forms.ImageList ImageListNext;
        public System.Windows.Forms.Label labelJunkSize;
        private System.Windows.Forms.Label labelItems;
        private CustomFrontLineButton ButtonNextTop;
        private CustomFrontLineButton ButtonIgnoreSelected;
        private System.Windows.Forms.ImageList ImageListIgnoreSelected;
        private CustomFrontLineButton ButtonFixErrors;
        private System.Windows.Forms.ImageList ImageListFixErrors;
        private AntiDoubleClickTreeView TreeViewErrors;
        private System.Windows.Forms.Label labelErrorsFixing;
        private System.Windows.Forms.Label labelJunksFixing;
        //private FourStateControl ActiveX4State;
    }
}
