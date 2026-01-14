
﻿using System;
using System.Collections.Specialized;
using System.Diagnostics;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;
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

        private void DrawBlockAbsolute(double x, double y, double blockSize, Brush color)
        { 

            Rectangle rect = new Rectangle
            {
                Width = blockSize,
                Height = blockSize,
                Fill = color
            };

            Canvas.SetLeft(rect, x);
            Canvas.SetTop(rect, y);

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

        // 문자열 가운데 정렬해서 그리기(점수판, 레벨)
        public void DrawStringCenter(string s)
        {
            int textBlockWidth = s.Length * 5;
            int textBlockHeight = 7;

            double textPixelWidth = textBlockWidth * blockSize;
            double textPixelHeight = textBlockHeight * blockSize;

            double startXpx = (canvas.ActualWidth - textPixelWidth) / 2;
            double startYpx = (canvas.ActualHeight - textPixelHeight) / 2;

            int startX = (int)Math.Round(startXpx / blockSize);
            int startY = (int)Math.Round(startYpx / blockSize);

            DrawString(s, startX, startY);
        }

        // 지정된 색깔로 문자 그리기
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
        public void DrawBoard(BoardWrapper board)
        {
            for (int y = 0; y < board.maxY; y++)
            {
                for (int x = 0; x < board.maxX; x++)
                {
                    int offset = (y * board.maxX + x) * sizeof(int);
                    int value = Marshal.ReadInt32(board.board, offset);
                    if (value <= 7)
                    {
                        DrawBlock20x10(x, y, GetTetrominoColor(value));
                    }
                }
            }
        }

        // 보드 디버깅용
        public void PrintBoard(BoardWrapper board)
        {
            StringBuilder sb = new StringBuilder();

            for (int y = 0; y < board.maxY; y++)
            {
                for (int x = 0; x < board.maxX; x++)
                {
                    int offset = (y * board.maxX + x) * sizeof(int);
                    int value = Marshal.ReadInt32(board.board, offset);

                    // 보기 좋게 한 칸씩
                    sb.Append(value);
                    sb.Append(' ');
                }
                sb.AppendLine();
            }

            MessageBox.Show(sb.ToString());
        }

        // 테트로미노 타입에 따라 블록 렌더링(메인 보드용)
        public void DrawTetromino(TetrominoWrapper tetromino)
        {
            DrawTetromino(tetromino, GetTetrominoColor(tetromino.type));
        }

        // 색깔 변경 가능
        public void DrawTetromino(TetrominoWrapper tetromino, Brush color)
        {
            if (tetromino.y < 1) return;
            DrawTetromino(Tetrominos.All[tetromino.type].Rotations[tetromino.rotation], tetromino.x - 1, tetromino.y - 3, color);
        }

        // 쉐도우 그리기
        public void DrawTetrominoShadow(BoardWrapper board, TetrominoWrapper tetromino)
        {
            // 1. 떨어질 위치 계산
            int dropY = CalculateDropY(board, tetromino);

            // 2. 예상 위치 블록 그리기 (반투명)
            DrawTetromino(Tetrominos.All[tetromino.type].Rotations[tetromino.rotation], tetromino.x - 1, dropY, CustomColors.Theme.Get(ColorKey.Comment));
        }

        private int CalculateDropY(BoardWrapper board, TetrominoWrapper tetromino)
        {
            int dropY = tetromino.y - 3;

            while (!IsColliding(board, tetromino, tetromino.x - 1, dropY + 1))
            {
                dropY++; // 한 칸씩 아래로 이동
            }

            return dropY; // 충돌 직전 위치 반환
        }

        private bool IsColliding(BoardWrapper board, TetrominoWrapper tetromino, int offsetX, int offsetY)
        {
            ushort shape = Tetrominos.All[tetromino.type].Rotations[tetromino.rotation];

            if (offsetY < -2) return true;

            for (int y = 0; y < 4; y++)
            {
                for (int x = 0; x < 4; x++)
                {
                    int bit = y * 4 + x;

                    if ((shape & (1UL << bit)) != 0)
                    {
                        int nx = 4 - x + offsetX;
                        int ny = 4 - y + offsetY;

                        if (nx < 0 || nx >= board.maxX || ny > 19)
                            return true;

                        int pidx = (ny * board.maxX + nx) * sizeof(int);
                        int value = Marshal.ReadInt32(board.board, pidx);
                        if (value >= 0 && value <= 7) // 이미 블록 존재
                            return true;
                    }
                }
            }

            return false;
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
                case 7: return CustomColors.Theme.Get(ColorKey.Comment);
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
        public void DrawTetrominoCenter(int tetrominoType)
        {
            if (tetrominoType < 0 || tetrominoType > 7) return;
            DrawTetrominoCenter(Tetrominos.All[tetrominoType].Rotations[0], GetTetrominoColor(tetrominoType));
        }

        private void DrawTetrominoCenter(ushort tetromino, Brush color)
        {
            double blockSize = Math.Min(canvas.ActualWidth / 4, canvas.ActualHeight / 4);

            var realSize = calculateRealSize(tetromino);
            int realRows = (int)realSize.Item1;
            int realCols = (int)realSize.Item2;

            double realWidth = realCols * blockSize;
            double realHeight = realRows * blockSize;

            // ⭐ 핵심: 실제 블록 기준 중앙
            double startX = (canvas.ActualWidth - realWidth) / 2;
            double startY = (canvas.ActualHeight - realHeight) / 2;

            // minRow / minCol 다시 계산
            int minRow = 4, minCol = 4;

            for (int r = 0; r < 4; r++)
                for (int c = 0; c < 4; c++)
                    if (((tetromino >> (r * 4 + c)) & 1) == 1)
                    {
                        minRow = Math.Min(minRow, r);
                        minCol = Math.Min(minCol, c);
                    }

            for (int y = 0; y < 4; y++)
            {
                for (int x = 0; x < 4; x++)
                {
                    int bit = y * 4 + x;
                    if ((tetromino & (1 << bit)) != 0)
                    {
                        DrawBlockAbsolute(
                            startX + (x - minCol) * blockSize,
                            startY + (y - minRow) * blockSize,
                            blockSize,
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
