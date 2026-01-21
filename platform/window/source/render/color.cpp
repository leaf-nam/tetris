#include "render/color.hpp"

using namespace std;

Theme& Theme::getInstance()
{
    static Theme t;
    return t;
}

Theme::Theme() { apply(ThemeKey::DRACULA); }

void Theme::apply(ThemeKey key)
{
    colors.clear();
    switch (key) {
    case ThemeKey::DRACULA:
        set_dracula_theme();
        break;
    case ThemeKey::MONOKAI:
        set_monokai_theme();
        break;
    case ThemeKey::NORD:
        set_nord_theme();
        break;
    case ThemeKey::SOLARIZED_DARK:
        set_solarized_dark_theme();
        break;
    }
}

void Theme::set(ColorKey key, RGB color) { colors[key] = color; }

string Theme::color(ColorKey key, bool background) const
{
    auto it = colors.find(key);
    if (it == colors.end()) return "";
    return to_ansi(it->second, background);
}

void Theme::set_dracula_theme()
{
    set(ColorKey::Background, {30, 32, 45}); // 더 어둡게
    set(ColorKey::Panel, {60, 63, 90});      // 보라 기 강조
    set(ColorKey::Foreground, {245, 245, 245});
    set(ColorKey::Comment, {140, 150, 200});

    set(ColorKey::Cyan, {120, 220, 255});
    set(ColorKey::Green, {80, 255, 150});
    set(ColorKey::Orange, {255, 180, 90});
    set(ColorKey::Pink, {255, 110, 200});
    set(ColorKey::Purple, {200, 160, 255});
    set(ColorKey::Red, {255, 90, 90});
    set(ColorKey::Yellow, {255, 240, 120});
}

void Theme::set_monokai_theme()
{
    set(ColorKey::Background, {25, 25, 25}); // 거의 블랙
    set(ColorKey::Panel, {60, 60, 60});
    set(ColorKey::Foreground, {250, 250, 240});
    set(ColorKey::Comment, {150, 140, 120});

    set(ColorKey::Cyan, {100, 220, 220});
    set(ColorKey::Green, {180, 255, 80});
    set(ColorKey::Orange, {255, 170, 60});
    set(ColorKey::Pink, {255, 60, 120});
    set(ColorKey::Purple, {190, 140, 255});
    set(ColorKey::Red, {255, 80, 80});
    set(ColorKey::Yellow, {255, 230, 120});
}

void Theme::set_nord_theme()
{
    set(ColorKey::Background, {20, 30, 45}); // 블루톤 강조
    set(ColorKey::Panel, {45, 65, 95});
    set(ColorKey::Foreground, {225, 235, 245});
    set(ColorKey::Comment, {150, 180, 210});

    set(ColorKey::Cyan, {120, 200, 220});
    set(ColorKey::Green, {160, 210, 160});
    set(ColorKey::Orange, {240, 160, 120});
    set(ColorKey::Pink, {210, 150, 210});
    set(ColorKey::Purple, {150, 200, 200});
    set(ColorKey::Red, {230, 110, 120});
    set(ColorKey::Yellow, {255, 210, 120});
}

void Theme::set_solarized_dark_theme()
{
    set(ColorKey::Background, {0, 30, 40}); // 더 어둡게
    set(ColorKey::Panel, {0, 60, 70});
    set(ColorKey::Foreground, {180, 200, 200});
    set(ColorKey::Comment, {110, 140, 140});

    set(ColorKey::Cyan, {50, 200, 180});
    set(ColorKey::Green, {150, 200, 60});
    set(ColorKey::Orange, {255, 140, 60});
    set(ColorKey::Pink, {220, 80, 160});
    set(ColorKey::Purple, {140, 140, 220});
    set(ColorKey::Red, {255, 90, 90});
    set(ColorKey::Yellow, {255, 220, 120});
}

extern const char* THEME[4] = {"DRACULA", "MONOKAI", "NORD", "SOLARIZED_DARK"};
