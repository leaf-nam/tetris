#ifndef __BOARD_HPP__
#define __BOARD_HPP__

#include <chrono>
#include <iostream>
#include "tetromino/tetromino.hpp"

inline constexpr int BOARD_ROW = 22;
inline constexpr int BOARD_COL = 10;
inline constexpr int BOARD_UPPER = 2;

typedef int board_row[BOARD_COL];
typedef board_row board_t[BOARD_ROW];


class Board
{
    private:
    Tetromino active_mino;
    Tetromino saved_mino;
    bool is_mino_swaped;
    board_t game_board;
    bool is_mino_active;
    
    public:
    Board();

    std::pair<int, int> get_active_mino_pos();
    int get_active_mino_rotation();
    
    void set_active_mino_pos(int new_r, int new_c);
    void set_active_mino_rotation(int new_rot);

    const bool has_active_mino() const;
    const bool has_swaped_mino() const;

    bool can_place_mino(int new_r, int new_c, int new_rot);
    void update_board();

    bool spawn_mino(int type);
    void swap_mino();

    bool is_line_full(int row);
    
    Tetromino& get_active_mino();
    Tetromino& get_saved_mino();
    bool get_is_mino_swaped();
    const board_t& get_board() const;
    const bool is_filled(int r, int c) const;
    const int at(int r, int c) const;
    bool fill(int r, int c, int type);

    void delete_line(int del_row);
    bool insert_line(int ins_row);
};

#endif
