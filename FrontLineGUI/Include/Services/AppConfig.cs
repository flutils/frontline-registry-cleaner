using System;
using System.Collections.Generic;
using System.IO;
using System.Text.Json;
using System.Text.Json.Serialization;
using FrontLineGUI.Include.Interfaces;

namespace FrontLineGUI.Include.Services
{
    public class AppConfig : IAppConfig
    {
        // RPECK 02/03/2026 - Paths 
        // These are excluded from JSON serialization because they are calculated dynamically
        [JsonIgnore]
        public string BaseFolder { get; }
        [JsonIgnore]
        public string DatabasePath => Path.Combine(BaseFolder, "data.db");
        [JsonIgnore]
        public string ConfigFilePath => Path.Combine(BaseFolder, "config.json");

        // RPECK 01/03/2026 - Settings to be saved
        public string CurrentLanguage { get; set; } = "en-GB";

        // Non-settable property for logic
        [JsonIgnore]
        public IReadOnlyList<string> SupportedLanguages { get; } = new List<string> { "en-GB", "fr-FR" };

        public AppConfig()
        {
            // Set up the local environment
            var path = Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData);
            BaseFolder = Path.Combine(path, "FrontLineGUI");

            if (!Directory.Exists(BaseFolder)) Directory.CreateDirectory(BaseFolder);

            // Load existing settings immediately
            Load();
        }

        public void Load()
        {
            if (!File.Exists(ConfigFilePath)) return;

            try
            {
                string json = File.ReadAllText(ConfigFilePath);

                // We deserialize into a plain DTO or use a specific options set
                // to avoid issues with read-only properties like BaseFolder
                var loaded = JsonSerializer.Deserialize<ConfigData>(json);

                if (loaded != null)
                {
                    this.CurrentLanguage = loaded.CurrentLanguage ?? "en-GB";
                }
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine($"Config Load Error: {ex.Message}");
            }
        }

        public void Save()
        {
            try
            {
                var options = new JsonSerializerOptions { WriteIndented = true };

                // We only want to save the user-changeable settings, not the paths
                var dataToSave = new ConfigData { CurrentLanguage = this.CurrentLanguage };

                string json = JsonSerializer.Serialize(dataToSave, options);
                File.WriteAllText(ConfigFilePath, json);
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine($"Config Save Error: {ex.Message}");
            }
        }

        // Internal DTO to prevent JSON errors with complex types/getters
        private class ConfigData
        {
            public string CurrentLanguage { get; set; }
        }

    }
}