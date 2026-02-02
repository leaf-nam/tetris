#ifndef __RENDER_FACTORY_HPP__
#define __RENDER_FACTORY_HPP__

#include "i_platform_renderer.hpp"
#include "menu_renderer.hpp"
#include "window_multi_renderer.hpp"
#include "window_renderer.hpp"

#include <i_renderer.hpp>

class RenderFactory
{
  private:
    RenderFactory() = default;

    Setting* setting;
    IPlatformRenderer* platform_renderer;
    ColorPicker color_picker;
    ShadowMaker shadow_maker;

  public:
    static RenderFactory& getInstance()
    {
        static RenderFactory instance;
        return instance;
    }

    void initialize(Setting*);
    InputWindowRenderer create_input_window_renderer();
    MenuRenderer create_menu_renderer();
    WindowRenderer create_window_renderer();
    WindowMultiRenderer create_window_multi_renderer();
    TextRenderer create_text_renderer();
    BoxRenderer create_box_renderer();
    BlockRenderer create_block_renderer();
};

#endif // !__RENDER_FACTORY_HPP__
