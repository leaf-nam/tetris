using System.Runtime.InteropServices;

[StructLayout(LayoutKind.Sequential, Pack = 4)]
public struct TetrominoWrapper
{
    public ushort shape;
    public int x;
    public int y;
}