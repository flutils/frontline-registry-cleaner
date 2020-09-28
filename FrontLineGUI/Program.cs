using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
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
                //throw new NullReferenceException("Student object is null.");

                if (null == Registry.CurrentUser.OpenSubKey(Program.ApplicationOptions))
                    Registry.CurrentUser.CreateSubKey(Program.ApplicationOptions);

                // RPECK 28/09/2020
                ApplicationSettings.NeedScan = args.Contains("-scan") ? true : false;

                Application.EnableVisualStyles();
                Application.SetCompatibleTextRenderingDefault(false);
                var MainWnd = new MainWindow();

                Application.Run(MainWnd);
            }
            catch (Exception ex)
            {
                // Log 
                // Dump exception to a log (txt)
                string strPath = @"Log.txt";
                if (!File.Exists(strPath))
                {
                    File.Create(strPath).Dispose();
                }
                using (StreamWriter sw = File.AppendText(strPath))
                {
                    sw.WriteLine("===========Start============= " + DateTime.Now);
                    sw.WriteLine("Error Message: " + ex.Message);
                    sw.WriteLine("Stack Trace: " + ex.StackTrace);
                    sw.WriteLine("===========End============= " + DateTime.Now);
                }
            }
        }
    }
}