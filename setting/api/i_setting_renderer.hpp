#ifndef __I_SETTING_RENDERER_HPP__
#define __I_SETTING_RENDERER_HPP__

#include "render/pos.hpp"
#include "setting.hpp"
#include "setting_title.hpp"

class ISettingRenderer
{
  public:
    virtual void render_settings_frame() = 0;
    virtual void render_settings(SettingTitle) = 0;
    virtual void render_input_window(Pos, std::string&&) = 0;
    virtual void render_side() = 0;
    virtual void render_shadow() = 0;
};

#endif // !__I_SETTING_RENDERER_HPP__
