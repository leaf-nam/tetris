#ifndef __COLOR_HPP__
#define __COLOR_HPP__

#include <cstdint>
#include <string>
#include <unordered_map>

enum class Color : uint8_t
{
    BACKGROUND = 0,
    PANEL,
    FOREGROUND,
    COMMENT,

    CYAN,
    GREEN,
    ORANGE,
    PINK,
    PURPLE,
    RED,
    YELLOW
};

struct RGB
{
    uint8_t r, g, b;
};

inline std::string to_ansi(const RGB& c, bool background = false)
{
    return "\x1b[" + std::string(background ? "48" : "38") + ";2;" + std::to_string(c.r) + ";" +
           std::to_string(c.g) + ";" + std::to_string(c.b) + "m";
}

#endif
