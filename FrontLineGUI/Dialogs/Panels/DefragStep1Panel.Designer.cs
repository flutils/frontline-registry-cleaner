namespace FrontLineGUI
{
    partial class DefragStep1Panel
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(DefragStep1Panel));
            this.panelDefragStep1 = new System.Windows.Forms.Panel();
            this.ButtonStartAnalysesStep1 = new FrontLineGUI.CustomFrontLineButton();
            this.ImageListStartAnalyses = new System.Windows.Forms.ImageList(this.components);
            this.panelDefragStep1.SuspendLayout();
            this.SuspendLayout();
            // 
            // panelDefragStep1
            // 
            this.panelDefragStep1.BackgroundImage = global::FrontLineGUI.Properties.Resources.BackgroundDefragStep1;
            this.panelDefragStep1.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.panelDefragStep1.Controls.Add(this.ButtonStartAnalysesStep1);
            this.panelDefragStep1.Location = new System.Drawing.Point(0, 0);
            this.panelDefragStep1.Name = "panelDefragStep1";
            this.panelDefragStep1.Size = new System.Drawing.Size(470, 444);
            this.panelDefragStep1.TabIndex = 1;
            // 
            // ButtonStartAnalysesStep1
            // 
            this.ButtonStartAnalysesStep1.BackColor = System.Drawing.Color.White;
            this.ButtonStartAnalysesStep1.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ButtonStartAnalysesStep1.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.ButtonStartAnalysesStep1.FlatAppearance.BorderSize = 0;
            this.ButtonStartAnalysesStep1.FlatAppearance.MouseDownBackColor = System.Drawing.Color.White;
            this.ButtonStartAnalysesStep1.FlatAppearance.MouseOverBackColor = System.Drawing.Color.White;
            this.ButtonStartAnalysesStep1.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonStartAnalysesStep1.ForeColor = System.Drawing.Color.White;
            this.ButtonStartAnalysesStep1.ImageIndex = 0;
            this.ButtonStartAnalysesStep1.ImageList = this.ImageListStartAnalyses;
            this.ButtonStartAnalysesStep1.Location = new System.Drawing.Point(172, 369);
            this.ButtonStartAnalysesStep1.Name = "ButtonStartAnalysesStep1";
            this.ButtonStartAnalysesStep1.Size = new System.Drawing.Size(123, 39);
            this.ButtonStartAnalysesStep1.TabIndex = 0;
            this.ButtonStartAnalysesStep1.UseVisualStyleBackColor = false;
            this.ButtonStartAnalysesStep1.Click += new System.EventHandler(this.ButtonStartAnalysesStep1_Click);
            // 
            // ImageListStartAnalyses
            // 
            this.ImageListStartAnalyses.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("ImageListStartAnalyses.ImageStream")));
            this.ImageListStartAnalyses.TransparentColor = System.Drawing.Color.White;
            this.ImageListStartAnalyses.Images.SetKeyName(0, "StartAnalyses_button.png");
            this.ImageListStartAnalyses.Images.SetKeyName(1, "StartAnalysesHover_button.png");
            // 
            // DefragStep1Panel
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.panelDefragStep1);
            this.Name = "DefragStep1Panel";
            this.Size = new System.Drawing.Size(470, 444);
            this.panelDefragStep1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panelDefragStep1;
        private CustomFrontLineButton ButtonStartAnalysesStep1;
        private System.Windows.Forms.ImageList ImageListStartAnalyses;
    }
}
