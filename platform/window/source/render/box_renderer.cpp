#include "render/box_renderer.hpp"

using namespace std;

BoxRenderer::BoxRenderer(ConsoleRenderer console_renderer) : console_renderer(console_renderer) {}

void BoxRenderer::draw_box(Pos pos, int w, int h, const std::string& title, Color color)
{
    console_renderer.set_cursor(pos.x, pos.y);
    console_renderer.print_s("¦£" + string(w * 2, ' ') + "¦¤", color);

    for (int i = 1; i <= h; ++i) {
        console_renderer.set_cursor(pos.x, pos.y + i);
        console_renderer.print_s("¦¢" + string(w * 2, ' ') + "¦¢", color);
    }
    console_renderer.set_cursor(pos.x, pos.y + h + 1);
    console_renderer.print_s("¦¦" + string(w * 2, ' ') + "¦¥", color);

    if (!title.empty()) {
        console_renderer.set_cursor(pos.x + (w * 2 - title.length()) / 2, pos.y);
        console_renderer.print_s("[" + title + "]", color);
    }
}
