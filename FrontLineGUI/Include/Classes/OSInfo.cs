using JCS;
using System;
using System.Runtime.InteropServices;

namespace FrontLineGUI
{
    public class OSInfo
    {
        // Cache the static info so it's only calculated ONCE for the entire app lifetime
        private static readonly Lazy<OSInfo> _instance = new(() => new OSInfo());
        public static OSInfo Default => _instance.Value;

        public string OSName { get; }
        public string OSVersion { get; }
        public string OSIcon { get; }
        public string OSArchitecture { get; }
        public string FullDisplayString { get; }

        public OSInfo()
        {
            // Cache version once to avoid multiple property lookups in JCS
            var major = OSVersionInfo.MajorVersion;
            var build = OSVersionInfo.BuildVersion;

            OSVersion = OSVersionInfo.VersionString;

            // Windows 11 Detection 
            OSName = (major == 10 && build >= 22000) ? "Windows 11" : OSVersionInfo.Name;

            // Architecture - Using RuntimeInformation is slightly faster than Environment checks
            OSArchitecture = RuntimeInformation.OSArchitecture == Architecture.X64 ? "64bit" : "32bit";

            // Pre-format the icon path to avoid runtime string manipulation during UI binding
            // We use Ordinal comparison for speed in the Replace call
            OSIcon = $"/Resources/OS/{OSName.Replace(" ", "-", StringComparison.Ordinal).ToLowerInvariant()}.png";

            FullDisplayString = $"{OSName} ({OSArchitecture})";
        }
    }
}