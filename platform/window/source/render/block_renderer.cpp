#include "render/block_renderer.hpp"

#include "render/theme.hpp"

#include <string>

using namespace std;

BlockRenderer::BlockRenderer(ConsoleRenderer a1, ColorPicker a2)
    : console_renderer(a1), color_picker(a2)
{
}

void BlockRenderer::render_mino_pattern(Pos pos, const Tetromino& tetromino)
{
    if (tetromino.get_mino_type() < 0 || tetromino.get_mino_type() > 6) return;

    string color = color_picker.get_block_color(tetromino);
    string bg = get_color(Color::BACKGROUND, true);

    for (int i = 0; i < 4; i++) {
        string line;
        for (int j = 0; j < 4; j++) {
            if (tetromino.get_shape()[i][j] != 0) {
                line += color + bg + "██" + Theme::reset();
            }
            else {
                line += bg + "  " + Theme::reset();
            }
        }

        console_renderer.set_cursor(pos.x, pos.y + i);
        printf("%s", line.c_str());
    }
}
