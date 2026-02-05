#ifndef __I_MENU_RENDERER_HPP__
#define __I_MENU_RENDERER_HPP__

#include "menu_title.hpp"
#include "render/block_renderer.hpp"
#include "render/input_window_renderer.hpp"
#include "render/text_renderer.hpp"
#include "setting_title.hpp"

#include <util/setting.hpp>

class IMenuRenderer
{
  public:
    virtual void render_menu_frame() = 0;
    virtual void render_menu(MenuTitle) = 0;
    virtual void render_settings_frame() = 0;
    virtual void render_settings(SettingTitle) = 0;
    virtual void render_side() = 0;
    virtual void render_shadow() = 0;
};

#endif // !__I_MENU_RENDERER_HPP__
