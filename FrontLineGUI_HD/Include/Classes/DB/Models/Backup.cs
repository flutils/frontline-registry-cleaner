using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using System.Text;

namespace FrontLineGUI.Include.Classes.DB.Models
{
    public class Backup
    {

        [Key]
        [DatabaseGenerated(DatabaseGeneratedOption.Identity)]
        public Scan Scan { get; set; }

        [Required]
        public DateTime CreatedAt { get; set; }

        public DateTime? UpdatedAt { get; set; }

    }
}
