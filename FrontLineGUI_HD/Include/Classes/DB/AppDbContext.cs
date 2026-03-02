using FrontLineGUI.Include.Classes.DB.Models;
using FrontLineGUI.Include.Interfaces;
using Microsoft.EntityFrameworkCore;
using System;
using System.Linq;

namespace FrontLineGUI.Include.Classes.DB
{

    // RPECK 02/03/2026 - ScanStatus
    // This is used to provide the means to manage how the "status" of a scan is managed
    public enum ScanStatus
    {
        Pending = 0,    // Initialized but not started
        Scanning = 1,   // Currently searching
        Completed = 2,  // Finished successfully
        Aborted = 3,    // Stopped by user
        Failed = 4      // Ended due to an error
    }

    // RPECK 02/03/2026 - DBContext
    // EntityFramework used to populate the Scan, ScanItem, Backup and Results values in the app
    public class AppDbContext : DbContext
    {
        private readonly IAppConfig _config;
        public DbSet<Scan> Scans { get; set; }
        public DbSet<ScanItem> ScanItems { get; set; }
        public DbSet<Backup> Backups { get; set; }
        public DbSet<ScanResult> Results { get; set; }

        public string DbPath { get; }

        // RPECK 02/03/2026 - Constructor
        // Creates the database and allows us to interface with it
        public AppDbContext(IAppConfig config)
        {
            _config = config;
        }

        // The following configures EF to create a Sqlite database file in the
        // special "local" folder for your platform.
        protected override void OnConfiguring(DbContextOptionsBuilder options)
        {
            // Use the path defined in your config system
            // e.g., _config.DatabasePath might return "C:\Users\Name\AppData\Local\FrontLineGUI\data.db"
            options.UseSqlite($"Data Source={_config.DatabasePath}");
        }


        // RPECK 21/02/2026 - Save Changes Hook
        // Used invoke the UpdateTimestamps function (which is responsible for the createdAt && updatedAt values being updated
        public override int SaveChanges()
        {
            UpdateTimestamps();
            return base.SaveChanges();
        }

        // RPECK 21/02/2026 - Save Changes Hook
        // Used invoke the UpdateTimestamps function (which is responsible for the createdAt && updatedAt values being updated
        public override int SaveChanges(bool acceptAllChangesOnSuccess)
        {
            UpdateTimestamps();
            return base.SaveChanges(acceptAllChangesOnSuccess);
        }

        // RPECK 21/02/2026 - Update Timestamps
        // Used to provide the means to use CreatedAt / UpdatedAt as per ActiveRecord
        private void UpdateTimestamps()
        {
            // Look for ANY entry that implements ITrackable
            var entries = ChangeTracker.Entries()
                .Where(e => e.Entity is ITrackable &&
                           (e.State == EntityState.Added || e.State == EntityState.Modified));

            foreach (var entry in entries)
            {
                var entity = (ITrackable)entry.Entity;

                // RPECK 02/03/2026 - Add the UpdatedAt value on creation only
                if (entry.State == EntityState.Added) entity.CreatedAt = DateTime.Now;

                // RPECK 02/03/2026 - Add the UpdatedAt value on modified only
                if(entry.State == EntityState.Modified) entity.UpdatedAt = DateTime.Now;
                
            }
        }

    }

}
