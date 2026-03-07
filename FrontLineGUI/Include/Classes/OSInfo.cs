using JCS;
using System;
using System.Runtime.InteropServices;

namespace FrontLineGUI
{
    public class OSInfo
    {
        // Pre-calculated properties (Performance optimization)
        public string OSName { get; }
        public string OSVersion { get; }
        public string OSIcon { get; }
        public string OSArchitecture { get; }
        public string FullDisplayString { get; }

        public OSInfo()
        {
            // 1. Get Version String
            OSVersion = OSVersionInfo.VersionString.ToString();

            // 2. Handle Windows 11 Detection 
            // Microsoft internally identifies Win11 as Win10 Build 22000+
            if (OSVersionInfo.MajorVersion == 10 && OSVersionInfo.BuildVersion >= 22000)
                OSName = "Windows 11";
            else
                OSName = OSVersionInfo.Name;

            // 3. Architecture
            string arch = Environment.Is64BitOperatingSystem ? "64bit" : "32bit";
            OSArchitecture = arch;

            // 4. Icon Path (Consistent with your folder structure)
            OSIcon = $"/Resources/OS/{OSName.Replace(" ", "-").ToLower()}.png";

            // 5. Helper string for your UI (e.g., "Windows 11 (64bit)")
            FullDisplayString = $"{OSName} ({OSArchitecture})";
        }
    }
}