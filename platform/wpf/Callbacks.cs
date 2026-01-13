using System.Runtime.InteropServices;

namespace wpf
{ 
    public class Callbacks
    {
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate char ScanCallback();
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void BackgroundCallback();
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void BoardCallback(BoardWrapper board, TetrominoWrapper tetromino);
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void HoldCallback(int type);
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void NextBlockCallback(IntPtr types);
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void TimerCallback(int value);
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