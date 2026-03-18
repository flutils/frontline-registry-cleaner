using System;

namespace FrontLineGUI.Include.Interfaces
{
    public interface INavigationService
    {

        void NavigateTo<TViewModel>() where TViewModel : ViewModelBase;

        void NavigateTo(Type viewModelType, object parameter = null);


        void Configure(Action<ViewModelBase> setCurrentViewModel);
    }
}