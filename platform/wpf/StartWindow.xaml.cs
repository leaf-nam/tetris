using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using wpf.render;
using wpf.render.theme;

namespace wpf
{

    public partial class StartWindow : Window
    {
        private BlockRenderer titleRenderer;
        private BlockRenderer leftRenderer;
        private BlockRenderer rightRenderer;

        public StartWindow()
        {
            InitializeComponent();
            CustomColors.SetTheme(new DynamicTheme());

            titleRenderer = new BlockRenderer(CanvasTitle, 10);
            leftRenderer = new BlockRenderer(CanvasLeft, 0);
            rightRenderer = new BlockRenderer(CanvasRight, 0);

            titleRenderer.DrawString("TETRISSEN v1", 3, 4);

            CanvasLeft.Loaded += (s, e) =>
            {
                for (int i = 0; i < 7; i++)
                {
                    TetrominoWrapper tetromino = new TetrominoWrapper();

                    tetromino.type = i;
                    tetromino.rotation = 0;
                    tetromino.x = 4;
                    tetromino.y = 2 + i * 3;

                    if (i == 0) tetromino.y++;
                    if (i == 1) tetromino.y--;

                    leftRenderer.DrawTetromino(tetromino);
                }
            };

            CanvasRight.Loaded += (s, e) =>
            {
                for (int i = 0; i < 7; i++)
                {
                    TetrominoWrapper tetromino = new TetrominoWrapper();

                    tetromino.type = i;
                    tetromino.rotation = 0;
                    tetromino.x = 4;
                    tetromino.y = 2 + (i + 5) % 7 * 3;

                    if (i == 0) tetromino.y++;
                    if (i == 1) tetromino.y--;

                    rightRenderer.DrawTetromino(tetromino);
                }
            };
        }

        private void SingleGame_Click(object sender, RoutedEventArgs e)
        {
            var gameWindow = new MainWindow();
            gameWindow.Show();
            this.Close();
        }

        private void MultiGame_Click(object sender, RoutedEventArgs e)
        {
            var gameWindow = new MainWindow();
            gameWindow.Show();
            this.Close();
        }

        private void Setting_Click(object sender, RoutedEventArgs e)
        {
            var settingWindow = new SettingWindow();
            settingWindow.Show();
            this.Close();
        }

        private void Exit_Click(object sender, RoutedEventArgs e)
        {
            Application.Current.Shutdown();
        }
    }

}
