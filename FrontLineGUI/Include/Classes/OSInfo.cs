using JCS;
using System;
using System.Runtime.InteropServices;

namespace FrontLineGUI
{
    public class OSInfo
    {
        // Singleton instance for the Service to consume
        private static readonly Lazy<OSInfo> _instance = new(() => new OSInfo());
        public static OSInfo Default => _instance.Value;

        // Basic Properties
        public string OSName { get; }
        public string OSVersion { get; }
        public string OSIcon { get; }
        public string OSArchitecture { get; }

        // Extended Metadata
        public string ComputerName { get; }
        public string UserName { get; }
        public string Uptime { get; }

        // Pre-formatted string for UI Headers
        public string FullDisplayString { get; }

        public OSInfo()
        {
            // 1. Versioning Logic
            var major = OSVersionInfo.MajorVersion;
            var build = OSVersionInfo.BuildVersion;
            OSVersion = OSVersionInfo.VersionString;

            // 2. Windows 11 Detection (Build 22000+ is the cutoff for Win11)
            if (major == 10 && build >= 22000)
            {
                OSName = "Windows 11";
            }
            else
            {
                OSName = OSVersionInfo.Name;
            }

            // 3. System Identity
            ComputerName = Environment.MachineName;
            UserName = Environment.UserName;
            OSArchitecture = RuntimeInformation.OSArchitecture == Architecture.X64 ? "64-bit" : "32-bit";

            // 4. Uptime Calculation (Converted to a friendly string)
            var upTimeSpan = TimeSpan.FromMilliseconds(Environment.TickCount64);
            Uptime = $"{upTimeSpan.Days}d {upTimeSpan.Hours}h {upTimeSpan.Minutes}m";

            // 5. Resource Formatting
            // Generates: /Resources/OS/windows-11.png
            OSIcon = $"/Resources/OS/{OSName.Replace(" ", "-", StringComparison.Ordinal).ToLowerInvariant()}.png";

            FullDisplayString = $"{OSName} {OSArchitecture}";
        }

        /// <summary>
        /// Refreshes the uptime specifically, as it's the only OS property that changes.
        /// </summary>
        public string GetCurrentUptime()
        {
            var upTimeSpan = TimeSpan.FromMilliseconds(Environment.TickCount64);
            return $"{upTimeSpan.Days}d {upTimeSpan.Hours}h {upTimeSpan.Minutes}m";
        }
    }
}