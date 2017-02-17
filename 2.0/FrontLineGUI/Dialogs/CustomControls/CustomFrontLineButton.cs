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
    public partial class CustomFrontLineButton : Button
    {
        private bool _Hover;
        private bool _Enabled;

        public CustomFrontLineButton()
        {
            _Hover = false;
            _Enabled = true;

            InitializeComponent();
            this.Cursor = Cursors.Hand;
            this.MouseEnter += new EventHandler(CustomFrontLineButton_MouseEnter);
            this.MouseLeave += new EventHandler(CustomFrontLineButton_MouseLeave);
        }

        private void MouseHoverOrNot(bool bHover, object sender)
        {
            var btn = (ButtonBase)sender;
            if (btn != null)
            {
                _Hover = bHover;
                btn.ImageIndex = bHover ? 1 : 0;
                btn.Invalidate();
            }
        }

        void CustomFrontLineButton_MouseEnter(object sender, EventArgs e)
        {
            MouseHoverOrNot(true, sender);
        }

        void CustomFrontLineButton_MouseLeave(object sender, EventArgs e)
        {
            MouseHoverOrNot(false, sender);
        }

        public bool Enabled
        {
            set
            {
                if (_Enabled == value)
                    return;

                _Enabled = value;
                if (!value)
                {
                    this.MouseEnter -= CustomFrontLineButton_MouseEnter;
                    this.MouseLeave -= CustomFrontLineButton_MouseLeave;
                    this.ImageIndex = 2;
                    _Hover = false;
                }
                else
                {
                    this.MouseEnter -= CustomFrontLineButton_MouseEnter;
                    this.MouseLeave -= CustomFrontLineButton_MouseLeave;
                    this.MouseEnter +=  new EventHandler(CustomFrontLineButton_MouseEnter);
                    this.MouseLeave +=  new EventHandler(CustomFrontLineButton_MouseLeave);
                    this.ImageIndex = _Hover ? 1 : 0;
                }
                this.Invalidate();
            }
            get
            {
                return _Enabled;
            }
        }

    }
}
