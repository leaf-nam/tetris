#ifndef __CONSOLE_RENDERER_HPP__
#define __CONSOLE_RENDERER_HPP__

#include "color.hpp"
#include "pos.hpp"
#include "theme.hpp"

#include <string>

class ConsoleRenderer
{
  public:
    ConsoleRenderer() = default;
    void clear();

    void show_cursor();
    void hide_cursor();
    void resize(int, int);

    void set_cursor(Pos);
    void set_cursor(int, int);
    void set_color(Color);
    void set_color(Color, Color);

    void print_s(const char* const, Color);
    void print_s(const char* const, Color, Color);
    void print_s(std::string, Color);

    // 랜덤 색상 출력
    void print_s(std::string);
};

#endif // !__CONSOLE_RENDERER_HPP__
