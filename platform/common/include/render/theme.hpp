#ifndef __THEME_HPP__
#define __THEME_HPP__

#include "color.hpp"

enum class ThemeKey : uint8_t
{
    DRACULA = 0,
    MONOKAI,
    NORD,
    SOLARIZED_DARK
};

extern std::string THEME[4];

class Theme
{
  public:
    static Theme& getInstance();
    static constexpr const char* reset() { return "\x1b[0m"; }
    std::string color(Color key, bool background = false) const;
    void apply(ThemeKey key);

  private:
    Theme();
    void set(Color key, RGB color);
    void set_dracula_theme();
    void set_monokai_theme();
    void set_nord_theme();
    void set_solarized_dark_theme();
    std::unordered_map<Color, RGB> colors;
};

inline std::string get_color(Color key) { return Theme::getInstance().color(key); }
inline std::string get_color(Color key, bool background)
{
    return Theme::getInstance().color(key, background);
}

#endif // !__THEME_HPP__
