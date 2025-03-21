using JCS;

namespace FrontLineGUI
{
    public class OSInfo
    {

        // RPECK 08/02/2025 - Constructor
        // Used to populate instance variables inside the class
        public OSInfo() { }

        #region Methods

        // RPECK 08/02/2025 - OS Version
        // Get the version of the OS using the "OSVersionInfo.VersionString.ToString()" value provided to us
        public string OSVersion
        {
            get { return OSVersionInfo.VersionString.ToString(); }
        }

        // RPECK 05/04/2023 - OS Name
        // This is a public method used to give us the means to access information about the OS Name (IE Windows X)
        public string OSName
        {
            get
            {
                // Check to see if the name needs to be changed
                if (OSVersionInfo.MajorVersion == 10 && OSVersionInfo.BuildVersion >= 22000) return "Windows 11";

                // Default
                return OSVersionInfo.Name;
            }
        }

        // RPECK 05/04/2023 - OS Icon
        // Shows the image at the bottom of the page (next to the OSVersion)
        public string OSIcon
        {
            get
            {

                // OSVersion
                // Get the version of the OS from the above method and then extract the version of Windows from it
                // https://stackoverflow.com/a/5651002/1143732
                return "/Resources/OS/" + OSName.Replace(" ", "-").ToLower() + ".png";

            }
        }

        #endregion

    }
}
