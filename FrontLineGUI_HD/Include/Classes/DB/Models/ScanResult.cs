using System;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace FrontLineGUI.Include.Classes.DB.Models
{
    public class ScanResult
    {
        [Key]
        [DatabaseGenerated(DatabaseGeneratedOption.Identity)]
        public int Id { get; set; }

        // Link to the parent Scan session
        [Required]
        public int ScanId { get; set; }
        [ForeignKey("ScanId")]
        public Scan Scan { get; set; }

        // Link to the Category (e.g., "Invalid Typelib")
        [Required]
        public int ScanItemId { get; set; }
        [ForeignKey("ScanItemId")]
        public ScanItem ScanItem { get; set; }

        public int Count { get; set; }

        // This handles "3.4GB" or "600MB" values if it's not a simple count
        public string? SizeDisplay { get; set; }

        public DateTime CreatedAt { get; set; } = DateTime.Now;
        public DateTime? UpdatedAt { get; set; }
    }
}
