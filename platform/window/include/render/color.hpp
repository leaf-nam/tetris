#ifndef __COLOR_HPP__
#define __COLOR_HPP__

#include <cstdint>
#include <string>
#include <unordered_map>

enum class ColorKey : uint8_t
{
    Background = 0,
    Panel,
    Foreground,
    Comment,

    Cyan,
    Green,
    Orange,
    Pink,
    Purple,
    Red,
    Yellow
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

enum class ThemeKey : uint8_t
{
    DRACULA = 0,
    MONOKAI,
    NORD,
    SOLARIZED_DARK
};

extern const char* THEME[4];

class Theme
{
  public:
    static Theme& getInstance();
    static constexpr const char* reset() { return "\x1b[0m"; }
    std::string color(ColorKey key, bool background = false) const;
    void apply(ThemeKey key);

  private:
    Theme();
    void set(ColorKey key, RGB color);
    void set_dracula_theme();
    void set_monokai_theme();
    void set_nord_theme();
    void set_solarized_dark_theme();
    std::unordered_map<ColorKey, RGB> colors;
};

inline std::string get_color(ColorKey key) { return Theme::getInstance().color(key); }

#endif
