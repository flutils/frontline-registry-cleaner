using System;

namespace FrontLineGUI.Include.Interfaces
{
    public interface INavigationService
    {

        void NavigateTo<TViewModel>() where TViewModel : ViewModelBase;

        void NavigateTo(Type viewModelType);

        void Configure(Action<ViewModelBase> setCurrentViewModel);
    }
}