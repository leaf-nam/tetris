#ifndef __TETROMINO_HPP__
#define __TETROMINO_HPP__

#include <cstdint>
#include <cstddef>
#include <utility>

typedef std::uint16_t mino;

extern mino *TETROMINO[7];

class Tetromino
{
    private:
    int mino_type;
    int rotation;
    std::pair<int, int> pos;

    public:
    Tetromino();
    void init_mino(int type);
    void set_rotation(int rot);
    void set_pos(int new_r, int new_c);
    std::pair<int, int> get_pos();
    mino get_shape();
    mino get_shape(int rot);
    int get_rotation();
};

#endif