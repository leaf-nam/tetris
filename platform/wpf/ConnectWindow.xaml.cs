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

    public partial class ConnectWindow : Window
    {
        private BlockRenderer titleRenderer;
        private BlockRenderer leftRenderer;
        private BlockRenderer rightRenderer;

        public ConnectWindow()
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

        private void CreateRoomComboBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (CreateRoomComboBox.SelectedItem is ComboBoxItem selectedItem)
            {
                if (selectedItem.Tag.ToString() == "on")
                {
                    MessageBox.Show("방 생성");
                }

                if (selectedItem.Tag.ToString() == "off")
                {
                    MessageBox.Show("방 종료");
                }
            }
        }

        private void ConnectButton_Click(object sender, RoutedEventArgs e)
        {
            string ip = IpAddressTextBox.Text.Trim();

            if (string.IsNullOrEmpty(ip))
            {
                MessageBox.Show("IP 주소를 입력하세요.");
                return;
            }

            // TODO: IP 형식 검증 (선택)
            // TODO: 서버 연결 로직

            MessageBox.Show($"연결 시도: {ip}");

            MessageBox.Show($"연결 완료");
            var gameWindow = new MultiWindow();
            gameWindow.Show();
            this.Close();
        }

        private void SaveSettings_Click(object sender, RoutedEventArgs e)
        {
            // 실제 저장: App Settings 또는 Engine 반영
            var startWindow = new StartWindow();
            startWindow.Show();
            this.Close();
        }

        private void CancelSettings_Click(object sender, RoutedEventArgs e)
        {
            // 변경사항 무시
            var startWindow = new StartWindow();
            startWindow.Show();
            this.Close();
        }
    }

}
