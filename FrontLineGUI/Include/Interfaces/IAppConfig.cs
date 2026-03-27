using System.Collections.Generic;
using System.Globalization;

namespace FrontLineGUI.Include.Interfaces
{
    public interface IAppConfig
    {
        // RPECK 02/03/2026 - Language Options
        // Used to set the present language, and any of the supported langugaes inside the system
        string CurrentLanguage { get; set; }
        IReadOnlyList<CultureInfo> SupportedLanguages { get; }

        // RPECK 02/03/2026 - Options
        // Used to define the path location for configuration & database files
        string BaseFolder { get; }
        string DatabasePath { get; }
        string ConfigFilePath { get; }
        bool IsDebug { get; set;  }
        bool IsLogging { get; set; }
 
        void Load();
        void Save();
    }
}