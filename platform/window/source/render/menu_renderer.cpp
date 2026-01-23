#include "render/menu_renderer.hpp"

#include <conio.h>
#include <string>
#include <wtypes.h>

using namespace std;
using namespace color;

MenuRenderer::MenuRenderer(Setting setting) : WindowRenderer(setting) {}
void MenuRenderer::render_menu_frame()
{
    hide_cursor();
    render_clear();
    draw_logo(20, 7);
}

void MenuRenderer::render_menu(Menu menu)
{
    for (int i = 0; i < 4; i++) {
        set_cursor(25, 18 + i * 2);

        int menu_num = static_cast<int>(menu);
        ColorKey color = (menu_num == i) ? ColorKey::YELLOW : ColorKey::COMMENT;
        string str = (menu_num == i) ? " > " : "   ";
        str += MENU[i];
        print_s(str, color);
    }
}

void MenuRenderer::render_settings_frame()
{
    hide_cursor();
    render_clear();
    print_big_string({20, 7}, "SETTINGS");
    render_side();
}

void MenuRenderer::render_side()
{
    // 왼쪽 블럭 그리기
    Tetromino* tetromino = new Tetromino();
    int x = 2;
    int y = 2;
    for (int i = 0; i < 3; i++) {
        tetromino->set_mino_type(i);
        render_mino_pattern({x, y + 9 * i + 3}, *tetromino);
    }

    // 오른쪽 블럭 그리기
    x = 80;
    for (int i = 3; i < 7; i++) {
        tetromino->set_mino_type(i);
        render_mino_pattern({x, y + 7 * (i - 3)}, *tetromino);
    }
}

void MenuRenderer::render_settings(SettingMenu menu, Setting& setting)
{
    for (int i = 0; i < 4; i++) {
        set_cursor(25, 18 + i * 2);

        int menu_num = static_cast<int>(menu);
        ColorKey color = (menu_num == i) ? ColorKey::CYAN : ColorKey::COMMENT;
        string str = (menu_num == i) ? " > " : "   ";
        str += SETTING_MENU[i];

        string value;

        switch (static_cast<SettingMenu>(i)) {
        case SettingMenu::NICKNAME:
            value = " : " + setting.nick_name;
            break;
        case SettingMenu::THEME:
            value = " : " + THEME[static_cast<int>(setting.color_theme)];
            break;
        case SettingMenu::SHADOW:
            value = setting.shadow_on ? " : ON " : " : OFF";
            break;
        default:
            break;
        }

        str += value;
        print_s(str, color);
    }
}
