using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Windows.Forms;
using System.Diagnostics;
using System.Net;
using System.Text.RegularExpressions;

namespace FrontLineGUI
{
    public struct UpdateResponse
    {
        public int Version;
        public String DownloadUrl;
    }

    public class UpdateManager
    {
        
        public UpdateResponse CheckUpdate()
        {
            string DownlodUrl = String.Empty;
            string responseBody = String.Empty;
            int serv_version = -99;
            try
            {
                WebRequest request = WebRequest.Create("http://www.frontlinecleaner.com/license2/update.ver");
                HttpWebResponse response = (HttpWebResponse)request.GetResponse();
                Stream responseStream = response.GetResponseStream();
                byte[] buffer = new byte[200];
                responseStream.Read(buffer, 0, 200);

                response.Close();
                request.Abort();

                responseBody = System.Text.Encoding.UTF8.GetString(buffer);
            }
            catch
            {
            }
            string vers = "version=";
            string url = "url=";
            if (responseBody.ToLower().Contains(vers))
            {
                
                var strs = Regex.Split(responseBody, "\r\n");
                foreach (var par in strs)
                {
                    if (par.Contains(vers))
                    {
                        serv_version = Convert.ToInt32(par.Substring(vers.Length + 1));
                    }
                    if (par.Contains(url))
                        DownlodUrl = par.Substring(url.Length);
                }
            }
            return new UpdateResponse { DownloadUrl = DownlodUrl, Version = serv_version };
        }

        public void ApplyUpdate(String upd_url)
        {
            if (!String.IsNullOrEmpty(upd_url))
            {
                try
                {
                    if (File.Exists(Path.Combine(Application.CommonAppDataPath, "FrontLineGUI.exe")))
                        File.Delete(Path.Combine(Application.CommonAppDataPath, "FrontLineGUI.exe"));
                    if (File.Exists(Path.Combine(Application.CommonAppDataPath, "runner.bat")))
                        File.Delete(Path.Combine(Application.CommonAppDataPath, "runner.bat"));
                    WebClient Client = new WebClient();
                    Client.DownloadFile(upd_url, Path.Combine(Application.CommonAppDataPath, "FrontLineGUI.exe"));
                    String strSriptTemplate = String.Empty;

                    strSriptTemplate = "if exist \"{1}\" del /f \"{0}\"\nif exist \"{0}\" sleep 5 \ncopy \"{1}\" \"{0}\"";
                    strSriptTemplate += "\ncd \"{2}\"\nstart FrontLineGUI.exe";
                    String strScript = String.Format(strSriptTemplate, Application.ExecutablePath, Path.Combine(Application.CommonAppDataPath, "FrontLineGUI.exe"), Environment.CurrentDirectory);

                    MessageBox.Show("Application is updated. It will be restarted in few momments.", "Notification", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                    try
                    {
                        StreamWriter sw = new StreamWriter(Application.CommonAppDataPath + @"\runner.bat");
                        sw.WriteLine(strScript);
                        sw.Close();
                        Process.Start(Application.CommonAppDataPath + @"\runner.bat");
                    }
                    catch (Exception)
                    {
                    }
                    Process.GetCurrentProcess().CloseMainWindow();
                    Process.GetCurrentProcess().Close();
                    Process.GetCurrentProcess().Kill();
                }
                catch { }
            }
        }
    }
}
