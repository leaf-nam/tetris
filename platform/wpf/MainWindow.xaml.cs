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

namespace wpf
{
    public partial class MainWindow : Window
    {

        //[DllImport("wpf_dll.dll")]
        //static extern void register_callback(Callbacks cb);

        //[DllImport("wpf_dll.dll")]
        //static extern void init_engine();

        //[DllImport("wpf_dll.dll")]
        //static extern void run_engine();

        //[DllImport("wpf_dll.dll")]
        //static extern void stop_engine();

        //[DllImport("wpf_dll.dll")]
        //static extern void finish_engine();

        private BlockRenderer titleRenderer;
        private BlockRenderer nextTitleRenderer;
        private BlockRenderer holdTitleRenderer;
        private BlockRenderer scoreTitleRenderer;
        private BlockRenderer levelTitleRenderer;

        private BlockRenderer boardRenderer;
        private BlockRenderer timerRenderer;
        private BlockRenderer nextRenderer;
        private BlockRenderer holdRenderer;
        private BlockRenderer scoreRenderer;
        private BlockRenderer levelRenderer;

        public MainWindow()
        {
            InitializeComponent();
            this.Focus();

            // 테마 설정
            CustomColors.SetTheme(new DynamicTheme());

            // 렌더 초기화
            titleRenderer = new BlockRenderer(CanvasTitle, 8);
            nextTitleRenderer = new BlockRenderer(CanvasNextTitle, 5);
            holdTitleRenderer = new BlockRenderer(CanvasHoldTitle, 5);
            scoreTitleRenderer = new BlockRenderer(CanvasScore, 3);
            levelTitleRenderer = new BlockRenderer(CanvasLevel, 3);

            boardRenderer = new BlockRenderer(CanvasBoard, 0);
            timerRenderer = new BlockRenderer(CanvasTimer, 5);
            nextRenderer = new BlockRenderer(CanvasBoard, 5);
            holdRenderer = new BlockRenderer(CanvasHold, 30);
            scoreRenderer = new BlockRenderer(CanvasScore, 3);
            levelRenderer = new BlockRenderer(CanvasLevel, 3);

            // 백그라운드 렌더링
            titleRenderer.DrawString("TETRISSEN v1", 0, 4);
            nextTitleRenderer.DrawString("NEXT", 6, 2, CustomColors.Theme.Get(ColorKey.Cyan));
            holdTitleRenderer.DrawString("HOLD", 6, 2, CustomColors.Theme.Get(ColorKey.Green));
            scoreTitleRenderer.DrawString("Score", 6, 2, CustomColors.Theme.Get(ColorKey.Comment));
            levelTitleRenderer.DrawString("Lv", 6, 2, CustomColors.Theme.Get(ColorKey.Comment));

            // 타이머 렌더링
            timerRenderer.DrawString("00:00", 3, 10, CustomColors.Theme.Get(ColorKey.Comment));

            // 보드 렌더링
            int[,] board = new int[20, 10];
            for (int i = 0; i < 20; i++)
            {
                for (int j = 0; j < 10; j++)
                {
                    board[i, j] = j % 7 + 1;
                }
                
            }

            // 보드 렌더링 콜백 등록(보드 크기를 알아야해서 콜백으로 실행함)
            CanvasBoard.Loaded += (s, e) =>
            {
                boardRenderer.DrawBoard(board);

                // 테트로미노 렌더링
                for (int i = 0; i < 7; i++)
                {
                    for (int j = 0; j < 4; j++)
                    {
                        boardRenderer.DrawTetromino(i, j, j * 3, i * 3);
                    }
                }

                // 홀드 렌더링
                holdRenderer.DrawTetrominoCenter(0, 1);
            };

        }

        IntPtr MyScanCallback()
        {
            string s = "Hello from C#";
            // C++에서 읽을 수 있도록 unmanaged 메모리로 변환
            return Marshal.StringToHGlobalAnsi(s);
        }

        StringBuilder buffer = new StringBuilder();

        private void Window_KeyDown(object sender, KeyEventArgs e)
        {
            string allowedKeys = "wasdfqeWASDFQE";

            // Key -> char 변환
            char keyChar = KeyToChar(e.Key);
            if (allowedKeys.Contains(keyChar))
            {
                buffer.Append(keyChar);
            }
        }

        private char KeyToChar(Key key)
        {
            return key switch
            {
                Key.W => 'w',
                Key.A => 'a',
                Key.S => 's',
                Key.D => 'd',
                Key.F => 'f',
                Key.Q => 'q',
                Key.E => 'e',
                _ => '\0'
            };
        }
    }

}