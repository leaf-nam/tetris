#include <gtest/gtest.h>
#include "tetromino/tetromino_queue.hpp"

TEST(TetrominoQueueTest, all)
{
    TetrominoQueue& q = TetrominoQueue::get_instance();
    const int * buff;

    for (int i = 0; i < 8; ++i)
    {
        buff = q.get_tetrominos();
        int next = buff[0];
        EXPECT_EQ(q.get_new_tetromino(), next);
    }
}