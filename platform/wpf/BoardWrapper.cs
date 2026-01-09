using System;
using System.Runtime.InteropServices;

[StructLayout(LayoutKind.Sequential)]
public struct BoardWrapper
{
    public IntPtr cells;
    public int xs;
    public int ys;
}