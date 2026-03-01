using FrontLineGUI.Include.Classes.DB.Models;
using System;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace FrontLineGUI.Include.Services
{
    // The Enum lives here now, so the Service can dictate the state
    public enum ScanProcessState
    {
        Ready,
        Scanning,
        Completed,
        Error
    }

    public class ScanService
    {
        public Scan CurrentScan { get; private set; }
        
        // Expose current properties so if a user navigates away and back, 
        // the new ViewModel can instantly sync up with the ongoing scan.
        public ScanProcessState CurrentState { get; private set; } = ScanProcessState.Ready;
        public double CurrentProgress { get; private set; } = 0;

        // Events that the ViewModel will listen to
        public event Action<double>? ProgressChanged;
        public event Action<ScanProcessState>? StateChanged;

        public Scan CreateNewScan(List<ScanItem> defaultTypes)
        {
            CurrentScan = new Scan();
            return CurrentScan;
        }

        public void StartScan(List<ScanItem> defaultTypes)
        {
            // Prevent starting a scan if one is already running
            if (CurrentState == ScanProcessState.Scanning) return; 

            CreateNewScan(defaultTypes);
            UpdateState(ScanProcessState.Scanning);
            UpdateProgress(0);

            // Task.Run pushes the heavy lifting to a background thread
            // so your UI (the app window) doesn't freeze!
            Task.Run(async () =>
            {
                try
                {
                    // SIMULATION: Replace this loop with your actual file scanning logic
                    for (int i = 0; i <= 100; i += 2)
                    {
                        await Task.Delay(50); // Simulating time taken to scan
                        UpdateProgress(i);
                    }

                    UpdateState(ScanProcessState.Completed);
                }
                catch (Exception)
                {
                    UpdateState(ScanProcessState.Error);
                }
            });
        }

        public void Clear()
        {
            CurrentScan = null;
            UpdateProgress(0);
            UpdateState(ScanProcessState.Ready);
        }

        // Helper methods to update internal state AND fire the events
        private void UpdateState(ScanProcessState newState)
        {
            CurrentState = newState;
            StateChanged?.Invoke(newState);
        }

        private void UpdateProgress(double progress)
        {
            CurrentProgress = progress;
            ProgressChanged?.Invoke(progress);
        }
    }
}
