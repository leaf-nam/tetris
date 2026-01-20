using System.Runtime.InteropServices;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using wpf.render;
using wpf.render.theme;
using wpf.render.utility;

namespace wpf
{

    public partial class MainWindow : Window
    {

        [DllImport("engine_wrapper.dll")]
        static extern void register_callbacks(
            Callbacks.ScanCallback scanCallback,
            Callbacks.BackgroundCallback backgroundCallback,
            Callbacks.BoardCallback boardCallback,
            Callbacks.HoldCallback holdCallback,
            Callbacks.NextBlockCallback NextCallback,
            Callbacks.TimerCallback timerCallback,
            Callbacks.ScoreCallback scoreCallback,
            Callbacks.LevelCallback levelCallback,
            Callbacks.FinishCallback finishCallback
        );

        [DllImport("engine_wrapper.dll")]
        static extern void init_engine();

        [DllImport("engine_wrapper.dll")]
        static extern void run_engine();

        [DllImport("engine_wrapper.dll")]
        static extern void stop_engine();

        [DllImport("engine_wrapper.dll")]
        static extern void finish_engine();

        private BlockRenderer titleRenderer;
        private BlockRenderer nextTitleRenderer;
        private BlockRenderer holdTitleRenderer;
        private BlockRenderer scoreTitleRenderer;
        private BlockRenderer levelTitleRenderer;
        private BlockRenderer boardRenderer;
        private BlockRenderer timerRenderer;
        private BlockRenderer next1Renderer;
        private BlockRenderer next2Renderer;
        private BlockRenderer next3Renderer;
        private BlockRenderer holdRenderer;
        private BlockRenderer scoreRenderer;
        private BlockRenderer levelRenderer;
        private BlockRenderer overlayTitleRenderer;
        private BlockRenderer overlayContentRenderer;


        Callbacks.ScanCallback scanCallback;
        Callbacks.BackgroundCallback backgroundCallback;
        Callbacks.BoardCallback boardCallback;
        Callbacks.HoldCallback holdCallback;
        Callbacks.NextBlockCallback nextCallback;
        Callbacks.TimerCallback timerCallback;
        Callbacks.ScoreCallback scoreCallback;
        Callbacks.LevelCallback levelCallback;
        Callbacks.FinishCallback finishCallback;

        bool finished = false;

        public MainWindow()
        {
            InitializeComponent();
            this.Focus();

            // 테마 설정
            CustomColors.SetTheme(new DynamicTheme());

            // 렌더 초기화
            InitRender();

            // 콜백 등록
            scanCallback = new Callbacks.ScanCallback(SendInput);
            backgroundCallback = new Callbacks.BackgroundCallback(UpdateBackground);
            boardCallback = new Callbacks.BoardCallback(UpdateBoard);
            holdCallback = new Callbacks.HoldCallback(UpdateHold);
            nextCallback = new Callbacks.NextBlockCallback(UpdateNext);
            timerCallback = new Callbacks.TimerCallback(UpdateTimer);
            scoreCallback = new Callbacks.ScoreCallback(UpdateScore);
            levelCallback = new Callbacks.LevelCallback(UpdateLevel);
            finishCallback = new Callbacks.FinishCallback(GameFinish);

            register_callbacks(scanCallback, backgroundCallback, boardCallback, holdCallback, nextCallback, timerCallback, scoreCallback, levelCallback, finishCallback);

            // 게임 실행
            GameStart();
        }

        private async void GameStart()
        {
            // 타이머 실행
            await GameStartCountdown();

            // 엔진 실행
            finished = false;
            init_engine();
            run_engine();
        }

        async Task GameStartCountdown()
        {
            Overlay.Visibility = Visibility.Visible;

            overlayTitleRenderer.DrawString("READY", -13, -10);

            CanvasOverlayContent.Children.Clear();
            overlayContentRenderer.DrawString(3.ToString(), 0, 0);
            await Task.Delay(1000);

            CanvasOverlayContent.Children.Clear();
            overlayContentRenderer.DrawString(2.ToString(), 0, 0);
            await Task.Delay(1000);

            CanvasOverlayContent.Children.Clear();
            overlayContentRenderer.DrawString(1.ToString(), 0, 0);
            await Task.Delay(1000);

            CanvasOverlayContent.Children.Clear();
            overlayContentRenderer.DrawString("GO", -3, 0);
            await Task.Delay(1000);

            CanvasOverlayTitle.Children.Clear();
            CanvasOverlayContent.Children.Clear();
            Overlay.Visibility = Visibility.Collapsed;
        }

