using System.Collections.Generic;

namespace FrontLineGUI.Include.Services
{
    public interface IAppConfig
    {
        string CurrentLanguage { get; set; }
        IReadOnlyList<string> SupportedLanguages { get; }

        void Load();
        void Save();
    }
}