#ifndef __RENDER_FACTORY_HPP__
#define __RENDER_FACTORY_HPP__

#include "menu_renderer.hpp"
#include "window_renderer.hpp"

#include <i_renderer.hpp>

class RenderFactory
{
  private:
    RenderFactory() = default;

    Setting* setting;
    ConsoleRenderer console_renderer;
    ColorPicker color_picker;
    ShadowMaker shadow_maker;

  public:
    static RenderFactory& getInstance()
    {
        static RenderFactory instance;
        return instance;
    }

    void initialize(Setting* setting);
    InputWindowRenderer create_input_window_renderer();
    MenuRenderer create_menu_renderer();
    WindowRenderer create_window_renderer();
    TextRenderer create_text_renderer();
    BoxRenderer create_box_renderer();
    BlockRenderer create_block_renderer();
};

#endif // !__RENDER_FACTORY_HPP__
