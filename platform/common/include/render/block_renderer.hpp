#ifndef __BLOCK_RENDERER_HPP__
#define __BLOCK_RENDERER_HPP__

#include "color_picker.hpp"
#include "i_platform_renderer.hpp"
#include "pos.hpp"

#include <tetromino/tetromino.hpp>

class BlockRenderer
{
  private:
    IPlatformRenderer* platform_renderer;
    ColorPicker color_picker;

  public:
    BlockRenderer(IPlatformRenderer*, ColorPicker);
    void render_mino_pattern(Pos, const Tetromino&, bool transparent = false);
    void render_mino_pattern(Pos, const Tetromino&, Color, bool transparent = false);
    void render_mino_pattern(Pos pos, const Tetromino& tetromino, Color mino_color,
                             Color background_color, bool transparent = false);
};
#endif // !__BLOCK_RENDERER_HPP__
