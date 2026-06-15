using System;
using System.Collections.Generic;
using System.Text;

using CommunityToolkit.Mvvm.ComponentModel;

namespace ChatServer_UI.ViewModels
{
    public partial class MessageViewModel : ObservableObject
    {
        [ObservableProperty]
        private string _content = string.Empty;

        [ObservableProperty]
        private bool _isSentByMe;

        [ObservableProperty]
        private DateTime _timestamp;
    }
}
