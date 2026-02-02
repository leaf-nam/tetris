#ifndef __I_MENU_RENDERER_HPP__
#define __I_MENU_RENDERER_HPP__

#include "menu.hpp"
#include "render/block_renderer.hpp"
#include "render/input_window_renderer.hpp"
#include "render/text_renderer.hpp"

#include <util/setting.hpp>

class IMenuRenderer
{
  public:
    virtual void render_menu_frame() = 0;
    virtual void render_menu(Menu) = 0;
    virtual void render_settings_frame() = 0;
    virtual void render_settings(SettingMenu) = 0;
    virtual void render_side() = 0;
    virtual void render_shadow() = 0;
};

#endif // !__I_MENU_RENDERER_HPP__
