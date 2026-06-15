using ChatServer_UI.ViewModels;
using System.Windows;
using System.Windows.Controls;

namespace ChatServer_UI.Controls
{
    public class MessageTemplateSelector : DataTemplateSelector
    {
        public DataTemplate? SentTemplate { get; set; }
        public DataTemplate? ReceivedTemplate { get; set; }

        public override DataTemplate? SelectTemplate(object item, DependencyObject container)
        {
            if (item is MessageViewModel msg)
                return msg.IsSentByMe ? SentTemplate : ReceivedTemplate;
            return base.SelectTemplate(item, container);
        }
    }
}