#include "render/theme.hpp"

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

void Theme::set(Color key, RGB color) { colors[key] = color; }

string Theme::color(Color key, bool background) const
{
    auto it = colors.find(key);
    if (it == colors.end()) return "";
    return to_ansi(it->second, background);
}

void Theme::set_dracula_theme()
{
    set(Color::BACKGROUND, {30, 32, 45}); // 더 어둡게
    set(Color::PANEL, {60, 63, 90});      // 보라 기 강조
    set(Color::FOREGROUND, {245, 245, 245});
    set(Color::COMMENT, {140, 150, 200});

    set(Color::CYAN, {120, 220, 255});
    set(Color::GREEN, {80, 255, 150});
    set(Color::ORANGE, {255, 180, 90});
    set(Color::PINK, {255, 110, 200});
    set(Color::PURPLE, {200, 160, 255});
    set(Color::RED, {255, 90, 90});
    set(Color::YELLOW, {255, 240, 120});
}

void Theme::set_monokai_theme()
{
    set(Color::BACKGROUND, {25, 25, 25}); // 거의 블랙
    set(Color::PANEL, {60, 60, 60});
    set(Color::FOREGROUND, {250, 250, 240});
    set(Color::COMMENT, {150, 140, 120});

    set(Color::CYAN, {100, 220, 220});
    set(Color::GREEN, {180, 255, 80});
    set(Color::ORANGE, {255, 170, 60});
    set(Color::PINK, {255, 60, 120});
    set(Color::PURPLE, {190, 140, 255});
    set(Color::RED, {255, 80, 80});
    set(Color::YELLOW, {255, 230, 120});
}

void Theme::set_nord_theme()
{
    set(Color::BACKGROUND, {20, 30, 45}); // 블루톤 강조
    set(Color::PANEL, {45, 65, 95});
    set(Color::FOREGROUND, {225, 235, 245});
    set(Color::COMMENT, {150, 180, 210});

    set(Color::CYAN, {120, 200, 220});
    set(Color::GREEN, {160, 210, 160});
    set(Color::ORANGE, {240, 160, 120});
    set(Color::PINK, {210, 150, 210});
    set(Color::PURPLE, {150, 200, 200});
    set(Color::RED, {230, 110, 120});
    set(Color::YELLOW, {255, 210, 120});
}

void Theme::set_solarized_dark_theme()
{
    set(Color::BACKGROUND, {0, 30, 40}); // 더 어둡게
    set(Color::PANEL, {0, 60, 70});
    set(Color::FOREGROUND, {180, 200, 200});
    set(Color::COMMENT, {110, 140, 140});

    set(Color::CYAN, {50, 200, 180});
    set(Color::GREEN, {150, 200, 60});
    set(Color::ORANGE, {255, 140, 60});
    set(Color::PINK, {220, 80, 160});
    set(Color::PURPLE, {140, 140, 220});
    set(Color::RED, {255, 90, 90});
    set(Color::YELLOW, {255, 220, 120});
}

string THEME[4] = {"DRACULA", "MONOKAI", "NORD", "SOLARIZED_DARK"};
