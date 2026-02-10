#include "render/window_menu_renderer.hpp"

#include "render/theme.hpp"

#include <conio.h>
#include <string>
#include <wtypes.h>

using namespace std;

MenuRenderer::MenuRenderer(IPlatformRenderer* a1, TextRenderer a2, BlockRenderer a3)
    : platform_renderer(a1), text_renderer(a2), block_renderer(a3)
{
}

void MenuRenderer::render_menu_frame()
{
    platform_renderer->clear();
    platform_renderer->hide_cursor();
    text_renderer.draw_logo({20, 7});
}

void MenuRenderer::render_menu(MenuTitle menu)
{
    for (int i = 0; i < 4; i++) {
        platform_renderer->set_cursor(25, 18 + i * 2);

        int menu_num = static_cast<int>(menu);
        Color color = (menu_num - 1 == i) ? Color::YELLOW : Color::COMMENT;
        string str = (menu_num - 1 == i) ? " > " : "   ";
        str += MENU_TITLE[i];
        platform_renderer->print_s(str, color);
    }
}
