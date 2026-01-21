#include "render/menu_renderer.hpp"

#include <conio.h>
#include <string>
#include <wtypes.h>

using namespace std;
using namespace color;

void MenuRenderer::render_menu_frame()
{
    hide_cursor();
    render_clear();
    draw_logo(20, 7);
}

static string menu_string[4] = {"single play", "multi play", "settings", "exit"};
void MenuRenderer::render_menu(Menu menu_num)
{
    for (int i = 0; i < 4; i++) {
        set_cursor(25, 18 + i * 2);

        string arrow = (static_cast<uint8_t>(menu_num) == i) ? " > " : "   ";
        cout << arrow << menu_string[i] << '\n';

        printf(RESET);
    }
}

void MenuRenderer::render_settings_frame()
{
    hide_cursor();
    render_clear();
    draw_logo(20, 7);
    print_big_string({20, 7}, "SETTINGS");

    // 왼쪽 블럭 그리기
    Tetromino* tetromino = new Tetromino();
    int x = 2;
    int y = 2;
    tetromino->set_mino_type(0);
    render_mino_pattern(x, y + 4, tetromino->get_shape(),
                        get_block_color(tetromino->get_mino_type()));
    for (int i = 1; i < 3; i++) {
        tetromino->set_mino_type(i);

        render_mino_pattern(x, y + 9 * i + 3, tetromino->get_shape(),
                            get_block_color(tetromino->get_mino_type()));
    }

    // 오른쪽 블럭 그리기
    x = 80;
    for (int i = 3; i < 7; i++) {
        tetromino->set_mino_type(i);

        render_mino_pattern(x, y + 7 * (i - 3), tetromino->get_shape(),
                            get_block_color(tetromino->get_mino_type()));
    }
}

void MenuRenderer::render_settings(SettingMenu menu_num, Setting& setting)
{
    for (int i = 0; i < 4; i++) {
        set_cursor(25, 18 + i * 2);

        string arrow = (static_cast<uint8_t>(menu_num) % 4 == i) ? " > " : "   ";
        string value;

        switch (static_cast<SettingMenu>(i)) {
        case SettingMenu::NICKNAME:
            value = " : " + setting.nick_name;
            break;
        case SettingMenu::THEME:
            value = " : " + to_string(static_cast<int>(setting.color_theme));
            break;
        case SettingMenu::SHADOW:
            value = setting.shadow_on ? " : ON " : " : OFF";
            break;
        default:
            break;
        }
        cout << arrow << SETTING_MENU[i] << value << '\n';

        printf(RESET);
    }
}
