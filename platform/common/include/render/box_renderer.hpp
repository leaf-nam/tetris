#ifndef __BOX_RENDERER_HPP__
#define __BOX_RENDERER_HPP__

#include "color.hpp"
#include "i_platform_renderer.hpp"
#include "pos.hpp"

class BoxRenderer
{
  private:
    IPlatformRenderer* platform_renderer;

  public:
    BoxRenderer(IPlatformRenderer*);
    void draw_box(Pos pos, int w, int h, const std::string& title, Color title_color,
                  Color box_color);
    void draw_rect(Pos pos, int w, int h, Color color);
    void draw_line(Pos pos, int w, int h, Color color);
};
#endif // !__DISPLAY_RENDERER_HPP__
