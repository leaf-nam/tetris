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
// black box testing
TEST(BoardTest, HasActiveMino)
{
    Board board;
    int isActive = board.has_active_mino();

    EXPECT_FALSE(isActive);
    board.spawn_mino(5);
    isActive = board.has_active_mino();

    EXPECT_TRUE(isActive);
}
TEST(BoardTest, SwapMinoFirstTime)
{
    Board board;
    board.spawn_mino(1);
    board.swap_mino();

    EXPECT_EQ(board.get_saved_mino().get_mino_type(), 1);
    EXPECT_TRUE(board.get_is_mino_swaped());
}
TEST(BoardTest, BoardFill)
{
    Board board;

    EXPECT_TRUE(board.fill(5, 5, 3));
    // EXPECT_FALSE(board.fill(5, 5, 3));
    EXPECT_FALSE(board.fill(5, 30, 3));
}
TEST(BoardTest, IsLineFull)
{
    Board board;
    for (int i = 0; i < 10; i++) {
        board.fill(0, i, 3);
    }
    for (int i = 1; i < 9; i++) { // 다 안채워 져서 안지워지는 라인
        board.fill(1, i, 3);
    }

    EXPECT_TRUE(board.is_line_full(0));
    EXPECT_FALSE(board.is_line_full(1));
}

TEST(BoardTest, GetActiveMino)
{
    Board board;
    board.spawn_mino(3);

    Tetromino& t = board.get_active_mino();
    EXPECT_EQ(t.get_mino_type(), 3);
}
TEST(BoardTest, DeleteMino)
{
    Board board;
    board.spawn_mino(3);
    for (int i = 0; i < 9; i++) { // 다 안채워진 라인
        board.fill(21, i, 3);
    }

    board.delete_line(21); // 다 안채워진 라인

    EXPECT_FALSE(board.is_filled(21, 3));
    EXPECT_FALSE(board.is_filled(21, 9));

    for (int i = 0; i < 10; i++) {
        board.fill(20, i, 3);
    }
    board.fill(19, 5, 1);
    board.delete_line(21);
    EXPECT_TRUE(board.is_filled(20, 5));
    EXPECT_FALSE(board.is_filled(20, 6));
}
TEST(BoardTest, InsertLine)
{
    Board board;
    board.insert_line(22);
    EXPECT_TRUE(board.is_filled(22, 5));
    int t = 0;
    for (int i = 0; i < 10; i++) {
        if (board.is_filled(22, i)) t++;
    }
    EXPECT_EQ(t, 10);
}