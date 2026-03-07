#ifndef __ENTRANCE_HPP__
#define __ENTRANCE_HPP__

#include <string>

enum class Entrance
{
    CREATE_ROOM,
    ENTER_ROOM,
    EXIT,

    COUNT
};

constexpr int ENTRANCE_COUNT = static_cast<int>(Entrance::COUNT);
const std::string ENTRANCE_TITLE[ENTRANCE_COUNT] = {"CREATE ROOM", "ENTER ROOM", "EXIT"};

inline Entrance& operator++(Entrance& rhs, int)
{
    rhs = static_cast<Entrance>((static_cast<int>(rhs) + 1) % ENTRANCE_COUNT);
    return rhs;
}

inline Entrance& operator--(Entrance& rhs, int)
{
    rhs = static_cast<Entrance>((static_cast<int>(rhs) - 1 + ENTRANCE_COUNT) % ENTRANCE_COUNT);
    return rhs;
}
#endif //__ENTRANCE_HPP__
