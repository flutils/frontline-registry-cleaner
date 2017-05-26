using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Windows.Forms;
using System.IO;
using System.Management;

namespace FrontLineGUI
{
    public enum LicenseStatus
    {
        eLicenseInvalid = 0,
        eLicenseOK = 1,
        eLicenseBlock = 2
    }

    public class LicenseManager
    {
        private string GetHardwareID()
        {
            var dirs = Directory.GetLogicalDrives().First().Split(':');
            string frst_dir = null;
            try
            {
                frst_dir = dirs.First();
                ManagementObject dsk = new ManagementObject(@"win32_logicaldisk.deviceid=""" + frst_dir + @":""");
                dsk.Get();
                return dsk["VolumeSerialNumber"].ToString();
            }
            catch
            {
                try
                {
                    frst_dir = dirs[1];
                    ManagementObject dsk = new ManagementObject(@"win32_logicaldisk.deviceid=""" + frst_dir + @":""");
                    dsk.Get();
                    return dsk["VolumeSerialNumber"].ToString();
                }
                catch { return null; }
            }
            return null;
        }

        public LicenseStatus CheckLicense(string license)
        {
            if (String.IsNullOrEmpty(license))
            {
                MessageBox.Show("Incorrect license!");
                return LicenseStatus.eLicenseInvalid;
            }
            HttpWebRequest request = (HttpWebRequest)WebRequest.Create("http://license.flcleaner.com/auth/");
            request.Method = "POST";
            request.ContentType = "application/x-www-form-urlencoded";
            string parameters = "license="+license;
            parameters+="&hardware=";
            parameters += GetHardwareID();
            byte[] bytes = Encoding.UTF8.GetBytes(parameters);
            Stream os = null;
            try
            { 
                request.ContentLength = bytes.Length;   
                os = request.GetRequestStream();
                os.Write(bytes, 0, bytes.Length);         
            }
            catch (WebException ex)
            {
                MessageBox.Show(ex.Message, "HttpPost: Request error",
                   MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            finally
            {
                if (os != null)
                    os.Close();
            }

            HttpWebResponse response = (HttpWebResponse)request.GetResponse();

            StreamReader responseStream = new StreamReader(response.GetResponseStream());
            byte[] buffer = new byte[12];
            string responseBody = responseStream.ReadToEnd().Trim();

            response.Close();
            request.Abort();


            responseBody = responseBody.ToLower();

            switch (responseBody)
            {
                case "0":
                    return LicenseStatus.eLicenseInvalid;
                case "1":
                    return LicenseStatus.eLicenseOK;
                case "2":
                default:
                    return LicenseStatus.eLicenseBlock;
            }

            return LicenseStatus.eLicenseBlock;
        }
    }
}
