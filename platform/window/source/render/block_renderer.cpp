#include "render/block_renderer.hpp"

#include "render/theme.hpp"

#include <string>

using namespace std;

BlockRenderer::BlockRenderer(ConsoleRenderer a1, ColorPicker a2)
    : console_renderer(a1), color_picker(a2)
{
}

void BlockRenderer::render_mino_pattern(Pos pos, const Tetromino& tetromino, bool transparent)
{
    if (tetromino.get_mino_type() < 0 || tetromino.get_mino_type() > 6) return;

    render_mino_pattern(pos, tetromino, color_picker.get_color_key(tetromino), transparent);
}

void BlockRenderer::render_mino_pattern(Pos pos, const Tetromino& tetromino, Color color,
                                        bool transparent)
{
    if (tetromino.get_mino_type() < 0 || tetromino.get_mino_type() > 6) return;

    string color_str = get_color(color);
    string bg = get_color(Color::BACKGROUND, true);

    for (int i = 0; i < 4; i++) {
        string line;
        for (int j = 0; j < 4; j++) {
            if (tetromino.get_shape()[i][j] != 0) {
                console_renderer.set_cursor(pos.x + j * 2, pos.y + i);
                console_renderer.print_s("██", color);
            }
            else if (!transparent) {
                console_renderer.set_cursor(pos.x + j * 2, pos.y + i);
                console_renderer.print_s("  ", Color::BACKGROUND);
            }
        }
    }
}
