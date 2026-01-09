using System.Collections.Specialized;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Shapes;
using wpf.render.theme;

namespace wpf.render
{
    public class BlockRenderer
    {
        private readonly Canvas canvas;
        private readonly int blockSize;

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

        public BlockRenderer(Canvas canvas, int blockSize)
        {
            this.canvas = canvas;
            this.blockSize = blockSize;
        }

        private void DrawBlock(int x, int y, Brush color)
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

        private void DrawBlockOffset(int x, int y, double offsetX, double offsetY, Brush color)
        {
            double blockSize = Math.Max(canvas.ActualWidth / 4, canvas.ActualHeight / 4);

            Rectangle rect = new Rectangle
            {
                Width = blockSize,
                Height = blockSize,
                Fill = color
            };

            Canvas.SetLeft(rect, x * blockSize + offsetX);
            Canvas.SetTop(rect, y * blockSize + offsetY);

            canvas.Children.Add(rect);
        }

        private void DrawBlock20x10(int x, int y, Brush color)
        {
            double blockSize = Math.Max(canvas.ActualWidth / 10, canvas.ActualHeight / 20);
            double offsetX = (canvas.ActualWidth - blockSize * 10) / 2;
            double offsetY = (canvas.ActualHeight - blockSize * 20) / 2;

            Rectangle rect = new Rectangle
            {
                Width = blockSize,
                Height = blockSize,
                Fill = color
            };

            Canvas.SetLeft(rect, x * blockSize + offsetX);
            Canvas.SetTop(rect, y * blockSize + offsetY);

            canvas.Children.Add(rect);
        }

        private Border GetParentBorder(Canvas c)
        {
            DependencyObject parent = VisualTreeHelper.GetParent(c);

            while (parent != null && !(parent is Border))
            {
                parent = VisualTreeHelper.GetParent(parent);
            }

            return parent as Border;
        }

        // 브러쉬 지정되지 않으면 랜덤으로 사용
        public void DrawString(string s, int x, int y)
        {
            for (int i = 0; i < s.Length; i++)
            {
                DrawChar(s[i], x + i * 6, y, BlockColors[rng.Next(BlockColors.Length)]);
            }
        }

        // 지정된 색깔로 문자 그리기
        public void DrawString(string s, int x, int y, Brush color)
        {
            for (int i = 0; i < s.Length; i++)
            {
                DrawChar(s[i], x + i * 6, y, color);
            }
        }

        private void DrawChar(char c, int offsetX, int offsetY, Brush color)
        {
            ulong charHex = Font5x7.Get(c);

            for (int y = 0; y < 7; y++)
            {
                for (int x = 0; x < 5; x++)
                {
                    int bit = y * 5 + x;

                    if ((charHex & (1UL << bit)) != 0)
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

        // 보드 렌더링
        public void DrawBoard(int[,] board)
        {
            for (int y = 0; y < 20; y++)
            {
                for (int x = 0; x < 10; x++)
                {
                    if (board[y, x] >= 1 && board[y, x] <= 7)
                    {
                        DrawBlock20x10(x, y, GetTetrominoColor(board[y, x] - 1));
                    }
                }
            }
        }

        // 테트로미노 타입에 따라 블록 렌더링(메인 보드용)
        public void DrawTetromino(int type, int rotate, int x, int y)
        {
            DrawTetromino(Tetrominos.All[type].Rotations[rotate], x, y, GetTetrominoColor(type));
        }

        private Brush GetTetrominoColor(int type)
        {
            switch (type)
            {
                case 0: return CustomColors.Theme.Get(ColorKey.Accent1);
                case 1: return CustomColors.Theme.Get(ColorKey.Accent2);
                case 2: return CustomColors.Theme.Get(ColorKey.Accent3);
                case 3: return CustomColors.Theme.Get(ColorKey.Accent4);
                case 4: return CustomColors.Theme.Get(ColorKey.Accent5);
                case 5: return CustomColors.Theme.Get(ColorKey.Accent6);
                case 6: return CustomColors.Theme.Get(ColorKey.Accent7);
                default: throw new ArgumentOutOfRangeException();
            }
        }

        private void DrawTetromino(ushort tetromino, int offsetX, int offsetY, Brush color)
        {
            for (int y = 0; y < 4; y++)
            {
                for (int x = 0; x < 4; x++)
                {
                    int bit = y * 4 + x;

                    if ((tetromino & (1UL << bit)) != 0)
                    {
                        DrawBlock20x10(
                            4 - x + offsetX,
                            4 - y + offsetY,
                            color
                        );
                    }
                }
            }
        }

        // 테트로미노 타입에 따라 블록 렌더링(홀드, 넥스트용)
        public void DrawTetrominoCenter(int type, int rotate)
        {
            DrawTetrominoCenter(Tetrominos.All[type].Rotations[rotate], GetTetrominoColor(type));
        }

        private void DrawTetrominoCenter(ushort tetromino, Brush color)
        {
            double blockSize = Math.Max(canvas.ActualWidth / 4, canvas.ActualHeight / 4);

            Tuple<double, double> realSize = calculateRealSize(tetromino);
            double rw = realSize.Item1 * blockSize, rh = realSize.Item2 * blockSize;
            double cw = canvas.ActualWidth, ch = canvas.ActualHeight;
            
            double offsetX = (cw - rw) / 2;
            double offsetY = (ch - rh) / 2;


            MessageBox.Show(
                "real : " + rh+ " | " + rw + "\n" +
                "canvas: " + ch + " | " + cw + "\n" +
                "==============================\n" + 
                "offset : " + offsetY + " | " + offsetX + "\n"
                );

            for (int y = 0; y < 4; y++)
            {
                for (int x = 0; x < 4; x++)
                {
                    int bit = y * 4 + x;

                    if ((tetromino & (1UL << bit)) != 0)
                    {
                        DrawBlockOffset(
                            3 - x,
                            3 - y,
                            offsetX,
                            offsetY,
                            color
                        );
                    }
                }
            }
        }

        private Tuple<double, double> calculateRealSize(ushort tetromino)
        {
            int minRow = 4, maxRow = -1;
            int minCol = 4, maxCol = -1;

            for (int r = 0; r < 4; r++)
            {
                for (int c = 0; c < 4; c++)
                {
                    if (((tetromino >> r * 4 + c) & 1) == 1)
                    {
                        minRow = Math.Min(minRow, r);
                        maxRow = Math.Max(maxRow, r);
                        minCol = Math.Min(minCol, c);
                        maxCol = Math.Max(maxCol, c);
                    }
                }
            }

            return new Tuple<double, double>(maxRow - minRow + 1, maxCol - minCol + 1);
        }

        public void Clear()
        {
            canvas.Children.Clear();
        }
    }
}
