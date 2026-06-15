using System;
using System.Collections.Generic;
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
using ChatServer_UI.ViewModels;

namespace ChatServer_UI.Controls
{
    public partial class ConversationListView : UserControl
    {
        private CollectionViewSource? _viewSource;

        public ConversationListView()
        {
            InitializeComponent();
            DataContextChanged += OnDataContextChanged;
        }

        private void OnDataContextChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            _viewSource = (CollectionViewSource)Resources["FilteredConversations"];
            _viewSource.Filter += OnFilter;

            if (e.NewValue is ConversationListViewModel vm)
                vm.PropertyChanged += (_, args) =>
                {
                    if (args.PropertyName == nameof(ConversationListViewModel.SearchQuery))
                        _viewSource.View.Refresh();
                };
        }

        private void OnFilter(object sender, FilterEventArgs e)
        {
            if (DataContext is not ConversationListViewModel vm) return;
            if (string.IsNullOrWhiteSpace(vm.SearchQuery)) { e.Accepted = true; return; }

            e.Accepted = e.Item is ConversationViewModel conv &&
                         (conv.DisplayName.Contains(vm.SearchQuery, StringComparison.OrdinalIgnoreCase) ||
                         conv.Messages.Any(m => m.MessageContent.Contains(vm.SearchQuery, StringComparison.OrdinalIgnoreCase)));
        }
    }
}
