#ifndef __LOBBY_TITLE_HPP__
#define __LOBBY_TITLE_HPP__

#include <array>

enum class EntranceMenu
{
    CREATE_ROOM,
    ENTER_ROOM,
    COUNT
};

constexpr std::array<const char*, static_cast<uint8_t>(EntranceMenu::COUNT)> ENTRANCE_MENU_TITLE = {
    "CREATE ROOM", "ENTER ROOM"};

inline EntranceMenu operator+(EntranceMenu m, int value)
{
    constexpr int count = static_cast<int>(EntranceMenu::COUNT);
    int next = (static_cast<int>(m) + value + count) % count;

    return static_cast<EntranceMenu>(next);
}

inline EntranceMenu operator-(EntranceMenu m, int value) { return m + (-value); }

#endif
