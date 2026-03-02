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

        [Required]
        public DateTime? CompletedAt { get; set; }
        public DateTime CreatedAt { get; set; }

        public DateTime? UpdatedAt { get; set; }

        // Relations
        public ICollection<ScanItem> ScanItems { get; set; }
        public ICollection<ScanResult> Results { get; set; }
        public Backup? Backup { get; set; }

    }

}
