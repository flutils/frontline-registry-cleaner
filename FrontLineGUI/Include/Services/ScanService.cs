using FLCleanEngine;
using FrontLineGUI.Include.Classes.DB.Models;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text.RegularExpressions;
using System.Threading;
using System.Threading.Tasks;

namespace FrontLineGUI.Include.Services
{
    public enum ScanProcessState
    {
        Ready,
        Scanning,
        Paused,
        Stopped,
        Completed,
        Error
    }

    public class ScanService
    {
        // Engine & Threading
        private ManagedCleanEngine _engine;
        private CancellationTokenSource? _cts;
        private int _totalScannersToRun = 0;
        private int _scannersFinished = 0;

        // Observable Properties
        public ScanProcessState CurrentState { get; private set; } = ScanProcessState.Ready;
        public double CurrentProgress { get; private set; } = 0;
        public int CurrentErrorCount { get; private set; }
        public long CurrentJunkSizeBytes { get; private set; }
        public string JunkSizeDisplay => FormatBytes(CurrentJunkSizeBytes);

        // UI Notification Events
        public event Action<double>? ProgressChanged;
        public event Action<ScanProcessState>? StateChanged;
        public event Action<string, int, int>? ItemFound; // description, itemid, scannerid

        public ScanService()
        {
            _engine = new ManagedCleanEngine();
            WireLegacyEvents();
        }

        private void WireLegacyEvents()
        {
            ManagedCleanEngine.CENotifierItemFound += (desc, itemid, scannerid) =>
            {
                // Increment immediately for real-time counting
                CurrentErrorCount++;

                if (IsFileScanner(scannerid))
                {
                    CurrentJunkSizeBytes += ParseFileSizeFromDescription(desc, scannerid);
                }

                // Trigger event so ViewModel knows to call OnPropertyChanged("ErrorCount")
                ItemFound?.Invoke(desc, itemid, scannerid);
            };

            ManagedCleanEngine.CEScanFinished += (id) =>
            {
                _scannersFinished++;
                if (_totalScannersToRun > 0)
                {
                    double progress = ((double)_scannersFinished / _totalScannersToRun) * 100;
                    UpdateProgress(progress);
                }
            };
        }

        #region Control Methods

        public async void StartScan(List<ScanItem> selectedItems)
        {
            // Efficiency: Only allow Start if we are in a 'Ready' or 'Completed' state
            if(CurrentState != ScanProcessState.Ready) return;

            // 1. Extract the unique IDs from the selected items
            var allIdsToScan = selectedItems
                    .Where(x => x.scancodes != null)
                    .SelectMany(x => x.scancodes)
                    .Select(code => int.Parse(code.ToString())) // Ensure it's an int
                    .Distinct()
                    .ToList();

            _totalScannersToRun = allIdsToScan.Count;

            ResetStats();
            UpdateState(ScanProcessState.Scanning);
            _cts = new CancellationTokenSource();

            // 2. Offload to background thread
            await Task.Run(() =>
            {
                try
                {
                    // 3. Inject IDs into the engine
                    // Note: Assuming the method is EnableScanner(int id). 
                    // If the engine has a bulk method like SetScanners(List<int>), use that instead.
                    foreach (var id in allIdsToScan)
                    {
                        _engine.EnableScanner(id, true);
                    }

                    // 4. Start the engine loop
                    _engine.Start();


                }
                catch (Exception)
                {
                    UpdateState(ScanProcessState.Error);
                }
                finally
                {
                    _cts?.Dispose();
                    _cts = null;
                }
            }, _cts.Token);
        }

        public void Toggle()
        {
            if (CurrentState == ScanProcessState.Paused) Resume();
            else if (CurrentState == ScanProcessState.Scanning) Pause();
        }

        public void Pause()
        {
            if (CurrentState != ScanProcessState.Scanning) return;
            _engine.Pause(1);
            UpdateState(ScanProcessState.Paused);
        }

        public void Resume()
        {
            if (CurrentState != ScanProcessState.Paused) return;
            _engine.Pause(0);
            UpdateState(ScanProcessState.Scanning);
        }

        public void Stop()
        {
            if (CurrentState == ScanProcessState.Ready || CurrentState == ScanProcessState.Completed || CurrentState == ScanProcessState.Stopped) return;

            _cts?.Cancel();
            _engine.Stop();
            UpdateState(ScanProcessState.Stopped);
        }

        #endregion

        #region Legacy Logic Ported (Regex & Win32)

        private bool IsFileScanner(int id)
        {
            // The specific IDs from your old system that indicate file-based junk
            int[] fileScannerIds = { 20403, 20404, 20405, 20406, 20407, 20501, 20502, 20503, 20504, 20505, 20506, 20507 };
            return fileScannerIds.Contains(id);
        }

        private long ParseFileSizeFromDescription(string descr, int id)
        {
            // File size extraction logic using Regex from old ScanningPanel
            Regex expFile = new Regex("^file \\((.*?)\\) at directory \\((.*?)\\)$", RegexOptions.IgnoreCase);
            if (expFile.IsMatch(descr))
            {
                var col = expFile.Split(descr);
                var path = Path.Combine(col[2], col[1]);
                try
                {
                    FileInfo fileInfo = new FileInfo(path);
                    return fileInfo.Exists ? fileInfo.Length : 0;
                }
                catch { return 0; }
            }

            // Handle Recycle Bin specifically via Shell32
            if (id == (int)CEScannerID.RECYCLEBIN_SCANNER_ID)
            {
                SHQUERYRBINFO query = new SHQUERYRBINFO();
                query.cbSize = Marshal.SizeOf(typeof(SHQUERYRBINFO));
                if (SHQueryRecycleBin(null, ref query) == 0)
                {
                    return (long)query.i64Size;
                }
            }
            return 0;
        }

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode, Pack = 1)]
        public struct SHQUERYRBINFO
        {
            public Int32 cbSize;
            public UInt64 i64Size;
            public UInt64 i64NumItems;
        }

        [DllImport("shell32.dll", CharSet = CharSet.Unicode)]
        public static extern int SHQueryRecycleBin(string pszRootPath, ref SHQUERYRBINFO pSHQueryRBInfo);

        #endregion

        #region Helper Methods

        private void ResetStats()
        {
            CurrentErrorCount = 0;
            CurrentJunkSizeBytes = 0;
            CurrentProgress = 0;
            _scannersFinished = 0;
        }

        private void UpdateState(ScanProcessState newState)
        {
            CurrentState = newState;
            StateChanged?.Invoke(newState);
        }

        private void UpdateProgress(double progress)
        {
            CurrentProgress = Math.Clamp(progress, 0, 100);
            ProgressChanged?.Invoke(CurrentProgress);
        }

        private string FormatBytes(long bytes)
        {
            string[] Suffix = { "B", "KB", "MB", "GB", "TB" };
            int i = 0;
            double dblSByte = bytes;
            while (dblSByte >= 1024 && i < Suffix.Length - 1)
            {
                i++;
                dblSByte /= 1024;
            }
            return $"{dblSByte:N1}{Suffix[i]}";
        }

        #endregion
    }

}