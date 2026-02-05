#ifndef __SETTING_RENDERER_HPP__
#define __SETTING_RENDERER_HPP__

#include "i_setting_renderer.hpp"
#include "menu.hpp"
#include "render/block_renderer.hpp"
#include "render/input_window_renderer.hpp"
#include "render/text_renderer.hpp"
#include "window_console_renderer.hpp"

class SettingRenderer : public ISettingRenderer
{
  private:
    Setting* setting;
    IPlatformRenderer* platform_renderer;
    TextRenderer text_renderer;
    BlockRenderer block_renderer;
    InputWindowRenderer input_window_renderer;

  public:
    SettingRenderer(Setting*, IPlatformRenderer*, TextRenderer, BlockRenderer, InputWindowRenderer);
    void render_settings_frame() override;
    void render_settings(SettingTitle) override;
    void render_input_window(Pos, std::string&&) override;
    void render_side() override;
    void render_shadow() override;
};

#endif // !__SETTING_RENDERER_HPP__
