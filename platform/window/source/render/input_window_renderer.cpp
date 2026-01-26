#include "render/input_window_renderer.hpp"

using namespace std;

InputWindowRenderer::InputWindowRenderer(ConsoleRenderer a1, BoxRenderer a2)
    : console_renderer(a1), box_renderer(a2) {};

void InputWindowRenderer::render_input_window(Pos pos, string message)
{
    int w = 40;
    int h = 5;
    Color foreground = Color::CYAN;
    Color background = Color::COMMENT;
    Color font = Color::BACKGROUND;
    box_renderer.draw_rect(pos, w, h, background);
    box_renderer.draw_rect(pos, w, 1, foreground);

    console_renderer.set_cursor(pos.x + 1, pos.y);
    console_renderer.print_s(message.c_str(), font, foreground);

    console_renderer.set_cursor(pos.x + 2, pos.y + 2);
    console_renderer.print_s(">> ", font, background);

    console_renderer.set_color(font, background);
    console_renderer.show_cursor();
}
