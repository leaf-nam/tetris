#ifndef __TETROMINO_HPP__
#define __TETROMINO_HPP__

#include <cstdint>
#include <cstddef>

typedef std::uint16_t mino;

extern mino *TETROMINO[7];

class Tetromino
{
    private:
    mino *shape;
    int rotation;
    int x, y;

    public:
    Tetromino(unsigned int block_type);
    void rotate(int rot);
    void move(int dir);
};

#endif