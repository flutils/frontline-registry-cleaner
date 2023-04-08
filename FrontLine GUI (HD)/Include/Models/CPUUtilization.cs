using System;
using System.Diagnostics;
using System.IO;
using System.Runtime.Versioning;

namespace FrontLineGUI
{

    // RPECK 26/03/2023
    // This is used to give us the means to dynamically manage the CPU performance for the 4 parts of the system

    // RPECK 08/04/2023 - this was required to limit the application to Windows OS versions only
    [SupportedOSPlatform("windows")]
    public class CPUUtilization : PropertyChangedBase
    {

        // Vars
        private int cpu_power = 0;
        private int gpu_power = 0;
        private int ram_power = 0;
        private int hdd_space = 0;

        // PerformanceContainers
        PerformanceCounter cpuCounter;
        PerformanceCounter ramCounter;
        PerformanceCounter hddCounter;
        PerformanceCounter gpuCounter;

        // CPUID
        public static CPUIDSDK pSDK;

        // Constructor
        public CPUUtilization()
        {
            // CPU & RAM (use PerformanceCounter)
            cpuCounter = new PerformanceCounter("Processor", "% Processor Time", "_Total");
            ramCounter = new PerformanceCounter("Memory", "% Committed Bytes In Use");
        }

        // Methods
        public void UpdateValues()
        {
            CPUPower = Convert.ToInt32(cpuCounter.NextValue());
            RAMPower = Convert.ToInt32(ramCounter.NextValue());
        }

        // Properties
        public int CPUPower
        {
            get { return cpu_power; }
            set {
                cpu_power = value;
                OnPropertyChanged("CPUPower");
            }
        }
        public int GPUPower
        {
            get { return gpu_power; }
            set {
                gpu_power = value;
                OnPropertyChanged("GPUPower");
            }
        }

        public int RAMPower
        {
            get { return ram_power; }
            set {
                ram_power = value;
                OnPropertyChanged("RAMPower");
            }
        }

        public int HDDSpace
        {
            get { return hdd_space; }
            set {
                hdd_space = value;
                OnPropertyChanged("HDDSpace");
            }
        }

        // Private Methods

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
