using System;
using System.Collections.Generic;
using System.Text;

namespace FrontLineGUI.Include.Services
{
    public interface INavigationService
    {

        void NavigateTo<TViewModel>() where TViewModel : ViewModelBase;

        void NavigateTo(Type viewModelType);

        void Configure(Action<ViewModelBase> setCurrentViewModel);
    }
}