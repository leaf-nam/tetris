#ifndef __INPUT_WINDOW_RENDERER_HPP__

#define __INPUT_WINDOW_RENDERER_HPP__

#include "box_renderer.hpp"
#include "console_renderer.hpp"
#include "pos.hpp"

#include <string>

class InputWindowRenderer
{
  private:
    ConsoleRenderer console_renderer;
    BoxRenderer box_renderer;

  public:
    InputWindowRenderer(ConsoleRenderer, BoxRenderer);
    void render_input_window(Pos, std::string);
};

#endif // !__INPUT_WINDOW_RENDERER_HPP__
