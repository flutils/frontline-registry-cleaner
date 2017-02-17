using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Win32;

namespace FrontLineGUI
{
    public static class ApplicationSettings
    {
        private static String _ReadValue(String ValueName)
        {
            var options = Registry.CurrentUser.OpenSubKey(Program.ApplicationOptions, false);
            if (options != null)
                return options.GetValue(ValueName, "0").ToString();
            return "1";
        }

        private static void _SetValue(String ValueName, String ValueValue)
        {
            var options = Registry.CurrentUser.OpenSubKey(Program.ApplicationOptions, true);
            if (options != null)
                options.SetValue(ValueName, ValueValue);
        }

        public static bool SetRestore
        {
            get
            {
                return _ReadValue("SetRestore") == "1" ? true : false;
            }
            set
            {
                _SetValue("SetRestore", (value == true) ? "1" : "0");
            }
        }

        public static bool AutoRepair
        {
            get
            {
                return _ReadValue("AutoRepair") == "1" ? true : false;
            }
            set
            {
                _SetValue("AutoRepair", (value == true) ? "1" : "0");
            }
        }

        public static bool AutoUpdate
        {
            get
            {
                return _ReadValue("AutoUpdate") == "1" ? true : false;
            }
            set
            {
                _SetValue("AutoUpdate", (value == true) ? "1" : "0");
            }
        }

        public static bool AutoShutDown
        {
            get
            {
                return _ReadValue("AutoShutDown") == "1" ? true : false;
            }
            set
            {
                _SetValue("AutoShutDown", (value == true) ? "1" : "0");
            }
        }

        public static bool IsPaid
        {
            get { return _ReadValue("IsPaid") == "1" ? true : false; }
            set { _SetValue("IsPaid", (value == true) ? "1" : "0"); }
        }

        public static bool NeedScan
        {
            get;
            set;
        }
    }
}
