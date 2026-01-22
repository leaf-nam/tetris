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
    set(ColorKey::BACKGROUND, {30, 32, 45}); // 더 어둡게
    set(ColorKey::PANEL, {60, 63, 90});      // 보라 기 강조
    set(ColorKey::FOREGROUND, {245, 245, 245});
    set(ColorKey::COMMENT, {140, 150, 200});

    set(ColorKey::CYAN, {120, 220, 255});
    set(ColorKey::GREEN, {80, 255, 150});
    set(ColorKey::ORANGE, {255, 180, 90});
    set(ColorKey::PINK, {255, 110, 200});
    set(ColorKey::PURPLE, {200, 160, 255});
    set(ColorKey::RED, {255, 90, 90});
    set(ColorKey::YELLOW, {255, 240, 120});
}

void Theme::set_monokai_theme()
{
    set(ColorKey::BACKGROUND, {25, 25, 25}); // 거의 블랙
    set(ColorKey::PANEL, {60, 60, 60});
    set(ColorKey::FOREGROUND, {250, 250, 240});
    set(ColorKey::COMMENT, {150, 140, 120});

    set(ColorKey::CYAN, {100, 220, 220});
    set(ColorKey::GREEN, {180, 255, 80});
    set(ColorKey::ORANGE, {255, 170, 60});
    set(ColorKey::PINK, {255, 60, 120});
    set(ColorKey::PURPLE, {190, 140, 255});
    set(ColorKey::RED, {255, 80, 80});
    set(ColorKey::YELLOW, {255, 230, 120});
}

void Theme::set_nord_theme()
{
    set(ColorKey::BACKGROUND, {20, 30, 45}); // 블루톤 강조
    set(ColorKey::PANEL, {45, 65, 95});
    set(ColorKey::FOREGROUND, {225, 235, 245});
    set(ColorKey::COMMENT, {150, 180, 210});

    set(ColorKey::CYAN, {120, 200, 220});
    set(ColorKey::GREEN, {160, 210, 160});
    set(ColorKey::ORANGE, {240, 160, 120});
    set(ColorKey::PINK, {210, 150, 210});
    set(ColorKey::PURPLE, {150, 200, 200});
    set(ColorKey::RED, {230, 110, 120});
    set(ColorKey::YELLOW, {255, 210, 120});
}

void Theme::set_solarized_dark_theme()
{
    set(ColorKey::BACKGROUND, {0, 30, 40}); // 더 어둡게
    set(ColorKey::PANEL, {0, 60, 70});
    set(ColorKey::FOREGROUND, {180, 200, 200});
    set(ColorKey::COMMENT, {110, 140, 140});

    set(ColorKey::CYAN, {50, 200, 180});
    set(ColorKey::GREEN, {150, 200, 60});
    set(ColorKey::ORANGE, {255, 140, 60});
    set(ColorKey::PINK, {220, 80, 160});
    set(ColorKey::PURPLE, {140, 140, 220});
    set(ColorKey::RED, {255, 90, 90});
    set(ColorKey::YELLOW, {255, 220, 120});
}

string THEME[4] = {"DRACULA", "MONOKAI", "NORD", "SOLARIZED_DARK"};
