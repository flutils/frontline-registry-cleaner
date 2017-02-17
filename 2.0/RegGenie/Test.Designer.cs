namespace RegGenie
{
    partial class Test
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Test));
            this.button1 = new System.Windows.Forms.Button();
            this.listView1 = new System.Windows.Forms.ListView();
            this.Item = new System.Windows.Forms.ColumnHeader();
            this.Errors = new System.Windows.Forms.ColumnHeader(0);
            this.Cleaned = new System.Windows.Forms.ColumnHeader();
            this.Ignored = new System.Windows.Forms.ColumnHeader();
            this.imageList1 = new System.Windows.Forms.ImageList(this.components);
            this.button2 = new System.Windows.Forms.Button();
            this.button3 = new System.Windows.Forms.Button();
            this.button4 = new System.Windows.Forms.Button();
            this.button5 = new System.Windows.Forms.Button();
            this.panel1 = new RegGenie.CustomProgressBar();
            this.SuspendLayout();
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(691, 12);
            this.button1.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 22);
            this.button1.TabIndex = 0;
            this.button1.Text = "start";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // listView1
            // 
            this.listView1.Activation = System.Windows.Forms.ItemActivation.OneClick;
            this.listView1.AutoArrange = false;
            this.listView1.BackColor = System.Drawing.SystemColors.InactiveCaption;
            this.listView1.BackgroundImageTiled = true;
            this.listView1.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.listView1.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.Item,
            this.Errors,
            this.Cleaned,
            this.Ignored});
            this.listView1.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.listView1.FullRowSelect = true;
            this.listView1.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
            this.listView1.LargeImageList = this.imageList1;
            this.listView1.Location = new System.Drawing.Point(0, 0);
            this.listView1.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.listView1.MultiSelect = false;
            this.listView1.Name = "listView1";
            this.listView1.Size = new System.Drawing.Size(671, 319);
            this.listView1.SmallImageList = this.imageList1;
            this.listView1.TabIndex = 1;
            this.listView1.UseCompatibleStateImageBehavior = false;
            this.listView1.View = System.Windows.Forms.View.Details;
            // 
            // Item
            // 
            this.Item.Text = "Category";
            this.Item.Width = 173;
            // 
            // Errors
            // 
            this.Errors.Text = "Error(s)";
            this.Errors.Width = 138;
            // 
            // Cleaned
            // 
            this.Cleaned.Text = "Cleaned";
            this.Cleaned.Width = 122;
            // 
            // Ignored
            // 
            this.Ignored.Text = "Ignored";
            this.Ignored.Width = 121;
            // 
            // imageList1
            // 
            this.imageList1.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageList1.ImageStream")));
            this.imageList1.TransparentColor = System.Drawing.Color.Transparent;
            this.imageList1.Images.SetKeyName(0, "Close_button.png");
            this.imageList1.Images.SetKeyName(1, "CloseHover_button.png");
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(12, 326);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(75, 23);
            this.button2.TabIndex = 4;
            this.button2.Text = "button2";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(691, 59);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(75, 23);
            this.button3.TabIndex = 5;
            this.button3.Text = "update";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // button4
            // 
            this.button4.Location = new System.Drawing.Point(691, 103);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(75, 23);
            this.button4.TabIndex = 6;
            this.button4.Text = "stop";
            this.button4.UseVisualStyleBackColor = true;
            this.button4.Click += new System.EventHandler(this.button4_Click);
            // 
            // button5
            // 
            this.button5.Location = new System.Drawing.Point(691, 146);
            this.button5.Name = "button5";
            this.button5.Size = new System.Drawing.Size(75, 23);
            this.button5.TabIndex = 7;
            this.button5.Text = "clean 4342";
            this.button5.UseVisualStyleBackColor = true;
            this.button5.Click += new System.EventHandler(this.button5_Click);
            // 
            // panel1
            // 
            this.panel1.BackgroundDrawMethod = RegGenie.CustomProgressBar.DrawMethod.Tile;
            this.panel1.BackgroundLeadingSize = 1;
            this.panel1.BackgroundPicture = global::RegGenie.Properties.Resources.mac_background;
            this.panel1.BackgroundTrailingSize = 1;
            this.panel1.ForegroundDrawMethod = RegGenie.CustomProgressBar.DrawMethod.Tile;
            this.panel1.ForegroundLeadingSize = 4;
            this.panel1.ForegroundPicture = global::RegGenie.Properties.Resources.ProgressForeground;
            this.panel1.ForegroundTrailingSize = 1;
            this.panel1.ImeMode = System.Windows.Forms.ImeMode.On;
            this.panel1.Location = new System.Drawing.Point(168, 326);
            this.panel1.Marquee = RegGenie.CustomProgressBar.MarqueeStyle.Wave;
            this.panel1.MarqueeWidth = 10;
            this.panel1.Maximum = 100;
            this.panel1.Minimum = 0;
            this.panel1.Name = "panel1";
            this.panel1.OverlayDrawMethod = RegGenie.CustomProgressBar.DrawMethod.Stretch;
            this.panel1.OverlayLeadingSize = 14;
            this.panel1.OverlayPicture = global::RegGenie.Properties.Resources.mac_overlay;
            this.panel1.OverlayTrailingSize = 13;
            this.panel1.Size = new System.Drawing.Size(172, 24);
            this.panel1.TabIndex = 3;
            this.panel1.Type = RegGenie.CustomProgressBar.BarType.Progress;
            this.panel1.Value = 50;
            // 
            // Test
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(759, 383);
            this.Controls.Add(this.button5);
            this.Controls.Add(this.button4);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.listView1);
            this.Controls.Add(this.button1);
            this.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.Name = "Test";
            this.Text = "Form1";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Test_FormClosing);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.ListView listView1;
        private System.Windows.Forms.ColumnHeader Item;
        private System.Windows.Forms.ColumnHeader Errors;
        private System.Windows.Forms.ColumnHeader Cleaned;
        private System.Windows.Forms.ColumnHeader Ignored;
        private System.Windows.Forms.ImageList imageList1;
        private CustomProgressBar panel1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Button button4;
        private System.Windows.Forms.Button button5;
    }
}

