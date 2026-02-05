#include "render/window_setting_renderer.hpp"

using namespace std;

SettingRenderer::SettingRenderer(Setting* a1, IPlatformRenderer* a2, TextRenderer a3,
                                 BlockRenderer a4, InputWindowRenderer a5)
    : setting(a1), platform_renderer(a2), text_renderer(a3), block_renderer(a4),
      input_window_renderer(a5)
{
}

void SettingRenderer::render_settings_frame()
{
    platform_renderer->clear();
    platform_renderer->hide_cursor();
    text_renderer.print_big_string({20, 7}, "SETTINGS");
    if (setting->shadow_on) render_shadow();
    render_side();
}

void SettingRenderer::render_side()
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

void SettingRenderer::render_shadow()
{
    // 왼쪽 블럭 그리기
    Tetromino tetromino;
    int x = 3;
    int y = 3;
    for (int i = 0; i < 3; i++) {
        tetromino.set_mino_type(i);
        block_renderer.render_mino_pattern({x, y + 9 * i + 3}, tetromino, Color::COMMENT,
                                           Color::BACKGROUND, true);
    }

    // 오른쪽 블럭 그리기
    x = 81;
    for (int i = 3; i < 7; i++) {
        tetromino.set_mino_type(i);
        block_renderer.render_mino_pattern({x, y + 7 * (i - 3)}, tetromino, Color::COMMENT,
                                           Color::BACKGROUND, true);
    }
}

void SettingRenderer::render_input_window(Pos pos, string&& text)
{
    input_window_renderer.render_input_window(pos, text);
}

void SettingRenderer::render_settings(SettingTitle title)
{
    for (int i = 0; i < 4; i++) {
        platform_renderer->set_cursor(25, 18 + i * 2);

        int title_num = static_cast<int>(title);
        Color color = (title_num == i) ? Color::CYAN : Color::COMMENT;
        string str = (title_num == i) ? " > " : "   ";
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
