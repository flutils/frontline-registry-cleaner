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
    public enum ItemState
    {
        eNotScanned,
        eScanning,
        eErrorsFound,
        eNoErrors,
        eScanNotPerformed
    }

    public partial class FourStateControl : UserControl
    {
        ItemState _cur_state = ItemState.eNotScanned;
        public ItemState CurrentState
        {
            set
            {
                _cur_state = value;
                switch(value)
                {
                    case ItemState.eNoErrors:
                        CurrentImage.Visible = true;
                        CurrentImage.Image = global::FrontLineGUI.Properties.Resources.Checked;
                        this._lblName.ForeColor = Color.Black;
                        LabelErrorsCount.Visible = false;
                        break;
                    case ItemState.eNotScanned:
                        CurrentImage.Visible = false;
                        this._lblName.ForeColor = Color.Black;
                        LabelErrorsCount.Visible = false;
                        break;
                    case ItemState.eScanning:
                        LabelErrorsCount.Visible = false;
                        CurrentImage.Visible = true;
                        this._lblName.ForeColor = Color.Black;
                        CurrentImage.Image = global::FrontLineGUI.Properties.Resources.loader;
                        break;
                    case ItemState.eErrorsFound:
                        LabelErrorsCount.Visible = true;
                        this._lblName.ForeColor = Color.Red;
                        CurrentImage.Image = global::FrontLineGUI.Properties.Resources.cross123;
                        CurrentImage.Visible = true;
                        break;
                    case ItemState.eScanNotPerformed:
                        CurrentImage.Visible = true;
                        CurrentImage.Image = global::FrontLineGUI.Properties.Resources.minus;
                        this._lblName.ForeColor = Color.Black;
                        LabelErrorsCount.Visible = false;
                        break;
                    default:
                        break;
                }
                this.Invalidate();
            }
            get
            {
                return _cur_state;
            }
        }

        private String _Errors = "";
        public String ErorrsCount
        {
            get
            { return _Errors; }
            set
            {
                if (!IsFileSacanner)
                {
                    _Errors = value;
                    LabelErrorsCount.Text = value;
                    if (!String.IsNullOrEmpty(value) && (value!="0"))
                        this.CurrentState = ItemState.eErrorsFound;
                }
                else
                {
                    //JunksSize = Convert.ToInt32(value);
                    if (JunksSize > 1024.0 * 1024.0 * 102.4)
                    {
                        var gb_size = JunksSize / (1024.0 * 1024.0 * 1024.0);
                        LabelErrorsCount.Text = gb_size.ToString("#.#") + "G";
                    }
                    else if (JunksSize > 1024.0 * 102.4)
                    {
                        var mb_size = JunksSize / (1024.0 * 1024.0);
                        LabelErrorsCount.Text = mb_size.ToString("#.#") + "M";
                    }
                    else if (JunksSize > 102.40)
                    {
                        var kb_size = JunksSize / 1024.0;
                        LabelErrorsCount.Text = kb_size.ToString("#.#") + "K";
                    }
                    if (!String.IsNullOrEmpty(LabelErrorsCount.Text)
                        && (LabelErrorsCount.Text.StartsWith(".") || LabelErrorsCount.Text.StartsWith(",")))
                    {
                        var new_text = LabelErrorsCount.Text.Insert(0, "0");
                        LabelErrorsCount.Text = new_text;
                    }
                    if (!String.IsNullOrEmpty(value) && (value != "0"))
                        this.CurrentState = ItemState.eErrorsFound;
                }
            }
        }

        public FourStateControl()
        {
            InitializeComponent();
            this.BorderStyle = BorderStyle.None;
            this.ForeColor = Color.Black;
            if (DesignMode)
                CurrentState = ItemState.eNotScanned;
            else
                CurrentState = ItemState.eNoErrors;
            LabelErrorsCount.Visible = false;
            CurrentImage.Visible = false;
            JunksSize = 0;
            IsFileSacanner = false;
            Update();

        }

        public String ScannerName
        {
            get { return _lblName.Text; }
            set { _lblName.Text = value; }
        }

        public String ScannerId
        {
            get;
            set;
        }

        public long JunksSize
        {
            get;
            set;
        }

        public bool IsFileSacanner
        {
            get;
            set;
        }

    }
}
