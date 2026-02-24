using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace FrontLineGUI.Include.Classes.DB.Models
{
    // RPECK 26/03/2023
    // ScanItem class for the 'scan' page -- allows us to populate a datagrid without having to define the items manually
    // Underlying benefit is that it gives us the means to extend the underlying scan feature as needed
    public class ScanItem : PropertyChangedBase
    {
        // Primary key
        [Key]
        public int ScanItemId { get; set; }

        // Name of the scan type
        private string _name;
        public string Name
        {
            get => _name;
            set
            {
                _name = value;
                OnPropertyChanged(nameof(Name));
            }
        }

        // Label / description
        private string _label;
        public string Label
        {
            get => _label;
            set
            {
                _label = value;
                OnPropertyChanged(nameof(Label));
            }
        }

        // Optional image/icon path
        private string _image;
        public string Image
        {
            get => _image;
            set
            {
                _image = value;
                OnPropertyChanged(nameof(Image));
            }
        }

        // Is selected by user (for UI binding)
        private bool _isselected = true;
        [NotMapped] // optional: UI-only
        public bool IsSelected
        {
            get => _isselected;
            set
            {
                _isselected = value;
                OnPropertyChanged(nameof(IsSelected));
            }
        }

        // Optional: list of codes for scan (string array stored as delimited string)
        private List<string> _scancodes = new List<string>();

        [NotMapped] // not persisted
        public List<string> scancodes
        {
            get => _scancodes;
            set
            {
                _scancodes = value;
                OnPropertyChanged(nameof(scancodes));
            }
        }

        // Persisted version of scancodes for EF
        public string ScanCodesString
        {
            get => string.Join(";", _scancodes);
            set
            {
                _scancodes = new List<string>();
                if (!string.IsNullOrEmpty(value))
                {
                    foreach (var code in value.Split(';'))
                        if (!string.IsNullOrWhiteSpace(code))
                            _scancodes.Add(code);
                }
                OnPropertyChanged(nameof(ScanCodesString));
            }
        }

        // Foreign Key to Scan 
        public int? ScanId { get; set; }
        public virtual Scan Scan { get; set; }

        // Constructor
        // https://learn.microsoft.com/en-us/dotnet/desktop/wpf/data/how-to-create-and-bind-to-an-observablecollection?view=netframeworkdesktop-4.8
        public ScanItem(string name, string label = null, bool enabled = true, string image = null, string scanCodesString = "0;")
        {
            Name = name;
            Label = label;
            Image = image;

            // Initialize scan codes from string
            ScanCodesString = scanCodesString;
        }

        public ScanItem() { } // parameterless constructor required for EF
    }
}