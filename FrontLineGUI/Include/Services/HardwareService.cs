using System;
using System.Diagnostics;
using System.Linq;
using System.Windows.Threading;
using LibreHardwareMonitor.Hardware;

namespace FrontLineGUI.Include.Services
{
    // ==========================================
    // The Visitor: Handles the "How to Update"
    // ==========================================
    public class UpdateVisitor : IVisitor
    {
        public void VisitComputer(IComputer computer) => computer.Traverse(this);

        public void VisitHardware(IHardware hardware)
        {
            hardware.Update();
            foreach (IHardware subHardware in hardware.SubHardware)
                subHardware.Accept(this);
        }

        public void VisitSensor(ISensor sensor) { }
        public void VisitParameter(IParameter parameter) { }
    }

    // ==========================================
    // The Service: Handles the Data & UI Logic
    // ==========================================
    public class HardwareService : PropertyChangedBase
    {
        private readonly Computer _computer;
        private readonly UpdateVisitor _updateVisitor = new UpdateVisitor();

        // Static Info
        public OSInfo OSInformation => OSInfo.Default;
        public string CPUName { get; private set; }
        public string GPUName { get; private set; }
        public string RAMTotal { get; private set; }
        public string ComputerName { get; private set; }

        // Live Metrics
        private int _cpuUsage;
        private int _gpuUsage;
        private int _ramUsage;

        public int CPUUsage { get => _cpuUsage; set { _cpuUsage = value; OnPropertyChanged(nameof(CPUUsage)); } }
        public int GPUUsage { get => _gpuUsage; set { _gpuUsage = value; OnPropertyChanged(nameof(GPUUsage)); } }
        public int RAMUsage { get => _ramUsage; set { _ramUsage = value; OnPropertyChanged(nameof(RAMUsage)); } }

        public event Action HardwareUpdated;

        public HardwareService()
        {
            ComputerName = Environment.MachineName;

            _computer = new Computer
            {
                IsCpuEnabled = true,
                IsGpuEnabled = true,
                IsMemoryEnabled = true
            };

            _computer.Open();

            // Run an initial update so names and totals are populated immediately
            _computer.Accept(_updateVisitor);
            InitStaticInfo();

            var timer = new DispatcherTimer
            {
                Interval = TimeSpan.FromSeconds(1)
            };
            timer.Tick += (s, e) => UpdateSensors();
            timer.Start();
        }

        private void InitStaticInfo()
        {
            foreach (var hw in _computer.Hardware)
            {
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
                        var totalSensor = hw.Sensors.FirstOrDefault(s => s.SensorType == SensorType.Data && s.Name.Contains("Memory"));
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

        private void UpdateSensors()
        {
            // The Visitor recursively updates all components
            _computer.Accept(_updateVisitor);

            foreach (var hw in _computer.Hardware)
            {
                foreach (var sensor in hw.Sensors)
                {
                    ProcessSensorValue(hw, sensor);
                }
            }

            HardwareUpdated?.Invoke();
        }

        private void ProcessSensorValue(IHardware hw, ISensor sensor)
        {
            float value = sensor.Value ?? 0;

            switch (sensor.SensorType)
            {
                case SensorType.Load:
                    if (hw.HardwareType == HardwareType.Cpu && sensor.Name == "CPU Total")
                        CPUUsage = (int)Math.Round(value);

                    if (IsGpu(hw.HardwareType) && sensor.Name.Contains("Core"))
                        GPUUsage = (int)Math.Round(value);
                    break;

                case SensorType.Data:
                    if (hw.HardwareType == HardwareType.Memory && sensor.Name.Contains("Used"))
                        RAMUsage = (int)Math.Round(value);
                    break;
            }
        }

        private bool IsGpu(HardwareType type) =>
            type == HardwareType.GpuNvidia || type == HardwareType.GpuAmd || type == HardwareType.GpuIntel;
    }
}