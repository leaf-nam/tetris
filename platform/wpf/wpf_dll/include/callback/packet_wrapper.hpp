#ifndef __PACKET_WRAPPER_HPP__
#define __PACKET_WRAPPER_HPP__

#include <cstdint>

extern "C"
{
    struct TetrominoWrapper
    {
    public:
        int type;
        int rotation;
        int x;
        int y;
    };
}
#endif
