using FrontLineGUI.Include.Classes.DB.Models;
using Microsoft.EntityFrameworkCore;
using System;

namespace FrontLineGUI.Include.Classes.DB
{
    public class AppDbContext : DbContext
    {
        public DbSet<Scan> Scans { get; set; }
        public DbSet<ScanItem> ScanItems { get; set; }
        public DbSet<Backup> Backups { get; set; }
        public DbSet<ScanResult> Results { get; set; }

        public string DbPath { get; }

        public AppDbContext()
        {
            var folder = Environment.SpecialFolder.LocalApplicationData;
            var path = Environment.GetFolderPath(folder);
            DbPath = System.IO.Path.Join(path, "data.db");
        }

        // The following configures EF to create a Sqlite database file in the
        // special "local" folder for your platform.
        protected override void OnConfiguring(DbContextOptionsBuilder options)
            => options.UseSqlite($"Data Source={DbPath}");

        
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
            var entries = ChangeTracker.Entries<Scan>();

            foreach (var entry in entries)
            {
                if (entry.State == EntityState.Added)
                {
                    entry.Entity.CreatedAt = DateTime.UtcNow;
                }

                if (entry.State == EntityState.Modified)
                {
                    entry.Entity.UpdatedAt = DateTime.UtcNow;
                }
            }
        }

    }
}
