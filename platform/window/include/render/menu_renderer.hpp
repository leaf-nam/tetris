#ifndef __MENU_RENDERER_HPP__
#define __MENU_RENDERER_HPP__

#include "block_renderer.hpp"
#include "console_renderer.hpp"
#include "input_window_renderer.hpp"
#include "menu.hpp"
#include "text_renderer.hpp"

#include <util/setting.hpp>

class MenuRenderer
{
  private:
    Setting* setting;
    ConsoleRenderer console_renderer;
    TextRenderer text_renderer;
    BlockRenderer block_renderer;

  public:
    MenuRenderer(Setting*, ConsoleRenderer, TextRenderer, BlockRenderer);
    void render_menu_frame();
    void render_menu(Menu);
    void render_settings_frame();
    void render_settings(SettingMenu);
    void render_side();
    void render_shadow();
};

#endif // !__MENU_RENDERER_HPP__
