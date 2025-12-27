#ifndef __BOARD_HPP__
#define __BOARD_HPP__

#include "tetromino.hpp"

class Board
{
    private:
    mino* active_mino;
    mino game_board[22];
    

    public:

    void move_mino(unsigned int cmd);
    void rotate_mino(unsigned int cmd);
};

#endif