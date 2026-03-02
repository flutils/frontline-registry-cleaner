using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using FrontLineGUI.Include.Interfaces;

namespace FrontLineGUI.Include.Classes.DB.Models
{
    public class Scan : ITrackable
    {
        [Key]
        [DatabaseGenerated(DatabaseGeneratedOption.Identity)]
        public int ScanId { get; set; }

        // Tracks the state of the scan
        [Required]
        public ScanStatus Status { get; set; } = ScanStatus.Pending;

        // Nullable because it only has a value once Status == Completed
        public DateTime? CompletedAt { get; set; }

        [Required]
        public DateTime CreatedAt { get; set; }

        public DateTime? UpdatedAt { get; set; }

        // Relations
        public ICollection<ScanItem> ScanItems { get; set; } = new List<ScanItem>();

        // Findings: These are the actual numerical results found during the scan.
        public ICollection<ScanResult> Results { get; set; } = new List<ScanResult>();
        public Backup? Backup { get; set; }

    }

}
