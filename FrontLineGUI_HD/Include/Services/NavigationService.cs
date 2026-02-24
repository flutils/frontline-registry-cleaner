using FrontLineGUI;
using Microsoft.Extensions.DependencyInjection;
using System;
using System.Collections.Generic;
using System.Text;

namespace FrontLineGUI.Include.Services
{
    public class NavigationService : INavigationService
    {
        private readonly IServiceProvider _provider;
        private Action<ViewModelBase> _setCurrentViewModel;

        public NavigationService(IServiceProvider provider)
        {
            _provider = provider;
        }

        // Called once after MainWindowViewModel is created
        public void Configure(Action<ViewModelBase> setCurrentViewModel)
        {
            _setCurrentViewModel = setCurrentViewModel;
        }

        public void NavigateTo<TViewModel>() where TViewModel : ViewModelBase
        {
            var vm = _provider.GetRequiredService<TViewModel>();
            _setCurrentViewModel?.Invoke(vm);
        }
    }

}
