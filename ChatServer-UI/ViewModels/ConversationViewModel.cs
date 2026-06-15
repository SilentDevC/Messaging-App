using CommunityToolkit.Mvvm.ComponentModel;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Text;

namespace ChatServer_UI.ViewModels
{
    public partial class ConversationViewModel : ObservableObject
    {
        [ObservableProperty]
        private string _displayName = string.Empty;

        [ObservableProperty]
        private string _lastMessagePreview = string.Empty;

        [ObservableProperty]
        private string _avatarUrl = string.Empty;

        [ObservableProperty]
        private int _unreadCount;

        [ObservableProperty]
        private DateTime _lastMessageTime;

        public ObservableCollection<MessageViewModel> Messages { get; } = new();
    }
}
