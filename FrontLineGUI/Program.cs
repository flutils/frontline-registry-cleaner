using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using Microsoft.Win32;

namespace FrontLineGUI
{
    public enum ApplicationState
    {
        eNone = 0,
        eFixingErrors
    }

    static class Program
    {
        public static String ApplicationOptions = @"Software\FrontLine";

        public static ApplicationState CurrentState
        {
            get;
            set;
        }

        [STAThread]
        static void Main(string[] args)
        {
            try
            {
                CurrentState = ApplicationState.eNone;
                try
                {
                    if (null == Registry.CurrentUser.OpenSubKey(Program.ApplicationOptions))
                        Registry.CurrentUser.CreateSubKey(Program.ApplicationOptions);
                    ApplicationSettings.NeedScan = false;
                    if (args.Contains("-scan"))
                    {
                        ApplicationSettings.NeedScan = true;
                    }
                }
                catch { }
                Application.EnableVisualStyles();
                Application.SetCompatibleTextRenderingDefault(false);
                var MainWnd = new MainWindow();

                Application.Run(MainWnd);
            }
            catch
            { /*do nothing*/ }
        }
    }
}