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

        // RPECK 24/02/2026 - Set the currentScan attribute as a means to track the scan that's presently under way (this is created by the "ScanViewModel" on initiatialisation)
        public Scan currentScan { get; private set; }

        // RPECK 24/02/2026 - Create a new Scan
        // This requires a list of ScanItems and will then populate the "Scan" object in the database
        public Scan CreateNewScan(List<ScanItem> defaultTypes)
        {
            currentScan = new Scan();

            return currentScan;
        }

        public void Clear()
        {
            currentScan = null;
        }
    }

}
