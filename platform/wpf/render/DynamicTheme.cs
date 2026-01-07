using System.Windows;
using System.Windows.Media;

namespace WpfApp1.render
{
    class DynamicTheme :IColorTheme
    {
        public Brush Get(ColorKey key)
        {
            return (Brush)Application.Current.Resources[key.ToString()];
        }
    }
}
