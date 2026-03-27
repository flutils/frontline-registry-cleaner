using FLCleanEngine;
using FrontLineGUI.Include.Classes.DB.Models;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
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
        public bool IsScanning => CurrentState == ScanProcessState.Scanning || CurrentState == ScanProcessState.Paused;
        public ObservableCollection<ScanResult> ScannerResultsCollection { get; set; } = new ObservableCollection<ScanResult>();
        public double CurrentProgress { get; private set; } = 0;
        public int CurrentErrorCount => ScannerResultsCollection.Sum(x => x.Count);
        public long CurrentJunkSizeBytes => ScannerResultsCollection.Sum(x => x.TotalBytes);
        public string JunkSizeDisplay => FormatBytes(CurrentJunkSizeBytes);
        public string CurrentScanningPath { get; private set; }

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
            // FIX: Static events persist for the life of the app. 
            // We clear previous handlers to prevent double-counting.
            ManagedCleanEngine.CENotifierItemFound -= OnEngineItemFound;
            ManagedCleanEngine.CENotifierItemFound += OnEngineItemFound;

            ManagedCleanEngine.CEScanFinished -= OnEngineScannerFinished;
            ManagedCleanEngine.CEScanFinished += OnEngineScannerFinished;
        }

        private void OnEngineItemFound(string desc, int itemid, int scannerid)
        {
            System.Windows.Application.Current.Dispatcher.BeginInvoke(() =>
            {

                // Only update if the engine actually gave us a path
                if (!string.IsNullOrEmpty(desc)) CurrentScanningPath = desc;

                // 2. Find the specific UI row (ScanResult) that matches this engine ID
                // We look inside the ScanItem's scancodes list for the match
                var result = ScannerResultsCollection.FirstOrDefault(r => r.ScanItem.scancodes != null &&
                                                                          r.ScanItem.scancodes.Contains(scannerid.ToString()));

                if (result != null)
                {
                    // 3. Increment the count for this specific category (e.g., "Recycle Bin")
                    result.Count++;

                    // 4. If it's a file scanner, calculate and add the size for this row
                    if (IsFileScanner(scannerid))
                    {
                        result.TotalBytes += ParseFileSizeFromDescription(desc, scannerid);
                    }
                }

                // 5. Keep the event trigger in case other parts of the app are listening
                ItemFound?.Invoke(desc, itemid, scannerid);
            });
        }

        private void OnEngineScannerFinished(int id)
        {
            System.Windows.Application.Current.Dispatcher.BeginInvoke(() =>
            {
                _scannersFinished++;
                if (_totalScannersToRun > 0)
                {
                    double progress = ((double)_scannersFinished / _totalScannersToRun) * 100;
                    UpdateProgress(progress);
                }
            });
        }

        public async void StartScan(List<ScanItem> selectedItems)
        {
            if (CurrentState == ScanProcessState.Scanning) return;

            ScannerResultsCollection.Clear();
            foreach (var item in selectedItems)
            {
                // Add the row to the 3-column UI with 0 counts
                ScannerResultsCollection.Add(new ScanResult
                {
                    ScanItem = item,
                    ScanItemId = item.ScanItemId,
                    Count = 0,
                    TotalBytes = 0
                });
            }

            var allIdsToScan = selectedItems
                    .Where(x => x.scancodes != null)
                    .SelectMany(x => x.scancodes)
                    .Select(code => int.Parse(code.ToString()))
                    .Distinct()
                    .ToList();

            _totalScannersToRun = allIdsToScan.Count;
            ResetStats();
            UpdateState(ScanProcessState.Scanning);
            _cts = new CancellationTokenSource();

            await Task.Run(() =>
            {
                try
                {
                    _engine.EnableAllScanners(false); // Clean slate
                    foreach (var id in allIdsToScan)
                    {
                        _engine.EnableScanner(id, true);
                    }

                    // This call usually BLOCKS until the entire scan is done
                    _engine.Start();

                }
                catch (Exception)
                {
                    UpdateState(ScanProcessState.Error);
                }
            }, _cts.Token);
        }

        #region Methods

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

        public void Reset()
        {

            // 1. Tell the Engine to stop and clear its internal scanner list
            _engine.Stop();
            _engine.EnableAllScanners(false);

            // 2. Clear the UI Path & Progress
            CurrentProgress = 0;
            _scannersFinished = 0;

            // 3. Zero out the data in your ObservableCollection
            // This is what makes the red numbers in your screenshot turn back to 0
            foreach (var result in ScannerResultsCollection)
            {
                result.Count = 0;
                result.TotalBytes = 0;
            }

            // 4. Update the State
            UpdateState(ScanProcessState.Ready);

            // 5. Tell WPF to re-calculate the "Big Totals" at the top
            // Without these, the "4.0GB" will stay stuck on screen

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

        public static string FormatBytes(long bytes)
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