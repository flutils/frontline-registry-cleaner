using FrontLineGUI.Include.Classes.DB.Models;
using System;
using System.Collections.Generic;
using System.Threading;
using System.Threading.Tasks;

namespace FrontLineGUI.Include.Services
{
    public enum ScanProcessState
    {
        Ready,
        Scanning,
        Paused,
        Completed,
        Error
    }

    public class ScanService
    {
        // RPECK 06/03/2026 - Control Mechanisms
        private CancellationTokenSource? _cts;
        private TaskCompletionSource<bool>? _pauseTcs;

        // RPECK 06/03/2026 - Provide Information to the view
        public int CurrentErrorCount { get; private set; }
        public long CurrentJunkSizeBytes { get; private set; } // Store in bytes for precision

        // Formatted string for the UI (e.g., "4.0 GB")
        public string JunkSizeDisplay => FormatBytes(CurrentJunkSizeBytes);

        // RPECK 06/03/2026 - Current Scan Info
        public Scan CurrentScan { get; private set; }
        public ScanProcessState CurrentState { get; private set; } = ScanProcessState.Ready;
        public double CurrentProgress { get; private set; } = 0;

        // Helps the ViewModel decide if the button should say "Pause" or "Resume"
        public bool IsPaused => _pauseTcs != null;

        // Events for the ViewModel to sync with the UI thread
        public event Action<double>? ProgressChanged;
        public event Action<ScanProcessState>? StateChanged;

        #region Control Methods

        public void Toggle()
        {
            if (CurrentState == ScanProcessState.Paused) Resume();
            else Pause();
        }

        public void Pause()
        {
            // We only pause if we are actively scanning
            if (_pauseTcs == null && CurrentState == ScanProcessState.Scanning)
            {
                _pauseTcs = new TaskCompletionSource<bool>();
                UpdateState(ScanProcessState.Paused);
            }
        }

        public void Resume()
        {
            if (_pauseTcs != null)
            {
                _pauseTcs.TrySetResult(true);
                _pauseTcs = null;
                UpdateState(ScanProcessState.Scanning);
            }
        }

        public void Stop()
        {
            // 1. Signal the cancellation token
            _cts?.Cancel();

            // 2. Release the pause gate so the thread can wake up and die
            _pauseTcs?.TrySetResult(true);
            _pauseTcs = null;

            // 3. Clean up
            _cts?.Dispose();
            _cts = null;

            UpdateState(ScanProcessState.Completed);
        }

        #endregion

        public Scan CreateNewScan(List<ScanItem> scanItems)
        {
            CurrentScan = new Scan();
            return CurrentScan;
        }

        public void StartScan(List<ScanItem> itemsToScan, int scanId)
        {
            // Prevent duplicate runs
            if (CurrentState == ScanProcessState.Scanning) return;

            _cts = new CancellationTokenSource();
            var token = _cts.Token;

            CreateNewScan(itemsToScan);
            UpdateState(ScanProcessState.Scanning);
            UpdateProgress(0);

            // Move work to background thread to keep UI smooth
            Task.Run(async () =>
            {
                try
                {
                    for (int i = 0; i <= 100; i += 2)
                    {
                        // Check if Stop() was called
                        token.ThrowIfCancellationRequested();

                        // Check if Pause() was called
                        if (_pauseTcs != null)
                        {
                            await _pauseTcs.Task;
                        }

                        // Simulate scanning work
                        await Task.Delay(50, token);

                        UpdateProgress(i);
                    }

                    UpdateState(ScanProcessState.Completed);
                }
                catch (OperationCanceledException)
                {
                    // User clicked Stop
                    UpdateState(ScanProcessState.Ready);
                }
                catch (Exception)
                {
                    UpdateState(ScanProcessState.Error);
                }
            }, token);
        }

        private string FormatBytes(long bytes)
        {
            string[] Suffix = { "B", "KB", "MB", "GB", "TB" };
            int i;
            double dblSByte = bytes;
            for (i = 0; i < Suffix.Length && bytes >= 1024; i++, bytes /= 1024)
            {
                dblSByte = bytes / 1024.0;
            }
            return $"{dblSByte:N1} {Suffix[i]}";
        }

        #region Helper Methods

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

        #endregion
    }
}