        private void InitRender()
        {
            titleRenderer = new BlockRenderer(CanvasTitle, 8);
            nextTitleRenderer = new BlockRenderer(CanvasNextTitle, 5);
            holdTitleRenderer = new BlockRenderer(CanvasHoldTitle, 5);
            scoreTitleRenderer = new BlockRenderer(CanvasScore, 3);
            levelTitleRenderer = new BlockRenderer(CanvasLevel, 3);
            boardRenderer = new BlockRenderer(CanvasBoard, 0);
            timerRenderer = new BlockRenderer(CanvasTimer, 5);
            next1Renderer = new BlockRenderer(CanvasNext1, 30);
            next2Renderer = new BlockRenderer(CanvasNext2, 30);
            next3Renderer = new BlockRenderer(CanvasNext3, 30);
            holdRenderer = new BlockRenderer(CanvasHold, 30);
            scoreRenderer = new BlockRenderer(CanvasScore, 3);
            levelRenderer = new BlockRenderer(CanvasLevel, 3);
            overlayTitleRenderer = new BlockRenderer(CanvasOverlayTitle, 12);
            overlayContentRenderer = new BlockRenderer(CanvasOverlayContent, 8);
        }

        // 입력 받아서 전송하기
        StringBuilder buffer = new StringBuilder();

        // TODO : 입력 누르고 있을때 처리
        private void Window_KeyDown(object sender, KeyEventArgs e)
        {
            buffer.Append(e.Key.ToString().ToLower());
        }

        char SendInput()
        {
            if (buffer.Length > 0)
            {
                char lastChar = buffer[^1];
                buffer.Clear();
                return lastChar;
            }

            return '\0';
        }

        // 백그라운드 업데이트
        void UpdateBackground()
        {
            Dispatcher.Invoke(() =>
            {
                titleRenderer.DrawString("TETRISSEN v1", 0, 4);
                nextTitleRenderer.DrawString("NEXT", 6, 2, CustomColors.Theme.Get(ColorKey.Cyan));
                holdTitleRenderer.DrawString("HOLD", 6, 2, CustomColors.Theme.Get(ColorKey.Green));
                scoreTitleRenderer.DrawString("Score", 6, 2, CustomColors.Theme.Get(ColorKey.Comment));
                levelTitleRenderer.DrawString("Lv", 6, 2, CustomColors.Theme.Get(ColorKey.Comment));

                // 초기 스코어, 레벨 렌더링
                scoreRenderer.DrawStringCenter("0");
                levelRenderer.DrawStringCenter("1");
            });
        }

        // 보드 업데이트
        void UpdateBoard(BoardWrapper board, TetrominoWrapper tetromino)
        {
            Dispatcher.Invoke(() =>
            {
                CanvasBoard.Children.Clear();
                if (Settings.Default.ShadowEnabled) boardRenderer.DrawTetrominoShadow(board, tetromino);

                boardRenderer.DrawBoard(board);
                boardRenderer.DrawTetromino(tetromino);
            });
        }

        // 홀드 업데이트
        void UpdateHold(int tetrominoType)
        {
            Dispatcher.Invoke(() =>
            {
                CanvasHold.Children.Clear();
                holdRenderer.DrawTetrominoCenter(tetrominoType);
            });
        }

        // 넥스트 업데이트
        void UpdateNext(IntPtr tetromino)
        {
            Dispatcher.Invoke(() =>
            {
                CanvasNext1.Children.Clear();
                CanvasNext2.Children.Clear();
                CanvasNext3.Children.Clear();
                next1Renderer.DrawTetrominoCenter(Marshal.ReadInt32(tetromino, 0));
                next2Renderer.DrawTetrominoCenter(Marshal.ReadInt32(tetromino, 1 * sizeof(int)));
                next3Renderer.DrawTetrominoCenter(Marshal.ReadInt32(tetromino, 2 * sizeof(int)));
            });
        }

        // 타이머 업데이트
        void UpdateTimer(int value)
        {
            Dispatcher.Invoke(() =>
            {
                CanvasTimer.Children.Clear();
                timerRenderer.DrawString(TimeUtility.ConvertSecondToString(value), 3, 10, CustomColors.Theme.Get(ColorKey.Comment));
            });
        }

        // 점수 업데이트
        void UpdateScore(int score)
        {
            Dispatcher.Invoke(() =>
            {
                CanvasScore.Children.Clear();
                scoreRenderer.DrawStringCenter(score.ToString());
            });
        }

        // 레벨 업데이트
        void UpdateLevel(int lv)
        {
            Dispatcher.Invoke(() =>
            {
                CanvasLevel.Children.Clear();
                levelRenderer.DrawStringCenter(lv.ToString());
            });
        }

        // 게임 종료
        void GameFinish()
        {
            try
            {
                if (!finished) finish_engine();
            }
            catch { }

            Dispatcher.Invoke(() =>
            {
                overlayTitleRenderer.DrawString("GAME OVER", -10, -10);
                Overlay.Visibility = Visibility.Visible;
                Overlay2.Visibility = Visibility.Visible;
            });
        }

        private void GoHome_Click(object sender, RoutedEventArgs e)
        {
            var startWindow = new StartWindow();
            startWindow.Show();
            this.Close();
        }


        private void Exit_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }

        protected override void OnClosed(EventArgs e)
        {
            try
            {
                if (!finished) finish_engine();
            }
            catch { }
            base.OnClosed(e);
        }
    }

}