#include "render/text_renderer.hpp"

using namespace std;

TextRenderer::TextRenderer(ConsoleRenderer a1, ColorPicker a2)
    : console_renderer(a1), color_picker(a2)
{
}

void TextRenderer::print_big_char(Pos pos, char c, Color key)
{
    if (c >= 'A' && c <= 'Z') {
        for (int i = 0; i < 5; ++i) {
            console_renderer.set_cursor(pos.x, pos.y + i);
            console_renderer.print_s(BIG_FONT[c - 'A'][i], key);
        }
    }

    else if (c >= '0' && c <= '9') {
        for (int i = 0; i < 5; ++i) {
            console_renderer.set_cursor(pos.x, pos.y + i);
            console_renderer.print_s(BIG_NUMBER[c - '0'][i], key);
        }
    }
}

void TextRenderer::print_big_char(Pos pos, char c)
{
    print_big_char(pos, c, color_picker.get_random_color());
}

void TextRenderer::print_big_string(Pos pos, string& str, Color key)
{
    for (int j = 0; j < str.size(); ++j) {
        char c = str[j];
        Pos next{pos.x + j * 6, pos.y};
        print_big_char(next, c, key);
    }
}

void TextRenderer::print_big_string(Pos pos, string& str)
{
    for (int j = 0; j < str.size(); ++j) {
        Color random_color = color_picker.get_random_color();
        char c = str[j];

        if (c >= 'A' && c <= 'Z') {
            for (int i = 0; i < 5; ++i) {
                console_renderer.set_cursor(pos.x + j * 6, pos.y + i);
                console_renderer.print_s(BIG_FONT[c - 'A'][i], random_color);
            }
        }

        else if (c >= '0' && c <= '9') {
            for (int i = 0; i < 5; ++i) {
                console_renderer.set_cursor(pos.x + j * 6, pos.y + i);
                console_renderer.print_s(BIG_NUMBER[c - '0'][i], random_color);
            }
        }
    }
}

void TextRenderer::print_big_string(Pos pos, const char* str, Color key)
{
    string s;
    s.assign(str);
    print_big_string(pos, s, key);
}

void TextRenderer::print_big_string(Pos pos, const char* str)
{
    string s;
    s.assign(str);
    print_big_string(pos, s);
}

void TextRenderer::print_small_string(Pos pos, string& str, Color key)
{
    console_renderer.set_cursor(pos.x, pos.y);
    console_renderer.print_s(str, key);
}

void TextRenderer::print_small_string(Pos pos, string& str)
{
    print_small_string(pos, str, color_picker.get_random_color());
}

void TextRenderer::print_small_string(Pos pos, const char* str)
{
    string s;
    s.assign(str);
    print_small_string(pos, s);
}

void TextRenderer::draw_game_start_count(int count)
{
    string count_str = to_string(count);
    print_big_string({32, 15}, count_str);
}

void TextRenderer::draw_game_over(Pos pos)
{
    print_big_string({pos.x, pos.y}, "GAMEOVER", color_picker.get_random_color());
}

void TextRenderer::draw_logo(Pos pos)
{
    print_big_string({pos.x, pos.y}, "TETRISSEN");
    print_small_string({pos.x + 56, pos.y + 4}, "v1");
}
