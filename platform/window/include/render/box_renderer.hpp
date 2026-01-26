#ifndef __BOX_RENDERER_HPP__
#define __BOX_RENDERER_HPP__

#include "color.hpp"
#include "console_renderer.hpp"
#include "pos.hpp"

class BoxRenderer
{
  private:
    ConsoleRenderer console_renderer;

  public:
    BoxRenderer();
    BoxRenderer(ConsoleRenderer);
    void draw_box(Pos pos, int w, int h, const std::string& title, Color color);
    void draw_rect(Pos pos, int w, int h, Color color);
};
#endif // !__DISPLAY_RENDERER_HPP__
