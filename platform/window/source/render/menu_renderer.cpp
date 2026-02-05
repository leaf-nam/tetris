#include "render/menu_renderer.hpp"

#include "render/theme.hpp"

#include <conio.h>
#include <string>
#include <wtypes.h>

using namespace std;

MenuRenderer::MenuRenderer(Setting* a1, IPlatformRenderer* a2, TextRenderer a3, BlockRenderer a4)
    : setting(a1), platform_renderer(a2), text_renderer(a3), block_renderer(a4)
{
}

void MenuRenderer::render_menu_frame()
{
    platform_renderer->clear();
    platform_renderer->hide_cursor();
    text_renderer.draw_logo({20, 7});
}

void MenuRenderer::render_menu(MenuTitle menu)
{
    for (int i = 0; i < 4; i++) {
        platform_renderer->set_cursor(25, 18 + i * 2);

        int menu_num = static_cast<int>(menu);
        Color color = (menu_num - 1 == i) ? Color::YELLOW : Color::COMMENT;
        string str = (menu_num - 1 == i) ? " > " : "   ";
        str += MENU_TITLE[i];
        platform_renderer->print_s(str, color);
    }
}

void MenuRenderer::render_settings_frame()
{
    platform_renderer->clear();
    platform_renderer->hide_cursor();
    text_renderer.print_big_string({20, 7}, "SETTINGS");
    if (setting->shadow_on) render_shadow();
    render_side();
}

void MenuRenderer::render_side()
{
    // 왼쪽 블럭 그리기
    Tetromino tetromino;
    int x = 2;
    int y = 2;
    for (int i = 0; i < 3; i++) {
        tetromino.set_mino_type(i);
        block_renderer.render_mino_pattern({x, y + 9 * i + 3}, tetromino, true);
    }

    // 오른쪽 블럭 그리기
    x = 80;
    for (int i = 3; i < 7; i++) {
        tetromino.set_mino_type(i);
        block_renderer.render_mino_pattern({x, y + 7 * (i - 3)}, tetromino, true);
    }
}

void MenuRenderer::render_shadow()
{
    // 왼쪽 블럭 그리기
    Tetromino tetromino;
    int x = 3;
    int y = 3;
    for (int i = 0; i < 3; i++) {
        tetromino.set_mino_type(i);
        block_renderer.render_mino_pattern({x, y + 9 * i + 3}, tetromino, Color::COMMENT, true);
    }

    // 오른쪽 블럭 그리기
    x = 81;
    for (int i = 3; i < 7; i++) {
        tetromino.set_mino_type(i);
        block_renderer.render_mino_pattern({x, y + 7 * (i - 3)}, tetromino, Color::COMMENT, true);
    }
}

void MenuRenderer::render_settings(SettingTitle menu)
{
    for (int i = 0; i < 4; i++) {
        platform_renderer->set_cursor(25, 18 + i * 2);

        int menu_num = static_cast<int>(menu);
        Color color = (menu_num == i) ? Color::CYAN : Color::COMMENT;
        string str = (menu_num == i) ? " > " : "   ";
        str += SETTING_TITLE[i];

        string value;

        switch (static_cast<SettingTitle>(i)) {
        case SettingTitle::NICKNAME:
            value = " : " + setting->nick_name;
            break;
        case SettingTitle::THEME:
            value = " : " + THEME[static_cast<int>(setting->color_theme)];
            break;
        case SettingTitle::SHADOW:
            value = setting->shadow_on ? " : ON " : " : OFF";
            break;
        default:
            break;
        }

        str += value;
        platform_renderer->print_s(str, color);
    }
}
