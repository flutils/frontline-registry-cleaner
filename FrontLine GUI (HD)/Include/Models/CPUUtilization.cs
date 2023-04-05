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
    public class CPUUtilization : PropertyChangedBase
    {

        // Vars
        private int cpu_power = 0;
        private int gpu_power = 0;
        private int ram_power = 0;
        private int hdd_power = 0;

        // Constructor
        public CPUUtilization()
        {}

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
                OnPropertyChanged("RAMPower");
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

    }

}
