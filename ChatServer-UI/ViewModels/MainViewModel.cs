
using CommunityToolkit.Mvvm.ComponentModel;
using ChatServer_UI.Controls;
using ChatServer_UI;
namespace ChatServer_UI.ViewModels
{
    public sealed class MainViewModel : ObservableObject
    {
        public ConversationListViewModel ConversationList { get; }
        public ChatViewModel ActiveChat { get; }

        public MainViewModel(ConversationListViewModel convList, ChatViewModel chat)
        {
            ConversationList = convList;
            ActiveChat = chat;
            ConversationList.ConversationSelected += chat.LoadConversation;
        }
    }

}