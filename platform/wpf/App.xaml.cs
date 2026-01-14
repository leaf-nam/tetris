using System.Configuration;
using System.Data;
using System.Windows;
using wpf.network;

namespace wpf
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        public static NetworkService Network { get; } = new NetworkService();

    }

}
