using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace FrontLineGUI
{
    public class IDToDescript
    {
        public int ID
        {
            get;
            set;
        }

        public String Description
        {
            get;
            set;
        }

        public bool Selected = true;
    }

    public class CustomScanner
    {
        public String Name
        {
            get;
            set;
        }

        public String IDs
        {
            get;
            set;
        }

        public Dictionary<IDToDescript, long> FoundItems
        {
            get;
            set;
        }

        public bool IsFileScanner()
        {
            string[] files_scanner_ids = { "20403", "20404", "20405", "20406", "20407"
                                         , "20501", "20502", "20503", "20504", "20505"
                                         , "20506", "20507" };
            foreach(var id in IDs.Split(';'))
                if (!String.IsNullOrEmpty(id) && files_scanner_ids.Contains(id))
                    return true;
            return false;
        }

        public long JunksSizeTotal
        {
            get
            {
                if (IsFileScanner())
                {
                    long sum = 0;
                    foreach (var item in FoundItems)
                    {
                        if (item.Key.Selected)
                        {
                            sum += item.Value;
                        }
                    }
                    return sum;
                }
                return 0;
            }
            private set
            {
                Console.WriteLine(value);
            }
        }
        
    }
}
