using System;
using System.Diagnostics;
using System.IO;
using System.Runtime.Versioning;
using System.Windows.Threading;

namespace FrontLineGUI.Include.Services
{
    [SupportedOSPlatform("windows")]
    public class HardwareService : PropertyChangedBase
    {
        // SDK Instance
        public static CPUIDSDK pSDK;

        // Fallback Counters
        private PerformanceCounter _cpuCounter;
        private PerformanceCounter _ramCounter;

        // Observable Properties for UI
        private int _cpuPower;
        private int _gpuPower;
        private int _ramPower;
        private int _hddSpace;

        public event Action HardwareUpdated;

        public HardwareService()
        {
            // 1. Initialize Fallbacks
            _cpuCounter = new PerformanceCounter("Processor", "% Processor Time", "_Total");
            _ramCounter = new PerformanceCounter("Memory", "% Committed Bytes In Use");

            // 2. Initialize CPUID SDK
            //Init_CPUID();

            // 3. Start Global Timer
            DispatcherTimer timer = new DispatcherTimer();
            timer.Interval = TimeSpan.FromSeconds(1);
            timer.Tick += (s, e) => UpdateValues();
            timer.Start();
        }

        public void UpdateValues()
        {
            // Update CPU & RAM via Performance Counters (or SDK if you prefer)
            CPUPower = Convert.ToInt32(_cpuCounter.NextValue());
            RAMPower = Convert.ToInt32(_ramCounter.NextValue());

            // Update HDD
            HDDSpace = GetDriveSpace();

            // Update GPU via CPUID SDK
            //UpdateGPUFromSDK();

            // Notify any listening ViewModels
            HardwareUpdated?.Invoke();
        }

        private void UpdateGPUFromSDK()
        {
            if (pSDK != null)
            {
                pSDK.RefreshInformation();
                // Note: You'll need to use the SDK's GetSensorValue or equivalent 
                // to find the GPU utilization index.
                // GPUPower = ... 
            }
        }

        #region Properties
        public int CPUPower { get => _cpuPower; set { _cpuPower = value; OnPropertyChanged(nameof(CPUPower)); } }
        public int GPUPower { get => _gpuPower; set { _gpuPower = value; OnPropertyChanged(nameof(GPUPower)); } }
        public int RAMPower { get => _ramPower; set { _ramPower = value; OnPropertyChanged(nameof(RAMPower)); } }
        public int HDDSpace { get => _hddSpace; set { _hddSpace = value; OnPropertyChanged(nameof(HDDSpace)); } }
        #endregion

        private int GetDriveSpace()
        {
            long totalHdd = 0;
            long availableHdd = 0;
            foreach (DriveInfo drive in DriveInfo.GetDrives())
            {
                if (drive.IsReady)
                {
                    availableHdd += drive.AvailableFreeSpace;
                    totalHdd += drive.TotalSize;
                }
            }
            return (availableHdd > 0 && totalHdd > 0)
                ? Convert.ToInt32(100 - (availableHdd / (float)totalHdd * 100))
                : 0;
        }

        // RPECK 25/03/2023
        // This is used to initialize the CPUID library in a separate thread
        private void Init_CPUID()
        {
            // RPECK 24/03/2023
            // CPUID
            bool res;
            int dll_version = 0;
            int error_code = 0, extended_error_code = 0;
            string error_message;

            pSDK = new CPUIDSDK();
            pSDK.CreateInstance();

            res = pSDK.Init(CPUIDSDK.szDllPath,
                            CPUIDSDK.szDllFilename,
                            CPUIDSDK.CPUIDSDK_CONFIG_USE_EVERYTHING,
                            ref error_code,
                            ref extended_error_code);

            if (error_code != CPUIDSDK.CPUIDSDK_ERROR_NO_ERROR)
            {

                //	Init failed, check errorcode
                switch ((uint)error_code)
                {
                    case CPUIDSDK.CPUIDSDK_ERROR_EVALUATION:
                        {
                            switch ((uint)extended_error_code)
                            {
                                case CPUIDSDK.CPUIDSDK_EXT_ERROR_EVAL_1:
                                    error_message = "You are running a trial version of the DLL SDK. In order to make it work, please run CPU-Z at the same time.";
                                    break;

                                case CPUIDSDK.CPUIDSDK_EXT_ERROR_EVAL_2:
                                    error_message = "Evaluation version has expired.";
                                    break;

                                default:
                                    error_message = "Eval version error " + extended_error_code;
                                    break;
                            }
                        }
                        break;

                    case CPUIDSDK.CPUIDSDK_ERROR_DRIVER:
                        error_message = "Driver error " + extended_error_code;
                        break;

                    case CPUIDSDK.CPUIDSDK_ERROR_VM_RUNNING:
                        error_message = "Virtual machine detected.";
                        break;

                    case CPUIDSDK.CPUIDSDK_ERROR_LOCKED:
                        error_message = "SDK mutex locked.";
                        break;

                    default:
                        error_message = "Error code 0x%X" + error_code;
                        break;
                }
                Debug.Write("CPUID Error - " + error_message);
            }

            if (res)
            {
                pSDK.GetDllVersion(ref dll_version);

            }

        }
    }
}