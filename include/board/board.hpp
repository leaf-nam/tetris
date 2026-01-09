#ifndef __BOARD_HPP__
#define __BOARD_HPP__

#include <chrono>
#include <iostream>
#include "tetromino/tetromino.hpp"

enum MoveOption
{
    DISMISS_IF_FAIL = 0,
    FIX_IF_FAIL = 1
};

class Board
{
private:
    Tetromino active_mino;
    Tetromino saved_mino;
    bool is_mino_swaped;
    mino game_board[22];
    bool is_mino_active;

    bool can_move_mino(int new_r, int new_c, int new_rot);

public:
    Board();

    std::pair<int, int> get_active_mino_pos();
    int get_active_mino_rotation();
    int get_active_mino_type();

    const bool has_active_mino() const;
    const bool has_swaped_mino() const;

    bool move_active_mino(int new_r, int new_c, int new_rot, MoveOption move_option);
    void update_board();

    bool spawn_mino(int type);
    void swap_mino();

    bool is_line_full(int row);
    
    Tetromino& get_active_mino();
    Tetromino& get_saved_mino();
    bool get_is_mino_swaped();
    const uint16_t* get_board() const;

    void delete_line(int del_row);
    bool insert_line(int ins_row);
};

#endif
