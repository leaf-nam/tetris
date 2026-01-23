#ifndef __INPUT_WINDOW_RENDERER_HPP__

#define __INPUT_WINDOW_RENDERER_HPP__

#include "console_renderer.hpp"
#include "pos.hpp"

class InputWindowRenderer
{
  private:
    ConsoleRenderer console_renderer;

  public:
    InputWindowRenderer(ConsoleRenderer);
    void renderInputWindow(Pos pos);
};

#endif // !__INPUT_WINDOW_RENDERER_HPP__
