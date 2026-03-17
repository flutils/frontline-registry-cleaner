using System;
using System.Diagnostics;
using System.Linq;
using System.Windows.Threading;
using LibreHardwareMonitor.Hardware;

namespace FrontLineGUI.Include.Services
{
    public class HardwareService : PropertyChangedBase
    {
        private readonly Computer _computer;

        // ========================
        // Static Info
        // ========================
        public OSInfo OSInformation => OSInfo.Default;
        public string CPUName { get; private set; }
        public string GPUName { get; private set; }
        public string RAMTotal { get; private set; }
        public string ComputerName { get; private set; }

        // ========================
        // Live Metrics
        // ========================
        private int _cpuUsage;
        private int _gpuUsage;
        private int _ramUsage;

        public int CPUUsage { get => _cpuUsage; set { _cpuUsage = value; OnPropertyChanged(nameof(CPUUsage)); } }
        public int GPUUsage { get => _gpuUsage; set { _gpuUsage = value; OnPropertyChanged(nameof(GPUUsage)); } }
        public int RAMUsage { get => _ramUsage; set { _ramUsage = value; OnPropertyChanged(nameof(RAMUsage)); } }

        public event Action HardwareUpdated;

        // ========================
        // Constructor
        // ========================
        public HardwareService()
        {
            ComputerName = Environment.MachineName;
            Debug.WriteLine(ComputerName);

            _computer = new Computer
            {
                IsCpuEnabled = true,
                IsGpuEnabled = true,
                IsMemoryEnabled = true
            };

            _computer.Open();

            InitStaticInfo();

            var timer = new DispatcherTimer
            {
                Interval = TimeSpan.FromSeconds(1)
            };
            timer.Tick += (s, e) => UpdateSensors();
            timer.Start();
        }

        // ========================
        // Static Info
        // ========================
        private void InitStaticInfo()
        {
            foreach (var hw in _computer.Hardware)
            {
                hw.Update();

                switch (hw.HardwareType)
                {
                    case HardwareType.Cpu:
                        CPUName = hw.Name;
                        break;

                    case HardwareType.GpuNvidia:
                    case HardwareType.GpuAmd:
                    case HardwareType.GpuIntel:
                        GPUName ??= hw.Name;
                        break;

                    case HardwareType.Memory:
                        var totalSensor = hw.Sensors.FirstOrDefault(s => s.Name.Contains("Memory"));
                        if (totalSensor != null)
                            RAMTotal = $"{Math.Round(totalSensor.Value ?? 0)} GB";
                        break;
                }
            }

            OnPropertyChanged(nameof(CPUName));
            OnPropertyChanged(nameof(GPUName));
            OnPropertyChanged(nameof(RAMTotal));
            OnPropertyChanged(nameof(ComputerName));
        }

        // ========================
        // Live Updates
        // ========================
        private void UpdateSensors()
        {
            foreach (var hw in _computer.Hardware)
            {
                hw.Update();

                foreach (var sensor in hw.Sensors)
                {
                    switch (sensor.SensorType)
                    {
                        case SensorType.Load:
                            if (hw.HardwareType == HardwareType.Cpu && sensor.Name == "CPU Total")
                                CPUUsage = (int)Math.Round(sensor.Value ?? 0);

                            if ((hw.HardwareType == HardwareType.GpuNvidia ||
                                 hw.HardwareType == HardwareType.GpuAmd ||
                                 hw.HardwareType == HardwareType.GpuIntel)
                                && sensor.Name.Contains("Core"))
                                GPUUsage = (int)Math.Round(sensor.Value ?? 0);
                            break;

                        case SensorType.Data:
                            if (hw.HardwareType == HardwareType.Memory && sensor.Name.Contains("Used"))
                                RAMUsage = (int)Math.Round(sensor.Value ?? 0);
                            break;
                    }
                }
            }

            HardwareUpdated?.Invoke();
        }
    }
}