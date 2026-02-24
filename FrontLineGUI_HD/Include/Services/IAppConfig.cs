using System;
using System.Collections.Generic;
using System.Text;

namespace FrontLineGUI.Include.Services
{
    public interface IAppConfig
    {
        string Debug { get; }

        void Reload();
    }

}
