#include "render/box_renderer.hpp"

using namespace std;

BoxRenderer::BoxRenderer(IPlatformRenderer* a1) : platform_renderer(a1) {}

void BoxRenderer::draw_box(Pos pos, int w, int h, const std::string& title, Color title_color,
                           Color box_color)
{
    draw_rect(pos, w, h, box_color);

    if (!title.empty()) {
        platform_renderer->set_cursor(pos.x, pos.y);
        platform_renderer->print_s(title, title_color, box_color);
    }
}

void BoxRenderer::draw_rect(Pos pos, int w, int h, Color color)
{
    for (int i = pos.y; i < pos.y + h; ++i) {
        for (int j = pos.x; j < pos.x + w; ++j) {
            platform_renderer->set_cursor(j, i);
            platform_renderer->print_s("██", color);
        }
    }
}

void BoxRenderer::draw_line(Pos pos, int w, int h, Color color)
{
    for (int x = 0; x < w - 1; ++x) {
        platform_renderer->set_cursor(pos.x + x - 1, pos.y);
        platform_renderer->print_s("██", color);

        platform_renderer->set_cursor(pos.x + x - 1, pos.y + h);
        platform_renderer->print_s("██", color);
    }

    for (int y = 0; y < h; ++y) {
        platform_renderer->set_cursor(pos.x - 1, pos.y + y);
        platform_renderer->print_s("█", color);

        platform_renderer->set_cursor(pos.x + w - 2, pos.y + y);
        platform_renderer->print_s("█", color);
    }
}
