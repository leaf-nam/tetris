#ifndef __TEXT_RENDERER_HPP__
#define __TEXT_RENDERER_HPP__

#include "color_picker.hpp"
#include "console_renderer.hpp"

class TextRenderer
{
  private:
    ConsoleRenderer console_renderer;
    ColorPicker color_picker;

  public:
    TextRenderer(ConsoleRenderer, ColorPicker);
    void print_big_char(Pos, char, Color);
    void print_big_char(Pos, char);
    void print_big_string(Pos, std::string&, Color);
    void print_big_string(Pos, std::string&);
    void print_big_string(Pos pos, const char* str, Color key);
    void print_big_string(Pos, const char*);
    void print_small_string(Pos, std::string&, Color);
    void print_small_string(Pos, std::string&);
    void print_small_string(Pos pos, const char* str);
    void draw_game_start_count(int count);
    void draw_game_over(Pos pos);
    void draw_logo(Pos pos);
};

const char* const BIG_FONT[26][5] = {
    // A
    {" ███ ", "█   █", "█████", "█   █", "█   █"},
    // B
    {"████ ", "█   █", "████ ", "█   █", "████ "},
    // C
    {" ████", "█    ", "█    ", "█    ", " ████"},
    // D
    {"████ ", "█   █", "█   █", "█   █", "████ "},
    // E
    {"█████", "█    ", "███  ", "█    ", "█████"},
    // F
    {"█████", "█    ", "███  ", "█    ", "█    "},
    // G
    {" ████", "█    ", "█  ██", "█   █", " ████"},
    // H
    {"█   █", "█   █", "█████", "█   █", "█   █"},
    // I
    {"███", " █ ", " █ ", " █ ", "███"},
    // J
    {"  ███", "   █ ", "   █ ", "█  █ ", " ██  "},
    // K
    {"█   █", "█  █ ", "███  ", "█  █ ", "█   █"},
    // L
    {"█    ", "█    ", "█    ", "█    ", "█████"},
    // M
    {"█   █", "██ ██", "█ █ █", "█   █", "█   █"},
    // N
    {"█   █", "██  █", "█ █ █", "█  ██", "█   █"},
    // O
    {" ███ ", "█   █", "█   █", "█   █", " ███ "},
    // P
    {"████ ", "█   █", "████ ", "█    ", "█    "},
    // Q
    {" ███ ", "█   █", "█ █ █", "█  ██", " ████"},
    // R
    {"████ ", "█   █", "████ ", "█  █ ", "█   █"},
    // S
    {" ████", "█    ", " ███ ", "    █", "████ "},
    // T
    {"█████", "  █  ", "  █  ", "  █  ", "  █  "},
    // U
    {"█   █", "█   █", "█   █", "█   █", " ███ "},
    // V
    {"█   █", "█   █", "█   █", " █ █ ", "  █  "},
    // W
    {"█   █", "█   █", "█ █ █", "██ ██", "█   █"},
    // X
    {"█   █", " █ █ ", "  █  ", " █ █ ", "█   █"},
    // Y
    {"█   █", " █ █ ", "  █  ", "  █  ", "  █  "},
    // Z
    {"█████", "   █ ", "  █  ", " █   ", "█████"}};

const char* const BIG_NUMBER[10][5] = {
    // 0
    {" ███ ", "█   █", "█   █", "█   █", " ███ "},
    // 1
    {"  █  ", " ██  ", "  █  ", "  █  ", " ███ "},
    // 2
    {" ███ ", "█   █", "   █ ", " █   ", "█████"},
    // 3
    {"████ ", "    █", " ███ ", "    █", "████ "},
    // 4
    {"█   █", "█   █", "█████", "    █", "    █"},
    // 5
    {"█████", "█    ", "████ ", "    █", "████ "},
    // 6
    {" ███ ", "█    ", "████ ", "█   █", " ███ "},
    // 7
    {"█████", "    █", "   █ ", "  █  ", "  █  "},
    // 8
    {" ███ ", "█   █", " ███ ", "█   █", " ███ "},
    // 9
    {" ███ ", "█   █", " ████", "    █", " ███ "}};

#endif // !__TEXT_RENDERER_HPP__
