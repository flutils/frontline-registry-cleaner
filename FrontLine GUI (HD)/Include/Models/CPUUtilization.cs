using System;
using System.Diagnostics;
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

        // Constructor
        public CPUUtilization()
        {
            cpuCounter = new PerformanceCounter("Processor", "% Processor Time", "_Total");
            ramCounter = new PerformanceCounter("Memory", "% Committed Bytes In Use");
            //hddCounter = new PerformanceCounter("PhysicalDisk", "FreeMegabytes", "_Total");
        }

        // Methods
        public void UpdateValues()
        {
            CPUPower = Convert.ToInt32(cpuCounter.NextValue());
            RAMPower = Convert.ToInt32(ramCounter.NextValue());
            //HDDSpace = Convert.ToInt32(hddCounter.NextValue());

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

    }

}
