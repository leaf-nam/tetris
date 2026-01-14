using System.Runtime.InteropServices;

namespace wpf
{
    [StructLayout(LayoutKind.Sequential, Pack = 4)]
    public struct TetrominoWrapper
    {
        public int type;
        public int rotation;
        public int x;
        public int y;
    }
}