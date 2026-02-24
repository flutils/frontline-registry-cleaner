using System;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace FrontLineGUI.Include.Classes.DB.Models
{
    public class ScanResult
    {
        [Key]
        [DatabaseGenerated(DatabaseGeneratedOption.Identity)]
        public Scan Scan { get; set; }

        [Required]
        public ScanItem ScanItem { get; set; }
        public int Count { get; set; }
        public DateTime CreatedAt { get; set; }

        public DateTime? UpdatedAt { get; set; }

    }

}
