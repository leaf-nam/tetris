#include "board/board.hpp"

#include <gtest/gtest.h>

TEST(BoardTest, SpawnMino)
{
    Board board;
    board.spawn_mino(3);

    auto [r, c] = board.get_active_mino_pos();
    int rotation = board.get_active_mino_rotation();
    int type = board.get_active_mino_type();

    EXPECT_EQ(r, 0);
    EXPECT_EQ(c, 3);
    EXPECT_EQ(rotation, 0);
    EXPECT_EQ(type, 3);
}

TEST(BoardTest, MoveMino)
{
    Board board;
    board.spawn_mino(3);

    board.move_active_mino(5, 5, 3, FIX_IF_FAIL);

    auto [r, c] = board.get_active_mino_pos();
    int rotation = board.get_active_mino_rotation();
    int type = board.get_active_mino_type();

    EXPECT_EQ(r, 5);
    EXPECT_EQ(c, 5);
    EXPECT_EQ(rotation, 3);
    EXPECT_EQ(type, 3);
}
