#ifndef __TETROMINOQUEUE_HPP__
#define __TETROMINOQUEUE_HPP__

#include "tetromino/tetromino.hpp"
#include "util/rand_gen.hpp"
#include <algorithm>
#include <random>

class TetrominoQueue
{
    private:
    int curr_tetrominos[7];
    int next_tetrominos[7];
    int draw_count;
    std::mt19937 rng{std::random_device{}()};
    TetrominoQueue();

    public:
    static TetrominoQueue& get_instance();
	TetrominoQueue(const TetrominoQueue&) = delete;
	TetrominoQueue& operator=(const TetrominoQueue&) = delete;
    void init();
    int get_new_tetromino();
    void set_new_tetromino(int new_tetromino);
    const int* get_tetrominos();
};

#endif
