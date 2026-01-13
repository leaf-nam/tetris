using System;
using System.Runtime.InteropServices;

[StructLayout(LayoutKind.Sequential)]
public struct BoardWrapper
{
    public IntPtr board;
    public int maxX;
    public int maxY;
}