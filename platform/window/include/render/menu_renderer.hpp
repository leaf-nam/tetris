#ifndef __MENU_RENDERER_HPP__
#define __MENU_RENDERER_HPP__

#include "console_renderer.hpp"
// #include "i_menu_renderer.hpp"
#include "menu.hpp"
#include "render/block_renderer.hpp"
#include "render/input_window_renderer.hpp"
#include "render/text_renderer.hpp"

#include <util/setting.hpp>

class MenuRenderer //: public IMenuRenderer
{
  private:
    Setting* setting;
    IPlatformRenderer* platform_renderer;
    TextRenderer text_renderer;
    BlockRenderer block_renderer;

  public:
    MenuRenderer(Setting*, IPlatformRenderer*, TextRenderer, BlockRenderer);
    void render_menu_frame();
    void render_menu(Menu);
    void render_settings_frame();
    void render_settings(SettingMenu);
    void render_side();
    void render_shadow();
};

#endif // !__MENU_RENDERER_HPP__
