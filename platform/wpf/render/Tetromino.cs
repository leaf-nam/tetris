using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace wpf.render
{
    // 테트로미노를 나타내는 클래스
    public class Tetromino
    {
        public ushort[] Rotations { get; private set; }

        public Tetromino(ushort[] rotations)
        {
            if (rotations.Length != 4)
                throw new ArgumentException("Rotation 배열은 반드시 4개여야 합니다.");
            Rotations = rotations;
        }
    }

    public static class Tetrominos
    {
        public static readonly Tetromino I = new Tetromino(new ushort[]
        {
            0b1111_0000_0000_0000,
            0b0010_0010_0010_0010,
            0b0000_1111_0000_0000,
            0b0100_0100_0100_0100
        });

        public static readonly Tetromino O = new Tetromino(new ushort[]
        {
            0b1100_1100_0000_0000,
            0b1100_1100_0000_0000,
            0b1100_1100_0000_0000,
            0b1100_1100_0000_0000
        });

        public static readonly Tetromino Z = new Tetromino(new ushort[]
        {
            0b1100_0110_0000_0000,
            0b0010_0110_0100_0000,
            0b0000_1100_0110_0000,
            0b0100_1100_1000_0000
        });

        public static readonly Tetromino S = new Tetromino(new ushort[]
        {
            0b0110_1100_0000_0000,
            0b0100_0110_0010_0000,
            0b0000_0110_1100_0000,
            0b1000_1100_0100_0000
        });

        public static readonly Tetromino J = new Tetromino(new ushort[]
        {
            0b1000_1110_0000_0000,
            0b0110_0100_0100_0000,
            0b0000_1110_0010_0000,
            0b0100_0100_1100_0000
        });

        public static readonly Tetromino L = new Tetromino(new ushort[]
        {
            0b0010_1110_0000_0000,
            0b0100_0100_0110_0000,
            0b0000_1110_1000_0000,
            0b1100_0100_0100_0000
        });

        public static readonly Tetromino T = new Tetromino(new ushort[]
        {
            0b0100_1110_0000_0000,
            0b0100_0110_0100_0000,
            0b0000_1110_0100_0000,
            0b0100_1100_0100_0000
        });

        public static readonly Tetromino[] All =
        {
            I, O, Z, S, J, L, T
        };
    }
}
