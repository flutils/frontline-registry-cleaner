using Microsoft.Extensions.DependencyInjection;
using System;
using FrontLineGUI.Include.Interfaces;

namespace FrontLineGUI.Include.Services
{
    public class NavigationService : INavigationService
    {
        private readonly IServiceProvider _serviceProvider;
        private Action<ViewModelBase>? _setCurrentViewModel;

        public NavigationService(IServiceProvider serviceProvider)
        {
            _serviceProvider = serviceProvider;
        }

        public void Configure(Action<ViewModelBase> setCurrentViewModel)
        {
            _setCurrentViewModel = setCurrentViewModel;
        }

        public void NavigateTo<TViewModel>() where TViewModel : ViewModelBase
        {
            var vm = _serviceProvider.GetRequiredService<TViewModel>();
            _setCurrentViewModel?.Invoke(vm);
        }

        public void NavigateTo(Type viewModelType, object parameter)
        {
            var vm = (ViewModelBase)_serviceProvider.GetRequiredService(viewModelType);

            vm.OnNavigatedTo(parameter);

            _setCurrentViewModel?.Invoke(vm);
        }
    }

}
