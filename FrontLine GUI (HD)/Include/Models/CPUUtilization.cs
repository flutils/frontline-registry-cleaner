using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Data;
using System.Xml.Linq;

namespace FrontLineGUI
{

    // RPECK 26/03/2023
    // This is used to give us the means to dynamically manage the CPU performance for the 4 parts of the system
    public class CPUUtilization : INotifyPropertyChanged
    {

        // Vars
        private int cpu_power;
        private int gpu_power;
        private int ram_power;
        private int hdd_power;

        // Constructor
        public CPUUtilization(int cpu_power, int ram_power, int hdd_power, int gpu_power)
        {
            this.cpu_power = cpu_power;
            this.gpu_power = gpu_power;
            this.ram_power = ram_power;
            this.hdd_power = hdd_power;
        }

        // Class methods
        public event PropertyChangedEventHandler PropertyChanged;

        // Class methods
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
                OnPropertyChanged(null);
            }
        }

        public int HDDPower
        {
            get { return hdd_power; }
            set {
                hdd_power = value;
                OnPropertyChanged("HDDPower");
            }
        }

        // Create the OnPropertyChanged method to raise the event
        // The calling member's name will be used as the parameter.
        protected void OnPropertyChanged([CallerMemberName] string name = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(name));
        }
    }

}
