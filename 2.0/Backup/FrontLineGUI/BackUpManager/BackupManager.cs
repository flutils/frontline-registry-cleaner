using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Win32;
using System.Diagnostics;
using System.IO;
using Cll;

namespace FrontLineGUI
{
    public class BackupManager
    {
        public static List<String> BackUpKeys = new List<string>();

        public String ErrorMessage
        {
            get;
            private set;
        }

        public bool BackUpList(String location)
        {
            xmlWriter w = new xmlWriter();
            xmlRegistry xmlReg = new xmlRegistry();
            try
            {
                w.open(location);
                foreach (var strRegistrySection in BackUpKeys)
                {
                    if (xmlRegistry.keyExists(strRegistrySection))
                    {
                        xmlElement wroot = new xmlElement(xmlRegistry.XML_ROOT);
                        wroot.write(w, 1, false, true);
                        xmlReg.saveAsXml(w, false, strRegistrySection, "");
                        wroot.writeClosingTag(w, -1, false, true);
                    }
                }
            }
            catch (Exception ex)
            {
                ErrorMessage = ex.Message;
                return false;
            }
            finally
            {
                w.close();
            }
            return true;
        }

        public bool RestoreBackUp(String location)
        {
            xmlReader xmlReader = new xmlReader();
			xmlRegistry xmlReg = new xmlRegistry();
            try
            {
                xmlReg.loadAsXml(xmlReader, location);
            }
            catch (Exception ex) { ErrorMessage = ex.Message; return false; }
            return true;
        }
    }
}
