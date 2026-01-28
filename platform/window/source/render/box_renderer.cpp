#include "render/box_renderer.hpp"

using namespace std;

BoxRenderer::BoxRenderer(ConsoleRenderer console_renderer) : console_renderer(console_renderer) {}

void BoxRenderer::draw_box(Pos pos, int w, int h, const std::string& title, Color title_color,
                           Color box_color)
{
    draw_rect(pos, w, h, box_color);

    if (!title.empty()) {
        console_renderer.set_cursor(pos.x, pos.y);
        console_renderer.print_s(title, title_color, box_color);
    }
}

void BoxRenderer::draw_rect(Pos pos, int w, int h, Color color)
{
    for (int i = pos.y; i < pos.y + h; ++i) {
        for (int j = pos.x; j < pos.x + w; ++j) {
            console_renderer.set_cursor(j, i);
            console_renderer.print_s("██", color);
        }
    }
}

void BoxRenderer::draw_line(Pos pos, int w, int h, Color color)
{
    for (int x = 0; x < w - 1; ++x) {
        console_renderer.set_cursor(pos.x + x - 1, pos.y);
        console_renderer.print_s("██", color);

        console_renderer.set_cursor(pos.x + x - 1, pos.y + h);
        console_renderer.print_s("██", color);
    }

    for (int y = 0; y < h; ++y) {
        console_renderer.set_cursor(pos.x - 1, pos.y + y);
        console_renderer.print_s("█", color);

        console_renderer.set_cursor(pos.x + w - 2, pos.y + y);
        console_renderer.print_s("█", color);
    }
}
