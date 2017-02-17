using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Microsoft.Win32;

namespace FrontLineGUI
{
    public partial class StartUpPanel : UserControl
    {
        public WizzardControl Wizzard
        {
            get;
            private set;
        }

        private StartUpPanel() { }
        private StartupManager manager = new StartupManager();

        public StartUpPanel(WizzardControl iWizzard)
        {
            InitializeComponent();

            Wizzard = iWizzard;
            
            manager.GetStartupInRunKeys();
            manager.GetStartupInMSConfigKeys();

            foreach (var sitem in manager.StratUpList)
            {
                InsertAnItemToList(sitem);
            }
        }

        private void ButtonEnableAll_Click(object sender, EventArgs e)
        {
            ListViewStartup.BeginUpdate();
            ListViewStartup.Items.Clear();
            foreach (var sitem in manager.StratUpList)
            {
                manager.MoveItemToEnabledState(sitem);
                InsertAnItemToList(sitem);
            }
            ListViewStartup.EndUpdate();
        }

        private void ButtonDisableAll_Click(object sender, EventArgs e)
        {
            ListViewStartup.BeginUpdate();
            ListViewStartup.Items.Clear();
            foreach (var sitem in manager.StratUpList)
            {
                manager.MoveItemToDisabledState(sitem);
                InsertAnItemToList(sitem);
            }
            ListViewStartup.EndUpdate();
        }

        private void ButtonEnableSelectedStartUp_Click(object sender, EventArgs e)
        {
            if (ListViewStartup.SelectedIndices.Count < 1)
                return;

            var selected_item = ListViewStartup.Items[ListViewStartup.SelectedIndices[0]];
            var item = (StartupEntry)selected_item.Tag;
            if (item != null)
            {
                manager.MoveItemToEnabledState(item);
                ListViewStartup.Items.Remove(selected_item);
                InsertAnItemToList(item);
            }
        }

        private void ButtonDeleteSelectedStartup_Click(object sender, EventArgs e)
        {
            if (ListViewStartup.SelectedIndices.Count < 1)
                return;

            ListViewStartup.BeginUpdate();
            var selected_item = ListViewStartup.Items[ListViewStartup.SelectedIndices[0]];
            manager.KillStartupEntry((StartupEntry)selected_item.Tag);
            ListViewStartup.Items.Remove(selected_item);
            ListViewStartup.EndUpdate();
            
        }

        private void ListViewStartup_ItemSelectionChanged(object sender, ListViewItemSelectionChangedEventArgs e)
        {
            try
            {
                if (e.Item.SubItems[1].Text.ToLower() == "disabled")
                {
                    ButtonEnableSelectedStartUp.Visible = true;
                    ButtonDisableSelected.Visible = false;
                }
                else
                {
                    ButtonDisableSelected.Visible = true;
                    ButtonEnableSelectedStartUp.Visible = false;
                }
            }
            catch { }
        }

        private void ButtonDisableSelected_Click(object sender, EventArgs e)
        {
            if (ListViewStartup.SelectedIndices.Count < 1)
                return;
            var selected_item = ListViewStartup.Items[ListViewStartup.SelectedIndices[0]];
            var item = (StartupEntry)selected_item.Tag;
            if (item != null)
            {
                manager.MoveItemToDisabledState(item);
                ListViewStartup.Items.Remove(selected_item);
                InsertAnItemToList(item);
            }
        }

        private void InsertAnItemToList(StartupEntry entry)
        {
            ListViewItem lv_item = new ListViewItem(entry.csName);
            lv_item.SubItems.Add(entry.bDisabled ? "Disabled" : "Enabled");
            lv_item.SubItems.Add(entry.csRegPath);
            lv_item.Tag = entry;
            ListViewStartup.Items.Add(lv_item);
        }

    }
}
