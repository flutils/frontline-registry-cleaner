using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace FrontLineGUI
{
    public partial class CustomFrontLineCheckButton : Button
    {
        public String ScannersToActivate
        {
            get;
            set;
        }

        public CustomFrontLineCheckButton()
        {
            InitializeComponent();
            //this.ImageList.Images.Clear();
            //this.ImageList.Images.Add(Properties.Resources.Checked);
            //this.ImageList.Images.Add(Properties.Resources.Unchecked);
            this.Click += new EventHandler(CustomFrontLineCheckButton_Click);
            this.MouseEnter += new EventHandler(CustomFrontLineCheckButton_MouseEnter);
            this.MouseLeave += new EventHandler(CustomFrontLineCheckButton_MouseLeave);
        }

        void CustomFrontLineCheckButton_MouseLeave(object sender, EventArgs e)
        {
            var btn = (Button)sender;
            if (btn != null)
            {
                var oldFont = new Font(btn.Font, FontStyle.Regular);
                btn.Font = oldFont;
            }
        }

        void CustomFrontLineCheckButton_MouseEnter(object sender, EventArgs e)
        {
            var btn = (Button)sender;
            if (null != btn)
            {
                var oldFont = new Font(btn.Font, FontStyle.Bold);
                btn.Font = oldFont;
            }
        }

        private void CheckButtonFace(object sender)
        {
            var btn = (Button)sender;
            btn.ImageIndex = (btn.ImageIndex == 0) ? 1 : 0;
        }

        void CustomFrontLineCheckButton_Click(object sender, EventArgs e)
        {
            CheckButtonFace(sender);
            var btn = (CustomFrontLineCheckButton)sender;
            
            if (String.IsNullOrEmpty(btn.ScannersToActivate))
                return;

            foreach (var index in btn.ScannersToActivate.Split(';'))
              {
                  if (String.IsNullOrEmpty(index))
                      continue;
                  CleanEngineClient.Instance().EnableScanner(ScannerHelpers.ScannerAsociation(Convert.ToInt32(index))
                                                             , (btn.ImageIndex == 1) ? true : false);
              }
        }


    }
}
