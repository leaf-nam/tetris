#include "tetromino/tetromino.hpp"

#include <gtest/gtest.h>

bool is_same(const Mino& m1, const Mino& m2);

bool is_same(const Mino& m1, const Mino& m2)
{
    for (int r = 0; r < MINO_SIZE; ++r) {
        for (int c = 0; c < MINO_SIZE; ++c) {
            if (m1[r][c] != m2[r][c]) return false;
        }
    }

    return true;
}

TEST(TetrominoTest, rotation)
{
    Tetromino m;
    m.init_mino(0);

    for (int rot = 0; rot < 4; ++rot) {
        m.set_rotation(rot);

        EXPECT_EQ(m.get_rotation(), rot);
        EXPECT_EQ(is_same(m.get_shape(), TETROMINO[0][rot]), true);
    }
}

TEST(TetrominoTest, position)
{
    Tetromino m;
    m.init_mino(0);

    m.set_pos(3, 3);

    auto [r, c] = m.get_pos();

    EXPECT_EQ(r, 3);
    EXPECT_EQ(c, 3);
}

TEST(TetrominoTest, set)
{
    Tetromino m;
    m.set_mino_type(2);

    EXPECT_EQ(m.get_mino_type(), 2);
}