namespace FrontLineGUI
{
    partial class FourStateControl
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
            this.LabelErrorsCount = new System.Windows.Forms.Label();
            this._lblName = new System.Windows.Forms.Label();
            this.CurrentImage = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.CurrentImage)).BeginInit();
            this.SuspendLayout();
            // 
            // LabelErrorsCount
            // 
            this.LabelErrorsCount.AutoSize = true;
            this.LabelErrorsCount.BackColor = System.Drawing.Color.Transparent;
            this.LabelErrorsCount.Location = new System.Drawing.Point(-3, 2);
            this.LabelErrorsCount.Name = "LabelErrorsCount";
            this.LabelErrorsCount.Size = new System.Drawing.Size(25, 13);
            this.LabelErrorsCount.TabIndex = 1;
            this.LabelErrorsCount.Text = "123";
            this.LabelErrorsCount.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // _lblName
            // 
            this._lblName.AutoSize = true;
            this._lblName.Location = new System.Drawing.Point(48, 2);
            this._lblName.Name = "_lblName";
            this._lblName.Size = new System.Drawing.Size(75, 13);
            this._lblName.TabIndex = 2;
            this._lblName.Text = "ScannerName";
            // 
            // CurrentImage
            // 
            this.CurrentImage.Image = global::FrontLineGUI.Properties.Resources.cross123;
            this.CurrentImage.Location = new System.Drawing.Point(33, 1);
            this.CurrentImage.Name = "CurrentImage";
            this.CurrentImage.Size = new System.Drawing.Size(16, 16);
            this.CurrentImage.TabIndex = 0;
            this.CurrentImage.TabStop = false;
            // 
            // FourStateControl
            // 
            this.BackColor = System.Drawing.Color.Transparent;
            this.Controls.Add(this._lblName);
            this.Controls.Add(this.LabelErrorsCount);
            this.Controls.Add(this.CurrentImage);
            this.Name = "FourStateControl";
            this.Size = new System.Drawing.Size(177, 20);
            ((System.ComponentModel.ISupportInitialize)(this.CurrentImage)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox CurrentImage;
        private System.Windows.Forms.Label LabelErrorsCount;
        private System.Windows.Forms.Label _lblName;
    }
}
