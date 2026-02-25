using System;

// RPECK 24/02/2026 - NavigationItem
// Added per recommendation to provide a simple wrapper for the navigation buttons in the top bar
namespace FrontLineGUI.Include.Classes
{
    public class NavigationItem
    {
        public string Title { get; }
        public Type ViewModelType { get; }

        public NavigationItem(string title, Type viewModelType)
        {
            Title = title;
            ViewModelType = viewModelType;
        }

    }
}
