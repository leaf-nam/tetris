#ifndef __ACTION_HPP__
#define __ACTION_HPP__

#include <cstdint>

enum Action : std::uint8_t
{
    LEFT = 0,
    RIGHT,
    DROP,
    ROTATE_CW,
    ROTATE_CCW,
    HARD_DROP,
    SWAP,
    INVALID,
};

#endif
