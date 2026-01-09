#ifndef __TETROMINO_WRAPPER_HPP__
#define __TETROMINO_WRAPPER_HPP__

#include <cstdint>

extern "C"
{
    struct TetrominoWrapper
    {
    private:
        uint16_t shape;
        int x;
        int y;
    };
}
#endif
