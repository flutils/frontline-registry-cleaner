using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Text.RegularExpressions;
using System.IO;
using System.Runtime.InteropServices;

namespace FrontLineGUI
{
    public partial class FixErrorsPanel : UserControl
    {
        public WizzardControl Wizzard
        {
            get;
            private set;
        }
       
        private FixErrorsPanel() { }

        public FixErrorsPanel(WizzardControl iWizzard)
        {
            InitializeComponent();

            Wizzard = iWizzard;
        }

        private void ButtonNextTop_Click(object sender, EventArgs e)
        {
            FixItems();
        }

        private void ButtonPrevTop_Click(object sender, EventArgs e)
        {
            Wizzard.SelectedIndex = 8;
        }
        
        private void FixItems()
        {
            var bkp_panel = (BackUpPanel)Wizzard.TabPages["BackUpPage"].Controls[0];
            if (bkp_panel != null)
                bkp_panel.CreateBackUp();
                
            var settings_panel = (SettingsPanel)Wizzard.TabPages["SettingsPage"].Controls[0];
            Dictionary<String, int> errors_fixed = new Dictionary<String, int>();
            if (settings_panel != null)
            {
                   
                foreach (TreeNode node in TreeViewErrors.Nodes)
                {
                    if (node.Nodes.Count > 0)
                    {
                        if (!node.Checked)
                            continue;
                        errors_fixed.Add((String)node.Tag, 0);
                        foreach (TreeNode item in node.Nodes)
                            if (item.Checked == true && !settings_panel.GetIgnores().Contains(item.Text))
                            {
                                var tag = (KeyValuePair<IDToDescript, long>)item.Tag;
                                if (CleanEngineClient.Instance().FixItem(tag.Key.ID))
                                {
                                    errors_fixed[(String)node.Tag] += 1;
                                }
                            }
                    }
                }
            }
            var stat_panel = (StatisticsPanel)Wizzard.TabPages["StatisticsPage"].Controls[0];
            if (stat_panel != null)
            {
                stat_panel.FixItems(errors_fixed);
                Program.CurrentState = ApplicationState.eNone;
                Wizzard.SelectedTab = Wizzard.TabPages["StatisticsPage"];
            }
        }

        private void ButtonFixErrors_Click(object sender, EventArgs e)
        {
            FixItems();
        }

        private void TreeViewErrors_AfterSelect(object sender, TreeViewEventArgs e)
        {

        }

        private void ButtonIgnoreSelected_Click(object sender, EventArgs e)
        {
            var settings_panel = (SettingsPanel)Wizzard.TabPages["SettingsPage"].Controls[0];
            if (settings_panel != null)
            {
                foreach (TreeNode node in TreeViewErrors.Nodes)
                {
                    if (node.Checked && node.Nodes.Count > 0)
                    {
                        foreach (TreeNode sub_node in node.Nodes)
                        {
                            if (sub_node.Checked)
                                settings_panel.AddIgnoreKey(sub_node.Text);
                        }
                    }
                }
                
            }
        }

        long _FixingJunksSize = 0;
        int _FixingErrorsCount = 0;
        public void PrepareCaption(string top, string bottom, int errs, long junks)
        {
            labelItems.Text = top;
            labelJunkSize.Text = bottom;

            _FixingErrorsCount = errs;
            _FixingJunksSize = junks;

            _ProcessErrors();
            _ProcessJunks();
        }

        private void _ProcessErrors()
        {
            var num = labelErrorsFixing.Text.Split(' ');
            num[0] = this._FixingErrorsCount.ToString();
            labelErrorsFixing.Text = "";
            string str = String.Empty;
            foreach (var txt in num)
                str += txt + " ";
            var new_str = str.Substring(0, str.Length - 1);
            if (new_str != labelErrorsFixing.Text)
                labelErrorsFixing.Text = new_str;
        }

        private void _ProcessJunks()
        {
            var num2 = labelJunksFixing.Text.Split(' ');
            num2[0] = SizeConverter(_FixingJunksSize);
            labelJunksFixing.Text = "";
            string str = String.Empty;
            foreach (var txt in num2)
                str += txt + " ";
            var new_str = str.Substring(0, str.Length - 1) ;
            if (new_str != labelJunksFixing.Text)
                labelJunksFixing.Text = new_str;
        }

