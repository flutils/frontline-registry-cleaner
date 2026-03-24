using System.Collections.Generic;

namespace FrontLineGUI.Include.Interfaces
{
    public interface IAppConfig
    {
        // RPECK 02/03/2026 - Language Options
        // Used to set the present language, and any of the supported langugaes inside the system
        string CurrentLanguage { get; set; }
        IReadOnlyList<string> SupportedLanguages { get; }

        // RPECK 02/03/2026 - Configuration Paths
        // Used to define the path location for configuration & database files
        string BaseFolder { get; }
        string DatabasePath { get; }
        string ConfigFilePath { get; }

        // RPECK 24/03/2026 - Debug
        bool IsDebug { get; set;  }

        void Load();
        void Save();
    }
}