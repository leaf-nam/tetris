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
        private static readonly Dictionary<ColorKey, string> colors =
            new()
            {
                [ColorKey.Background] = "#282A36",
                [ColorKey.Panel] = "#44475A",
                [ColorKey.Foreground] = "#F8F8F2",
                [ColorKey.Comment] = "#6272A4",

                [ColorKey.Cyan] = "#8BE9FD",
                [ColorKey.Green] = "#50FA7B",
                [ColorKey.Orange] = "#FFB86C",
                [ColorKey.Pink] = "#FF79C6",
                [ColorKey.Purple] = "#BD93F9",
                [ColorKey.Red] = "#FF5555",
                [ColorKey.Yellow] = "#F1FA8C",

                [ColorKey.Accent1] = "#F1FA8C",
                [ColorKey.Accent2] = "#FFD866",
                [ColorKey.Accent3] = "#FFF59D",
                [ColorKey.Accent4] = "#E6DB74",
                [ColorKey.Accent5] = "#FFCC66",
                [ColorKey.Accent6] = "#FAF3C0",
                [ColorKey.Accent7] = "#EAFD5E",
            };

        public Brush Get(ColorKey key) => (Brush)new BrushConverter().ConvertFromString(colors[key]);

    }
}
