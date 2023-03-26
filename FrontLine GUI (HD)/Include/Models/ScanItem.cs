using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FrontLineGUI
{
    // RPECK 26/03/2023
    // ScanItem class for the 'scan' page -- allows us to populate a datagrid without having to define the items manually
    // Underlying benefit is that it gives us the means to extend the underlying scan feature as needed
    public class ScanItem
    {
        // Internal values
        private string firstName;
        private string lastName;

        // Constructor
        public ScanItem(string first, string last)
        {
            this.firstName = first;
            this.lastName = last;
        }

        public string FirstName
        {
            get { return firstName; }
            set { firstName = value; }
        }

        public string LastName
        {
            get { return lastName; }
            set { lastName = value; }
        }

    }

}
