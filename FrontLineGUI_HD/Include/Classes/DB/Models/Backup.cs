using System;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using FrontLineGUI.Include.Interfaces;

namespace FrontLineGUI.Include.Classes.DB.Models
{
    public class Backup : ITrackable
    {
        [Key]
        [DatabaseGenerated(DatabaseGeneratedOption.Identity)]
        public int BackupId { get; set; }

        // Link to the Scan that triggered this backup
        [Required]
        public int ScanId { get; set; }

        [ForeignKey("ScanId")]
        public Scan Scan { get; set; }

        // The path to the .reg file or the blob of data saved
        [Required]
        public string BackupFilePath { get; set; }

        [Required]
        public DateTime CreatedAt { get; set; } = DateTime.Now;

        public DateTime? UpdatedAt { get; set; }
    }
}