        private String SizeConverter(long input)
        {
            String retVal = String.Empty;
            if (input > 1024.0 * 1024.0 * 102.4)
            {
                var gb_size = input / (1024.0 * 1024.0 * 1024.0);
                retVal = gb_size.ToString("#.#") + "Gb";
            }
            else if (input > 1024.0 * 102.4)
            {
                var mb_size = input / (1024.0 * 1024.0);
                retVal = mb_size.ToString("#.#") + "Mb";
            }
            else if (input > 102.40)
            {
                var kb_size = input / 1024.0;
                retVal = kb_size.ToString("#.#") + "Kb";
            }
            else
                retVal = "0Mb";
            if (!String.IsNullOrEmpty(retVal)
                && (retVal.StartsWith(".") || retVal.StartsWith(",")))
            {
                var new_text = retVal.Insert(0, "0");
                retVal = new_text;
            }
            return retVal;
        }

        public void PrepareTreeWithErrors()
        {
            TreeViewErrors.AfterCheck -= TreeViewErrors_AfterCheck;
            TreeViewErrors.Nodes.Clear();
            var scanning_panel = (ScanningPanel)Wizzard.TabPages["ScanningPage"].Controls[0];
            if (scanning_panel != null)
            {
                var lst = scanning_panel.CustomScannerItems;
                foreach (var el in lst)
                {
                    if (el.FoundItems.Count == 0)
                        continue;
                    var tree_node = TreeViewErrors.Nodes.Add(el.Name);
                    tree_node.Tag = el.IDs;
                    tree_node.Checked = true;
                    foreach (var threat in el.FoundItems)
                    {
                        var sub_node = tree_node.Nodes.Add(threat.Key.Description);
                        sub_node.Tag = threat;//threat.Key.ID;
                        sub_node.Checked = true;
                    }
                    tree_node.Expand();
                }
            }
            TreeViewErrors.AfterCheck -= TreeViewErrors_AfterCheck;
            TreeViewErrors.AfterCheck += new TreeViewEventHandler(TreeViewErrors_AfterCheck);
        }

        private bool _IsFileScanner(int id)
        {
            int[] files_scanner_ids = { 20403, 20404, 20405, 20406, 20407, 20501, 20502, 20503, 20504, 20505, 20506, 20507 };
            if (files_scanner_ids.Contains(id))
                return true;
            return false;
        }

        private void TreeViewErrors_AfterCheck(object sender, TreeViewEventArgs e)
        {
            TreeViewErrors.AfterCheck -= TreeViewErrors_AfterCheck;
            TreeViewErrors.BeginUpdate();
            foreach (TreeNode child in e.Node.Nodes)
                child.Checked = e.Node.Checked;
            if (e.Node.Parent != null)
            {
                try
                {
                    var tag = (KeyValuePair<IDToDescript, long>)e.Node.Tag;
                    if (tag.Value > 0)
                    {
                        if (e.Node.Checked)
                            _FixingJunksSize += tag.Value;
                        else
                            _FixingJunksSize -= tag.Value;
                        _ProcessJunks();
                    }
                    else
                    {
                        if (e.Node.Checked)
                            _FixingErrorsCount += 1;
                        else
                            _FixingErrorsCount -= 1;
                        _ProcessErrors();
                    }
                }
                catch { }
                bool bAtLeastOneChecked = false;
                foreach (TreeNode node in e.Node.Parent.Nodes)
                {
                    if (node.Checked == true)
                    {
                        bAtLeastOneChecked = true;
                        break;
                    }
                }
                e.Node.Parent.Checked = bAtLeastOneChecked;
            }

            if (e.Node.Nodes.Count > 0)
            {
                foreach (TreeNode node in e.Node.Nodes)
                {
                    var tag = (KeyValuePair<IDToDescript, long>)node.Tag;
                    var parent_tag = (String)e.Node.Tag;
                    var scanner_id = Convert.ToInt32(parent_tag.Split(';').First());
                    if (/*tag.Value > 0 && */_IsFileScanner(scanner_id))
                    {
                        if (node.Checked)
                            _FixingJunksSize += tag.Value;
                        else
                            _FixingJunksSize -= tag.Value;
                    }
                    else
                        if (node.Checked)
                            _FixingErrorsCount += 1;
                        else
                            _FixingErrorsCount -= 1;
                }
                _ProcessJunks();
                _ProcessErrors();
            }
            TreeViewErrors.EndUpdate();
            TreeViewErrors.AfterCheck -= TreeViewErrors_AfterCheck;
            TreeViewErrors.AfterCheck += new TreeViewEventHandler(TreeViewErrors_AfterCheck);
        }

        private void TreeViewErrors_DoubleClick(object sender, EventArgs e)
        {
            return;
        }

       

    }
}
