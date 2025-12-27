#ifndef __BOARD_HPP__
#define __BOARD_HPP__

#include <chrono>
#include <iostream>
#include "tetromino/tetromino.hpp"

class Board
{
    private:
    Tetromino active_mino;
    mino game_board[22];
    bool is_mino_active;

    public:
    Board();
    void move_mino(int cmd);
    void spawn_mino(int type);
    void update_board();
    void draw_board();
};

#endif