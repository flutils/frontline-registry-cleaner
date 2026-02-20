using Microsoft.EntityFrameworkCore;
using System;

namespace FrontLineGUI.Include.Classes.Database
{
    public class Context : DbContext
    {
        //public DbSet<Blog> Scans { get; set; }
        //public DbSet<Post> Backups { get; set; }
        //public DbSet<Post> Options { get; set; }
        //public DbSet<Post> ScanTypes { get; set; }
        //public DbSet<Post> ScanResults { get; set; }

        public string DbPath { get; }

        public Context()
        {
            var folder = Environment.SpecialFolder.LocalApplicationData;
            var path = Environment.GetFolderPath(folder);
            DbPath = System.IO.Path.Join(path, "blogging.db");
        }

        // The following configures EF to create a Sqlite database file in the
        // special "local" folder for your platform.
        protected override void OnConfiguring(DbContextOptionsBuilder options)
            => options.UseSqlite($"Data Source={DbPath}");
    }
}
