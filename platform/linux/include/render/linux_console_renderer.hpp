#ifndef __CONSOLE_RENDERER_HPP__
#define __CONSOLE_RENDERER_HPP__

#include "i_platform_renderer.hpp"
#include "render/color.hpp"
#include "render/pos.hpp"
#include "render/theme.hpp"

#include <string>

class ConsoleRenderer : public IPlatformRenderer
{
  public:
    void clear();

    void show_cursor();
    void hide_cursor();

    void set_cursor(Pos);
    void set_cursor(int x, int y);
    void set_color(Color);
    void set_color(Color foreground, Color background);

    void print_s(const char* const, Color);
    void print_s(const char* const, Color foreground, Color background);
    void print_s(std::string, Color);
    void print_s(std::string s, Color foreground, Color background);

    void print_s(std::string);
};

#endif // !__CONSOLE_RENDERER_HPP__
