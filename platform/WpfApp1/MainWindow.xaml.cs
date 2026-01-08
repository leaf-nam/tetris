using System;
using System.Runtime.InteropServices;
using System.Windows;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Threading;
using WpfApp1.render;

namespace WpfApp1
{

    public partial class MainWindow : Window
    {

        [DllImport("DllTest2.dll")]
        static extern void RegisterCallback(Callback cb);

        [DllImport("DllTest2.dll")]
        static extern void StartLoop();

        [DllImport("DllTest2.dll")]
        static extern void StopLoop();

        private BlockRenderer rendererTimer;
        public MainWindow()
        {
            InitializeComponent();
            this.Focus();

            // 테마 설정
            CustomColors.SetTheme(new DynamicTheme());

            rendererTimer = new BlockRenderer(CanvasTimer, 5);
            var rendererTitle = new BlockRenderer(CanvasTitle, 8);
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

            CanvasTitle.UpdateLayout();
            CanvasTimer.UpdateLayout();
            CanvasNextTitle.UpdateLayout();

            // 콜백 등록
            _callbackDelegate = new Callback(OnUpdateTimer);

            RegisterCallback(_callbackDelegate);

            // 루프 시작
            StartLoop();

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

        delegate void Callback(int value);

        private Callback _callbackDelegate;

        // DLL에서 호출되는 함수
        void OnUpdateTimer(int value)
        {
            // WPF UI 스레드 안전하게 실행
            Dispatcher.Invoke(() =>
            {
                CanvasTimer.Children.Clear();
                rendererTimer.DrawString(TimeUtility.ConvertSecondToString(value), 3, 10, CustomColors.Theme.Get(ColorKey.Comment));
            });
        }

        protected override void OnClosed(EventArgs e)
        {
            
            try
            {
                StopLoop(); // DLL 스레드 종료
            }
            catch { }
            base.OnClosed(e);
        }

    }

}