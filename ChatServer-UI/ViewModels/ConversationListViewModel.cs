using CommunityToolkit.Mvvm.ComponentModel;
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
    /// Interaction logic for ConversationListView.xaml
    /// </summary>
    public partial class ConversationListViewModel : ObservableObject
    {
        public ObservableCollection<ConversationViewModel> Conversations { get; } =
        [
            new ConversationViewModel { DisplayName = "Alice", LastMessagePreview = string.Empty, LastMessageTime = DateTime.Now },
            new ConversationViewModel { DisplayName = "Bob", LastMessagePreview = string.Empty, LastMessageTime = DateTime.Now},
    ];

        [ObservableProperty]
        private ConversationViewModel? _selectedConversation;

        [ObservableProperty]
        private string _searchQuery = string.Empty;

        // CommunityToolkit generates OnSelectedConversationChanged automatically
        partial void OnSelectedConversationChanged(ConversationViewModel? value)
        {
            if (value is not null)
                ConversationSelected?.Invoke(value);
        }

        public event Action<ConversationViewModel>? ConversationSelected;
    }
}
