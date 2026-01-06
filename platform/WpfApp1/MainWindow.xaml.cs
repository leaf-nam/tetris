using System.Windows;
using System.Windows.Input;
using WpfApp1.render;

namespace WpfApp1
{
    public partial class MainWindow : Window
    {

        public MainWindow()
        {
            InitializeComponent();
            this.Focus();

            // 테마 설정
            CustomColors.SetTheme(new DynamicTheme());

            var rendererTitle = new BlockRenderer(CanvasTitle, 8);
            var rendererTimer = new BlockRenderer(CanvasTimer, 5);
            var rendererNextTitle = new BlockRenderer(CanvasNextTitle, 5);
            var rendererHoldTitle = new BlockRenderer(CanvasHoldTitle, 5);
            var rendererScore = new BlockRenderer(CanvasScore, 3);
            var rendererLevel = new BlockRenderer(CanvasLevel, 3);


            rendererTitle.DrawString("TETRISSEN v1", 0, 4);
            rendererTimer.DrawString("00:00", 3, 10, CustomColors.Theme.Get(ColorKey.Comment));
            rendererNextTitle.DrawString("NEXT", 6, 2, CustomColors.Theme.Get(ColorKey.Cyan));
            rendererHoldTitle.DrawString("HOLD", 6, 2, CustomColors.Theme.Get(ColorKey.Green));

            rendererScore.DrawString("Score", 6, 2, CustomColors.Theme.Get(ColorKey.Comment));
            rendererLevel.DrawString("Lv", 6, 2, CustomColors.Theme.Get(ColorKey.Comment));
        }

        private void Window_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Left)
                MessageBox.Show("왼쪽 키 입력됨");

            if (e.Key == Key.Space)
                MessageBox.Show("스페이스 입력됨");
        }

        private void Window_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key != Key.Enter)
                MessageBox.Show($"{e.Key} released");
        }
    }

}