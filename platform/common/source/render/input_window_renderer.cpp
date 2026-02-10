#include "render/input_window_renderer.hpp"

using namespace std;

InputWindowRenderer::InputWindowRenderer(IPlatformRenderer* a1, BoxRenderer a2)
    : platform_renderer(a1), box_renderer(a2) {};

void InputWindowRenderer::render_input_window(Pos pos, string message)
{
    int w = 40;
    int h = 5;
    Color foreground = Color::CYAN;
    Color background = Color::COMMENT;
    Color font = Color::BACKGROUND;
    box_renderer.draw_rect(pos, w, h, background);
    box_renderer.draw_rect(pos, w, 1, foreground);

    platform_renderer->set_cursor(pos.x + 1, pos.y);
    platform_renderer->print_s(message.c_str(), font, foreground);

    platform_renderer->set_cursor(pos.x + 2, pos.y + 2);
    platform_renderer->print_s(">> ", font, background);

    platform_renderer->set_color(font, background);
    platform_renderer->show_cursor();
}
