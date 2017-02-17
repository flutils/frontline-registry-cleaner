using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Win32;
using System.Windows.Forms;
using System.Security.AccessControl;
using System.Runtime.InteropServices;
using System.Diagnostics;

namespace FrontLineGUI
{
    public class StartupEntry
    {
        #region data

        public String csName = String.Empty;
	    public String csRegPath = String.Empty;
        public bool bDisabled = false;
        public bool bLocal = false;
        
        #endregion 

        public StartupEntry()
	    {
		    csName = "";
		    csRegPath = "";
		    bDisabled = false;
		    bLocal   = false;
	    }

    }

    public class StartupManager
    {
        public List<StartupEntry> StratUpList = new List<StartupEntry>();

        public void GetStartupInRunKeys()
        {
            RegistryKey rKey = null;
            try
            {
                rKey = Registry.CurrentUser.OpenSubKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
                foreach (var name in rKey.GetValueNames())
                {
                    StratUpList.Add(new StartupEntry()
                    {
                        csName = name,
                        csRegPath = rKey.GetValue(name).ToString(),
                        bDisabled = false,
                        bLocal = false
                    });
                }
            }
            catch (Exception) { /*MessageBox.Show(ex.Message, "HKCU");*/ }
            try
            {

                rKey = Registry.LocalMachine.OpenSubKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
                foreach (var name in rKey.GetValueNames())
                {
                    StratUpList.Add(new StartupEntry()
                    {
                        csName = name,
                        csRegPath = rKey.GetValue(name).ToString(),
                        bDisabled = false,
                        bLocal = true
                    });
                }
            }
            catch (Exception) { /*MessageBox.Show(ex.Message, "HKLM");*/ }
        }

        public void GetStartupInMSConfigKeys()
        {
            RegistryKey rKey = null;
            try
            {
                rKey = Registry.CurrentUser.OpenSubKey("Software\\StartUp");//Microsoft\\Shared Tools\\MSConfig\\startupreg");
                if (rKey == null)
                    Registry.CurrentUser.CreateSubKey("Software\\StartUp");
                foreach (var sub_key in rKey.GetSubKeyNames())
                {
                    StratUpList.Add(new StartupEntry() 
                    {
                        csName = rKey.OpenSubKey(sub_key).GetValue("item").ToString(),
                        csRegPath = rKey.OpenSubKey(sub_key).GetValue("command").ToString(),
                        bLocal = (rKey.OpenSubKey(sub_key).GetValue("hkey").ToString().ToLower() == "hklm") ? true:false,
                        bDisabled = true
                    });
                }
            }
            catch (Exception) { /*MessageBox.Show(ex.Message, "MSConf");*/ }
        }

        public void MoveItemToDisabledState(StartupEntry entry)
        {
            RegistryKey rKey = null;

            try
            {
                rKey = Registry.CurrentUser.CreateSubKey("Software\\StartUp\\" + entry.csName + (entry.bLocal ? "HKLM" : "HKCU"));//Microsoft\\Shared Tools\\MSConfig\\startupreg\\" + entry.csName + (entry.bLocal ? "HKLM" : "HKCU"));
                rKey.SetValue("item", entry.csName);
                rKey.SetValue("command",entry.csRegPath);
                rKey.SetValue("hkey", entry.bLocal ? "HKLM" : "HKCU");
                entry.bDisabled = true;
            }
            catch (Exception) { /*MessageBox.Show(ex.Message);*/ }

            try
            {
                if (entry.bLocal)
                    rKey = Registry.LocalMachine.OpenSubKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", true);
                else
                    rKey = Registry.CurrentUser.OpenSubKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", true);
                rKey.DeleteValue(entry.csName, false);
            }
            catch (Exception) { /*MessageBox.Show(ex.Message); */}
            finally
            {
                entry.bDisabled = true;
            }
        }

        public void MoveItemToEnabledState(StartupEntry entry)
        {
            RegistryKey rKey = null;
            try
            {
                if (entry.bLocal)
                    rKey = Registry.LocalMachine.OpenSubKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",true);
                else
                    rKey = Registry.CurrentUser.OpenSubKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",true);
                if (null != rKey)
                {
                    rKey.SetValue(entry.csName, entry.csRegPath);
                }
            }
            catch { }

            try
            {
                Registry.CurrentUser.DeleteSubKey("Software\\StartUp\\" + entry.csName + (entry.bLocal ? "HKLM" : "HKCU")); //Microsoft\\Shared Tools\\MSConfig\\startupreg\\" + entry.csName + (entry.bLocal ? "HKLM" : "HKCU"));
            }
            catch (Exception ) { /*MessageBox.Show(ex.Message);*/ }
            finally
            {
                entry.bDisabled = false;
            }
        }

        public void KillStartupEntry(StartupEntry entry)
        {
            try
            {
                RegistryKey rKey = null;
                if (entry.bDisabled)
                    Registry.CurrentUser.DeleteSubKey("Software\\StartUp\\" + entry.csName);//Microsoft\\Shared Tools\\MSConfig\\startupreg\\" + entry.csName);
                else
                {
                    if (entry.bLocal)
                        rKey = Registry.LocalMachine.OpenSubKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", true);
                    else
                        rKey = Registry.CurrentUser.OpenSubKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", true);
                    rKey.DeleteValue(entry.csName, false);
                }
            }
            catch (Exception) { /*MessageBox.Show(ex.Message);*/ }
            finally
            {
                StratUpList.Remove(entry);
            }
        }
    }
}
