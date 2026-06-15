using ChatServer_UI.ViewModels;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace ChatServer_UI.ViewModels
{
    /// <summary>
    /// Interaction logic for ChatView.xaml
    /// </summary>
    public partial class ChatViewModel : ObservableObject
    {
        [ObservableProperty]
        private string _headerName = string.Empty;

        [ObservableProperty]
        private string _draftMessage = string.Empty;

        public ObservableCollection<MessageViewModel> Messages { get; } = new();

        [RelayCommand]
        private void Send()
        {
            if (string.IsNullOrWhiteSpace(DraftMessage) || _currentConversation is null) return;

            var message = new MessageViewModel
            {
                Content = DraftMessage,
                IsSentByMe = true,
                Timestamp = DateTime.Now
            };

            _currentConversation.Messages.Add(message);
            _currentConversation.LastMessagePreview = DraftMessage;
            Messages.Add(message);
            DraftMessage = string.Empty;
        }

        public void LoadConversation(ConversationViewModel conversation)
        {
            _currentConversation = conversation;
            HeaderName = conversation.DisplayName;
            Messages.Clear();
            foreach (var msg in conversation.Messages)
                Messages.Add(msg);
        }

        private ConversationViewModel? _currentConversation;
    }
}
