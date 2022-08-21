
using System;
using System.Collections.Generic;
using System.Xml;

// This is from the original FLCleanerGUI 
// It takes inputs and allows us to store registry backups using XML management classes
namespace FrontLineGUI
{
    public class BackupManager
    {

        // Vars
        public static List<String> BackUpKeys = new List<string>(); // This is populated when a registry scan is performed

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
