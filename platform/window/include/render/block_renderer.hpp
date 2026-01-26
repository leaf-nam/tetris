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
    void render_mino_pattern(Pos, const Tetromino&, bool transparent = false);
    void render_mino_pattern(Pos, const Tetromino&, Color, bool transparent = false);
};
#endif // !__BLOCK_RENDERER_HPP__
