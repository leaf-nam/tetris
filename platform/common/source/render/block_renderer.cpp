#include "render/block_renderer.hpp"

#include "render/theme.hpp"

#include <string>

using namespace std;

BlockRenderer::BlockRenderer(IPlatformRenderer* a1, ColorPicker a2)
    : platform_renderer(a1), color_picker(a2)
{
}

void BlockRenderer::render_mino_pattern(Pos pos, const Tetromino& tetromino, bool transparent)
{
    if (tetromino.get_mino_type() < 0 || tetromino.get_mino_type() > 6) return;

    render_mino_pattern(pos, tetromino, color_picker.get_color_key(tetromino), Color::BACKGROUND,
                        transparent);
}

void BlockRenderer::render_mino_pattern(Pos pos, const Tetromino& tetromino, Color background_color,
                                        bool transparent)
{
    render_mino_pattern(pos, tetromino, color_picker.get_color_key(tetromino), background_color,
                        transparent);
}

void BlockRenderer::render_mino_pattern(Pos pos, const Tetromino& tetromino, Color mino_color,
                                        Color background_color, bool transparent)
{
    if (tetromino.get_mino_type() < 0 || tetromino.get_mino_type() > 6) return;

    for (int i = 0; i < 4; i++) {
        string line;
        for (int j = 0; j < 4; j++) {
            if (tetromino.get_shape()[i][j] != 0) {
                platform_renderer->set_cursor(pos.x + j * 2, pos.y + i);
                platform_renderer->print_s("██", mino_color);
            }
            else if (!transparent) {
                platform_renderer->set_cursor(pos.x + j * 2, pos.y + i);
                platform_renderer->print_s("██", background_color);
            }
        }
    }
}
