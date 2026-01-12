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
            Callbacks.TimerCallback timerCallback,
            Callbacks.ScoreCallback scoreCallback,
            Callbacks.LevelCallback levelCallback
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


        Callbacks.ScanCallback scanCallback;
        Callbacks.BackgroundCallback backgroundCallback;
        Callbacks.BoardCallback boardCallback;
        Callbacks.TimerCallback timerCallback;
        Callbacks.ScoreCallback scoreCallback;
        Callbacks.LevelCallback levelCallback;

        public MainWindow()
        {
            InitializeComponent();
            this.Focus();

            // 테마 설정
            CustomColors.SetTheme(new DynamicTheme());

            // 렌더 초기화
            initRender();

            // 콜백 등록
            scanCallback = new Callbacks.ScanCallback(SendInput);
            backgroundCallback = new Callbacks.BackgroundCallback(UpdateBackground);
            boardCallback = new Callbacks.BoardCallback(UpdateBoard);
            timerCallback = new Callbacks.TimerCallback(UpdateTimer);
            scoreCallback = new Callbacks.ScoreCallback(UpdateScore);
            levelCallback = new Callbacks.LevelCallback(UpdateLevel);

            register_callbacks(scanCallback, backgroundCallback, boardCallback, timerCallback, scoreCallback, levelCallback);

            // 엔진 실행
            init_engine();
            run_engine();

            // 보드 렌더링 콜백 등록(보드 크기를 알아야해서 콜백으로 실행함)
            CanvasBoard.Loaded += (s, e) =>
            {

                // 홀드 렌더링
                //holdRenderer.DrawTetrominoCenter(5, 0);

                // 넥스트 렌더링
                //next1Renderer.DrawTetrominoCenter(1, 0);
                //next2Renderer.DrawTetrominoCenter(2, 0);
                //next3Renderer.DrawTetrominoCenter(3, 0);

            };

        }

        private void initRender()
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
        }

        protected override void OnClosed(EventArgs e)
        {
            try
            {
                finish_engine();
            }
            catch { }
            base.OnClosed(e);
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
                boardRenderer.DrawBoard(board);
                boardRenderer.DrawTetromino(tetromino);
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
                scoreRenderer.DrawStringCenter(score.ToString());
            });
        }

        // 레벨 업데이트
        void UpdateLevel(int lv)
        {
            Dispatcher.Invoke(() =>
            {
                levelRenderer.DrawStringCenter(lv.ToString());
            });
        }

        IntPtr MyScanCallback()
        {
            string s = "Hello from C#";
            // C++에서 읽을 수 있도록 unmanaged 메모리로 변환
            return Marshal.StringToHGlobalAnsi(s);
        }
    }

}