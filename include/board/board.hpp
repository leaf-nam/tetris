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

    void update_board();
    void draw_board();
    void draw_mino();
    bool can_place_mino(int new_r, int new_c, int new_rot);

    public:
    Board();
    bool has_active_mino();
    void move_mino(int cmd);
    bool spawn_mino(int type);
    bool is_line_full(int row);
    const uint16_t* get_board() const;
    void delete_line(int del_row);
    bool insert_line(int ins_row);
    void render(); // 추후 UI 전용 객체로 이전
};

#endif
