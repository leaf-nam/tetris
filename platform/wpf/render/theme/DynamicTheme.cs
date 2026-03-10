using System.Windows;
using System.Windows.Media;

namespace wpf.render.theme
{
    class DynamicTheme :IColorTheme
    {
        public Brush Get(Color key)
        {
            return (Brush)Application.Current.Resources[key.ToString()];
        }
    }
}
