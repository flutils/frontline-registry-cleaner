using System;
using System.Collections.Generic;
using System.IO;
using System.Text.Json;

namespace FrontLineGUI.Include.Services
{
    public class AppConfig : IAppConfig
    {
        private readonly string _filePath;

        // RPECK 01/03/2026 - Defaults
        // Used to provide the means to provide default values to the config class
        public string CurrentLanguage { get; set; } = "en-GB";
        public IReadOnlyList<string> SupportedLanguages { get; } = new List<string> { "en-GB", "fr-FR" };

        public AppConfig()
        {
            // Store config in %AppData%\Frontline\config.json
            string folder = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData), "FLCleaner-2.0");

            if (!Directory.Exists(folder))
                Directory.CreateDirectory(folder);

            _filePath = Path.Combine(folder, "config.json");

            // Load existing settings immediately upon service creation
            Load();
        }

        public void Load()
        {
            if (!File.Exists(_filePath)) return;

            try
            {
                string json = File.ReadAllText(_filePath);
                // Deserialize into a temporary object to avoid overwriting defaults with nulls
                var loaded = JsonSerializer.Deserialize<AppConfig>(json);

                if (loaded != null)
                {
                    this.CurrentLanguage = loaded.CurrentLanguage;
                }
            }
            catch (Exception ex)
            {
                // If the JSON is corrupted, we stay with the defaults
                System.Diagnostics.Debug.WriteLine($"Config Load Error: {ex.Message}");
            }
        }

        public void Save()
        {
            try
            {
                var options = new JsonSerializerOptions { WriteIndented = true };
                string json = JsonSerializer.Serialize(this, options);
                File.WriteAllText(_filePath, json);
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine($"Config Save Error: {ex.Message}");
            }
        }
    }
}