using FrontLineGUI.Include.Interfaces;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Text.Json;
using System.Text.Json.Serialization;

namespace FrontLineGUI.Include.Services
{
    public class AppConfig : IAppConfig
    {
        // Paths (not serialized)
        [JsonIgnore]
        public string BaseFolder { get; }

        [JsonIgnore]
        public string DatabasePath => Path.Combine(BaseFolder, "data.db");

        [JsonIgnore]
        public string ConfigFilePath => Path.Combine(BaseFolder, "config.json");

        // Settings (serialized)
        public string CurrentLanguage { get; set; }
        public bool IsDebug { get; set; } = false;
        public bool IsLogging { get; set; } = false;

        // Dynamically derived supported languages (from .resx)
        [JsonIgnore]
        public IReadOnlyList<CultureInfo> SupportedLanguages =>
            GetSupportedCultures()
                .OrderBy(c => c.NativeName)
                .ToList();

        public AppConfig()
        {
            // Set up local app folder
            var path = Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData);
            BaseFolder = Path.Combine(path, "FrontLineGUI");

            if (!Directory.Exists(BaseFolder))
                Directory.CreateDirectory(BaseFolder);

            // Load config from disk
            Load();

            // Ensure CurrentLanguage is valid
            if (string.IsNullOrWhiteSpace(CurrentLanguage) ||
                !SupportedLanguages.Any(c => c.Name == CurrentLanguage))
            {
                CurrentLanguage = SupportedLanguages.FirstOrDefault()?.Name
                                  ?? CultureInfo.CurrentUICulture.Name;
            }
        }

        private IEnumerable<CultureInfo> GetSupportedCultures()
        {
            // Point this specifically to your Strings class
            var rm = FrontLineGUI.Resources.Localization.Strings.ResourceManager;

            return CultureInfo.GetCultures(CultureTypes.SpecificCultures)
                .Where(c => {
                    try
                    {
                        // Look for the resource set. 
                        // If it finds 'Strings.fr.resx' for a French culture, it returns true.
                        var rs = rm.GetResourceSet(c, true, false);
                        return rs != null;
                    }
                    catch { return false; }
                });
        }

        public void Load()
        {
            if (!File.Exists(ConfigFilePath))
                return;

            try
            {
                string json = File.ReadAllText(ConfigFilePath);

                var loaded = JsonSerializer.Deserialize<ConfigData>(json);

                if (loaded != null)
                {
                    CurrentLanguage = loaded.CurrentLanguage;
                    IsDebug = loaded.IsDebug;
                    IsLogging = loaded.IsLogging;
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

                var dataToSave = new ConfigData
                {
                    CurrentLanguage = this.CurrentLanguage,
                    IsDebug = this.IsDebug,
                    IsLogging = this.IsLogging
                };

                string json = JsonSerializer.Serialize(dataToSave, options);
                File.WriteAllText(ConfigFilePath, json);
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine($"Config Save Error: {ex.Message}");
            }
        }

        // DTO for serialization
        private class ConfigData
        {
            public string CurrentLanguage { get; set; }
            public bool IsDebug { get; set; }
            public bool IsLogging { get; set; }
        }
    }
}