namespace FrontLineGUI
{
    partial class DefragStep2Panel
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(DefragStep2Panel));
            this.panelDefragStep2 = new System.Windows.Forms.Panel();
            this.ButtonCompleteDefrag = new FrontLineGUI.CustomFrontLineButton();
            this.ImageListCompleteDefrag = new System.Windows.Forms.ImageList(this.components);
            this.ButtonCancelDefragStep2 = new FrontLineGUI.CustomFrontLineButton();
            this.ImageListCancel = new System.Windows.Forms.ImageList(this.components);
            this.panelDefragStep2.SuspendLayout();
            this.SuspendLayout();
            // 
            // panelDefragStep2
            // 
            this.panelDefragStep2.BackgroundImage = global::FrontLineGUI.Properties.Resources.BackgroundDefragStep2;
            this.panelDefragStep2.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.panelDefragStep2.Controls.Add(this.ButtonCompleteDefrag);
            this.panelDefragStep2.Controls.Add(this.ButtonCancelDefragStep2);
            this.panelDefragStep2.Location = new System.Drawing.Point(0, 0);
            this.panelDefragStep2.Name = "panelDefragStep2";
            this.panelDefragStep2.Size = new System.Drawing.Size(470, 444);
            this.panelDefragStep2.TabIndex = 1;
            // 
            // ButtonCompleteDefrag
            // 
            this.ButtonCompleteDefrag.BackColor = System.Drawing.Color.White;
            this.ButtonCompleteDefrag.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ButtonCompleteDefrag.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.ButtonCompleteDefrag.FlatAppearance.BorderSize = 0;
            this.ButtonCompleteDefrag.FlatAppearance.CheckedBackColor = System.Drawing.Color.White;
            this.ButtonCompleteDefrag.FlatAppearance.MouseDownBackColor = System.Drawing.Color.White;
            this.ButtonCompleteDefrag.FlatAppearance.MouseOverBackColor = System.Drawing.Color.White;
            this.ButtonCompleteDefrag.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonCompleteDefrag.ForeColor = System.Drawing.Color.White;
            this.ButtonCompleteDefrag.ImageIndex = 0;
            this.ButtonCompleteDefrag.ImageList = this.ImageListCompleteDefrag;
            this.ButtonCompleteDefrag.Location = new System.Drawing.Point(337, 396);
            this.ButtonCompleteDefrag.Name = "ButtonCompleteDefrag";
            this.ButtonCompleteDefrag.Size = new System.Drawing.Size(123, 39);
            this.ButtonCompleteDefrag.TabIndex = 2;
            this.ButtonCompleteDefrag.UseVisualStyleBackColor = false;
            this.ButtonCompleteDefrag.Click += new System.EventHandler(this.ButtonCompleteDefrag_Click);
            // 
            // ImageListCompleteDefrag
            // 
            this.ImageListCompleteDefrag.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("ImageListCompleteDefrag.ImageStream")));
            this.ImageListCompleteDefrag.TransparentColor = System.Drawing.Color.White;
            this.ImageListCompleteDefrag.Images.SetKeyName(0, "CompleteDefrag_button.png");
            this.ImageListCompleteDefrag.Images.SetKeyName(1, "CompleteDefragHover_button.png");
            // 
            // ButtonCancelDefragStep2
            // 
            this.ButtonCancelDefragStep2.BackColor = System.Drawing.Color.White;
            this.ButtonCancelDefragStep2.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ButtonCancelDefragStep2.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.ButtonCancelDefragStep2.FlatAppearance.BorderSize = 0;
            this.ButtonCancelDefragStep2.FlatAppearance.CheckedBackColor = System.Drawing.Color.White;
            this.ButtonCancelDefragStep2.FlatAppearance.MouseDownBackColor = System.Drawing.Color.White;
            this.ButtonCancelDefragStep2.FlatAppearance.MouseOverBackColor = System.Drawing.Color.White;
            this.ButtonCancelDefragStep2.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonCancelDefragStep2.ForeColor = System.Drawing.Color.White;
            this.ButtonCancelDefragStep2.ImageIndex = 0;
            this.ButtonCancelDefragStep2.ImageList = this.ImageListCancel;
            this.ButtonCancelDefragStep2.Location = new System.Drawing.Point(14, 396);
            this.ButtonCancelDefragStep2.Name = "ButtonCancelDefragStep2";
            this.ButtonCancelDefragStep2.Size = new System.Drawing.Size(123, 39);
            this.ButtonCancelDefragStep2.TabIndex = 1;
            this.ButtonCancelDefragStep2.UseVisualStyleBackColor = false;
            this.ButtonCancelDefragStep2.Click += new System.EventHandler(this.ButtonCancelDefragStep2_Click);
            // 
            // ImageListCancel
            // 
            this.ImageListCancel.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("ImageListCancel.ImageStream")));
            this.ImageListCancel.TransparentColor = System.Drawing.Color.White;
            this.ImageListCancel.Images.SetKeyName(0, "Cancel_button.png");
            this.ImageListCancel.Images.SetKeyName(1, "CancelHover_button.png");
            // 
            // DefragStep2Panel
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.panelDefragStep2);
            this.Name = "DefragStep2Panel";
            this.Size = new System.Drawing.Size(470, 444);
            this.panelDefragStep2.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panelDefragStep2;
        private CustomFrontLineButton ButtonCompleteDefrag;
        private CustomFrontLineButton ButtonCancelDefragStep2;
        private System.Windows.Forms.ImageList ImageListCancel;
        private System.Windows.Forms.ImageList ImageListCompleteDefrag;
    }
}
