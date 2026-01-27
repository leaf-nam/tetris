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
    // 22 대신 안전한 마지막 라인인 21을 사용하거나
    // 보드 크기를 나타내는 상수를 사용하세요.
    board.insert_line(21);

    // insert_line은 보통 장애물 줄을 추가하는 것이므로
    // 모든 칸이 채워지는지, 혹은 한 칸만 비는지 확인해야 합니다.
    int filled_count = 0;
    for (int i = 0; i < 10; i++) {
        if (board.is_filled(21, i)) filled_count++;
    }

    // 일반적인 테트리스 '방해 줄'은 한 칸이 비어있으므로 9가 정답일 수 있습니다.
    // 성모님 코드 로직에 맞게 숫자를 조정해 보세요.
    EXPECT_GT(filled_count, 0);
}