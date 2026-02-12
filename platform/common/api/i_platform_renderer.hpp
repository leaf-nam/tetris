#ifndef __I_PLATFORM_RENDERER_HPP__
#define __I_PLATFORM_RENDERER_HPP__

#include "render/color.hpp"
#include "render/pos.hpp"

class IPlatformRenderer
{
  public:
    virtual void clear() = 0;

    virtual void show_cursor() = 0;
    virtual void hide_cursor() = 0;

    virtual void set_cursor(Pos) = 0;
    virtual void set_cursor(int x, int y) = 0;
    virtual void set_color(Color) = 0;
    virtual void set_color(Color foreground, Color background) = 0;

    virtual void print_s(const char* const, Color) = 0;
    virtual void print_s(const char* const, Color foreground, Color background) = 0;
    virtual void print_s(std::string, Color) = 0;
    virtual void print_s(std::string s, Color foreground, Color background) = 0;

    // 랜덤 색상 출력
    virtual void print_s(std::string) = 0;
};

#endif //!__I_PLATFORM_RENDERER_HPP__
