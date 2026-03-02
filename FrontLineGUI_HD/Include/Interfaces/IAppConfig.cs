using System.Collections.Generic;

namespace FrontLineGUI.Include.Interfaces
{
    public interface IAppConfig
    {
        string CurrentLanguage { get; set; }
        IReadOnlyList<string> SupportedLanguages { get; }

        void Load();
        void Save();
    }
}