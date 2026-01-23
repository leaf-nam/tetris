using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using System.Windows.Media;

namespace wpf.render.theme
{
    public sealed class DraculaTheme : IColorTheme
    {
        private static readonly Dictionary<Color, string> colors =
            new()
            {
                [Color.Background] = "#282A36",
                [Color.Panel] = "#44475A",
                [Color.Foreground] = "#F8F8F2",
                [Color.Comment] = "#6272A4",

                [Color.Cyan] = "#8BE9FD",
                [Color.Green] = "#50FA7B",
                [Color.Orange] = "#FFB86C",
                [Color.Pink] = "#FF79C6",
                [Color.Purple] = "#BD93F9",
                [Color.Red] = "#FF5555",
                [Color.Yellow] = "#F1FA8C",

                [Color.Accent1] = "#F1FA8C",
                [Color.Accent2] = "#FFD866",
                [Color.Accent3] = "#FFF59D",
                [Color.Accent4] = "#E6DB74",
                [Color.Accent5] = "#FFCC66",
                [Color.Accent6] = "#FAF3C0",
                [Color.Accent7] = "#EAFD5E",
            };

        public Brush Get(Color key) => (Brush)new BrushConverter().ConvertFromString(colors[key]);

    }
}
