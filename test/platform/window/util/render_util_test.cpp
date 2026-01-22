#include "board/board.hpp"
#include "util/shadow_maker.hpp"

#include <gtest/gtest.h>
#include <vector>

using namespace std;

/*
    1111


    1110000000
*/
TEST(DownI1, all)
{
    // given
    ShadowMaker sut;

    Board board;
    board.fill(BOARD_ROW - 1, 0, 1);
    board.fill(BOARD_ROW - 1, 1, 1);
    board.fill(BOARD_ROW - 1, 2, 1);

    Tetromino tetromino;
    tetromino.init_mino(0);
    tetromino.set_pos(0, 0);

    // when
    auto b = board.get_board();
    vector<Pos> shadows = sut.get_shadow_pos(board, tetromino);

    // then
    EXPECT_EQ(sut.isShadow(shadows, {0, BOARD_ROW - 2}), true);
    EXPECT_EQ(sut.isShadow(shadows, {1, BOARD_ROW - 2}), true);
    EXPECT_EQ(sut.isShadow(shadows, {2, BOARD_ROW - 2}), true);
    EXPECT_EQ(sut.isShadow(shadows, {3, BOARD_ROW - 2}), true);
    EXPECT_EQ(sut.isShadow(shadows, {4, BOARD_ROW - 2}), false);
    EXPECT_EQ(sut.isShadow(shadows, {5, BOARD_ROW - 2}), false);
    EXPECT_EQ(sut.isShadow(shadows, {6, BOARD_ROW - 2}), false);
    EXPECT_EQ(sut.isShadow(shadows, {7, BOARD_ROW - 2}), false);
    EXPECT_EQ(sut.isShadow(shadows, {8, BOARD_ROW - 2}), false);
    EXPECT_EQ(sut.isShadow(shadows, {9, BOARD_ROW - 2}), false);
    EXPECT_EQ(sut.isShadow(shadows, {10, BOARD_ROW - 2}), false);
}

/*
       1111


    1110000000
*/
TEST(DownI2, all)
{
    // given
    ShadowMaker sut;

    Board board;
    board.fill(BOARD_ROW - 1, 0, 1);
    board.fill(BOARD_ROW - 1, 1, 1);
    board.fill(BOARD_ROW - 1, 2, 1);

    Tetromino tetromino;
    tetromino.init_mino(0);
    tetromino.set_pos(0, 3);

    // when
    auto b = board.get_board();
    vector<Pos> shadows = sut.get_shadow_pos(board, tetromino);

    // then
    EXPECT_EQ(sut.isShadow(shadows, {0, BOARD_ROW - 1}), false);
    EXPECT_EQ(sut.isShadow(shadows, {1, BOARD_ROW - 1}), false);
    EXPECT_EQ(sut.isShadow(shadows, {2, BOARD_ROW - 1}), false);
    EXPECT_EQ(sut.isShadow(shadows, {3, BOARD_ROW - 1}), true);
    EXPECT_EQ(sut.isShadow(shadows, {4, BOARD_ROW - 1}), true);
    EXPECT_EQ(sut.isShadow(shadows, {5, BOARD_ROW - 1}), true);
    EXPECT_EQ(sut.isShadow(shadows, {6, BOARD_ROW - 1}), true);
    EXPECT_EQ(sut.isShadow(shadows, {7, BOARD_ROW - 1}), false);
    EXPECT_EQ(sut.isShadow(shadows, {8, BOARD_ROW - 1}), false);
    EXPECT_EQ(sut.isShadow(shadows, {9, BOARD_ROW - 1}), false);
    EXPECT_EQ(sut.isShadow(shadows, {10, BOARD_ROW - 1}), false);
}
