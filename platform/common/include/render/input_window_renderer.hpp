#ifndef __INPUT_WINDOW_RENDERER_HPP__
#define __INPUT_WINDOW_RENDERER_HPP__

#include "box_renderer.hpp"
#include "i_platform_renderer.hpp"
#include "pos.hpp"

#include <string>

class InputWindowRenderer
{
  private:
    IPlatformRenderer* platform_renderer;
    BoxRenderer box_renderer;

  public:
    InputWindowRenderer(IPlatformRenderer*, BoxRenderer);
    void render_input_window(Pos, std::string);
};

#endif // !__INPUT_WINDOW_RENDERER_HPP__
