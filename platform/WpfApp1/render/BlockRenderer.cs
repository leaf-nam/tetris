using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Shapes;

namespace WpfApp1.render
{
    public class BlockRenderer
    {
        private readonly Canvas canvas;
        private readonly int blockSize;

        public BlockRenderer(Canvas canvas, int blockSize)
        {
            this.canvas = canvas;
            this.blockSize = blockSize;
        }

        public void DrawBlock(int x, int y, Brush color)
        {
            Rectangle rect = new Rectangle
            {
                Width = blockSize,
                Height = blockSize,
                Fill = color
            };

            Canvas.SetLeft(rect, x * blockSize);
            Canvas.SetTop(rect, y * blockSize);

            canvas.Children.Add(rect);
        }

        public void DrawChar(char c, int offsetX, int offsetY, Brush color)
        {
            ulong glyph = Font5x7.Get(c);

            for (int y = 0; y < 7; y++)
            {
                for (int x = 0; x < 5; x++)
                {
                    int bit = y * 5 + x;

                    if ((glyph & (1UL << bit)) != 0)
                    {
                        DrawBlock(
                            4 - x + offsetX,
                            6 - y + offsetY,
                            color
                        );
                    }
                }
            }
        }

        public void DrawString(string s, int x, int y, Brush color)
        {
            for (int i = 0; i < s.Length; i++)
            {
                DrawChar(s[i], x + i * 6, y, color);
            }
        }

        static readonly Brush[] BlockColors =
        {
            CustomColors.Theme.Get(ColorKey.Accent1),
            CustomColors.Theme.Get(ColorKey.Accent2),
            CustomColors.Theme.Get(ColorKey.Accent3),
            CustomColors.Theme.Get(ColorKey.Accent4),
            CustomColors.Theme.Get(ColorKey.Accent5),
            CustomColors.Theme.Get(ColorKey.Accent6),
            CustomColors.Theme.Get(ColorKey.Accent7)
        };

        static readonly Random rng = new Random();

        public void DrawString(string s, int x, int y)
        {
            for (int i = 0; i < s.Length; i++)
            {
                DrawChar(s[i], x + i * 6, y, BlockColors[rng.Next(BlockColors.Length)]);
            }
        }

        public void Clear()
        {
            canvas.Children.Clear();
        }
    }
}
