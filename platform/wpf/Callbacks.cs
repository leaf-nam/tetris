using System.Runtime.InteropServices;

namespace wpf
{ 
    public class Callbacks
    {
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate IntPtr ScanCallback();
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void BackgroundCallback();
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void BoardCallback(BoardWrapper boardWrapper, TetrominoWrapper tetrominoWrapper);
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void TimerCallback(int value);
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void NextBlockCallback(TetrominoWrapper tetrominoWrapper1, TetrominoWrapper tetrominoWrapper2, TetrominoWrapper tetrominoWrapper3);
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void HoldCallback(TetrominoWrapper tetrominoWrapper);
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void ScoreCallback(int value);
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void LevelCallback(int value);

        public ScanCallback scan_callback { get; set; }
        public BackgroundCallback background_callback { get; set; }
        public BoardCallback board_callback { get; set; }
        public TimerCallback timer_callback { get; set; }
        public NextBlockCallback next_block_callback { get; set; }
        public HoldCallback hold_callback { get; set; }
        public ScoreCallback score_callback { get; set; }
        public LevelCallback level_callback { get; set; }

    }

}