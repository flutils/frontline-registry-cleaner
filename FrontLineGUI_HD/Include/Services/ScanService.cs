using FrontLineGUI.Include.Classes.DB.Models;
using System;
using System.Collections.Generic;

// RPECK 24/02/2026 - Service Layer
// Used to provide the means to access + manage different elements of data without having to worry about scope
namespace FrontLineGUI.Include.Services
{

    // RPECK 24/02/2026 - Scan Service
    // Used to provide the means to manage the effectve scan of the system (IE when the application loads, invoke the scanner and create a new instance)
    public class ScanService
    {
        public Scan CurrentScan { get; private set; }

        // Notify the UI when a scan starts or finishes
        public event Action<Scan>? ScanStarted;
        public event Action? ScanCleared;

        public Scan CreateNewScan(List<ScanItem> defaultTypes)
        {
            CurrentScan = new Scan();
            // logic to actually START the scanning process would go here

            ScanStarted?.Invoke(CurrentScan);
            return CurrentScan;
        }

        public void Clear()
        {
            CurrentScan = null;
            ScanCleared?.Invoke();
        }
    }

}
