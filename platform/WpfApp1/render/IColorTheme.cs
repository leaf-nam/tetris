using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;

namespace WpfApp1.render
{
    public interface IColorTheme
    {
        Brush Get(ColorKey key);
    }
}
