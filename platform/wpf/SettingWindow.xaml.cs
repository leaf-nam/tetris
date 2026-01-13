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

    public partial class SettingWindow : Window
    {
        // 현재 설정 임시 저장
        private string _selectedTheme = Settings.Default.ThemePath;
        private bool _shadowEnabled = Settings.Default.ShadowEnabled;

        private BlockRenderer leftRenderer;
        private BlockRenderer rightRenderer;

        public SettingWindow()
        {
            InitializeComponent();

            // ComboBox 선택 반영
            foreach (ComboBoxItem item in ShadowComboBox.Items)
            {
                if ((_shadowEnabled && item.Tag.ToString() == "on") ||
                    (!_shadowEnabled && item.Tag.ToString() == "off"))
                {
                    item.IsSelected = true;
                    break;
                }
            }

            leftRenderer = new BlockRenderer(CanvasLeft, 0);
            rightRenderer = new BlockRenderer(CanvasRight, 0);

            CanvasLeft.Loaded += (s, e) => { RenderSide(); };
        }

        private void RenderSide()
        {
            CustomColors.SetTheme(new DynamicTheme());
            CanvasLeft.Children.Clear();
            CanvasRight.Children.Clear();

            for (int i = 0; i < 7; i++)
            {
                TetrominoWrapper tetromino = new TetrominoWrapper();

                tetromino.type = i;
                tetromino.rotation = 0;
                tetromino.x = 4;
                tetromino.y = 2 + i * 3;

                if (i == 0) tetromino.y++;
                if (i == 1) tetromino.y--;

                if (_shadowEnabled)
                {
                    tetromino.y++;
                    leftRenderer.DrawTetromino(tetromino, CustomColors.Theme.Get(ColorKey.Comment));
                    tetromino.y--;
                }

                leftRenderer.DrawTetromino(tetromino);
            }

            for (int i = 0; i < 7; i++)
            {
                TetrominoWrapper tetromino = new TetrominoWrapper();

                tetromino.type = i;
                tetromino.rotation = 0;
                tetromino.x = 4;
                tetromino.y = 2 + (i + 5) % 7 * 3;

                if (i == 0) tetromino.y++;
                if (i == 1) tetromino.y--;


                if (_shadowEnabled)
                {
                    tetromino.y++;
                    rightRenderer.DrawTetromino(tetromino, CustomColors.Theme.Get(ColorKey.Comment));
                    tetromino.y--;
                }

                rightRenderer.DrawTetromino(tetromino);
            }
        }
        #region 테마 변경
        private void ThemeComboBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (ThemeComboBox.SelectedItem is ComboBoxItem selectedItem)
            {
                string themePath = selectedItem.Tag.ToString();
                ChangeTheme(themePath);
                _selectedTheme = themePath;
            }
        }

        private void ChangeTheme(string themePath)
        {
            var app = Application.Current;

            // 새 테마 추가
            var newTheme = new ResourceDictionary { Source = new Uri(themePath, UriKind.Relative) };
            app.Resources.MergedDictionaries.Add(newTheme);

            // 기존 테마 제거
            var oldTheme = app.Resources.MergedDictionaries
                .FirstOrDefault(d => d.Source != null && d.Source.OriginalString.Contains("theme") && d != newTheme);

            if (oldTheme != null)
                app.Resources.MergedDictionaries.Remove(oldTheme);

            if (leftRenderer != null && rightRenderer != null)
            {
                leftRenderer = new BlockRenderer(CanvasLeft, 0);
                rightRenderer = new BlockRenderer(CanvasRight, 0);
                
                RenderSide();
            }
        }
        #endregion

        #region 쉐도우 체크박스
        private void ShadowComboBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (ShadowComboBox.SelectedItem is ComboBoxItem selectedItem)
            {
                _shadowEnabled = selectedItem.Tag.ToString() == "on";
            }

            if (leftRenderer != null && rightRenderer != null)
            {
                leftRenderer = new BlockRenderer(CanvasLeft, 0);
                rightRenderer = new BlockRenderer(CanvasRight, 0);

                RenderSide();
            }
        }
        #endregion

        private void SaveSettings_Click(object sender, RoutedEventArgs e)
        {
            // 실제 저장: App Settings 또는 Engine 반영
            Settings.Default.ThemePath = _selectedTheme;
            Settings.Default.ShadowEnabled = _shadowEnabled;
            Settings.Default.Save();

            MessageBox.Show("설정이 저장되었습니다!", "TETRISSEN", MessageBoxButton.OK, MessageBoxImage.Information);
            
            var startWindow = new StartWindow();
            startWindow.Show();
            this.Close();
        }

        private void CancelSettings_Click(object sender, RoutedEventArgs e)
        {
            // 변경사항 무시
            if (_selectedTheme != Settings.Default.ThemePath)
            {
                ChangeTheme(Settings.Default.ThemePath);
            }

            var startWindow = new StartWindow();
            startWindow.Show();
            this.Close();
        }
    }

}
