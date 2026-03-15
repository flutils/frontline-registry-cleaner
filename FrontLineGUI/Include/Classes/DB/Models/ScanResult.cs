using FrontLineGUI.Include.Services;
using System;
using System.ComponentModel;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using System.Runtime.CompilerServices;

namespace FrontLineGUI.Include.Classes.DB.Models
{
    public class ScanResult : PropertyChangedBase
    {
        [Key]
        [DatabaseGenerated(DatabaseGeneratedOption.Identity)]
        public int Id { get; set; }

        [Required]
        public int ScanItemId { get; set; }

        [ForeignKey("ScanItemId")]
        public virtual ScanItem ScanItem { get; set; }

        // --- Live Data ---

        private int _count;
        public int Count
        {
            get => _count;
            set { _count = value; OnPropertyChanged(); OnPropertyChanged(nameof(IsError)); }
        }

        private long _totalBytes;
        public long TotalBytes
        {
            get => _totalBytes;
            set { _totalBytes = value; OnPropertyChanged(); OnPropertyChanged(nameof(SizeDisplay)); }
        }

        // --- UI Helpers (Not Mapped to Database) ---

        [NotMapped]
        public bool IsError => Count > 0 || TotalBytes > 0;

        [NotMapped]
        public string SizeDisplay => TotalBytes > 0 ? ScanService.FormatBytes(TotalBytes) : string.Empty;

        public DateTime CreatedAt { get; set; } = DateTime.Now;
        public DateTime? UpdatedAt { get; set; }

    }
}