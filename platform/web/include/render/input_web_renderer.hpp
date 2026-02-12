#ifndef __INPUT_WEB_RENDERER_HPP__

#define __INPUT_WEB_RENDERER_HPP__

#include "box_renderer.hpp"
#include "console_renderer.hpp"
#include "pos.hpp"

#include <string>

class InputWebRenderer
{
  private:
    ConsoleRenderer console_renderer;
    BoxRenderer box_renderer;

  public:
    InputWebRenderer(ConsoleRenderer, BoxRenderer);
    void render_input_web(Pos, std::string);
};

#endif // !__INPUT_WEB_RENDERER_HPP__
