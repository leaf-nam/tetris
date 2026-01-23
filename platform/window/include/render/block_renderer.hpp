#ifndef __BLOCK_RENDERER_HPP__
#define __BLOCK_RENDERER_HPP__

#include "color_picker.hpp"
#include "console_renderer.hpp"

#include <tetromino/tetromino.hpp>

class BlockRenderer
{
  private:
    ConsoleRenderer console_renderer;
    ColorPicker color_picker;

  public:
    BlockRenderer(ConsoleRenderer, ColorPicker);
    void render_mino_pattern(Pos pos, const Tetromino& tetromino);
};
#endif // !__BLOCK_RENDERER_HPP__
