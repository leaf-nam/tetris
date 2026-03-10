using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;

namespace wpf.render.theme
{
    class CustomColors
    {

        public static IColorTheme Theme { get; private set; }

        public static void SetTheme(IColorTheme theme) => Theme = theme;

    }

}
