using System;
using System.Runtime.InteropServices;

[StructLayout(LayoutKind.Sequential)]
public struct BoardWrapper
{
    public CharPtr cells;
    public int xs;
    public int ys;
}