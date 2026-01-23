#include "render/render_factory.hpp"

void RenderFactory::initialize(Setting* setting)
{
    this->setting = setting;
    console_renderer = ConsoleRenderer();
    color_picker = ColorPicker();
    shadow_maker = ShadowMaker();
}

MenuRenderer RenderFactory::create_menu_renderer()
{
    return MenuRenderer(setting, console_renderer, TextRenderer(console_renderer, color_picker),
                        BlockRenderer(console_renderer, color_picker));
}

WindowRenderer RenderFactory::create_window_renderer()
{
    return WindowRenderer(setting, console_renderer, color_picker,
                          TextRenderer(console_renderer, color_picker), BoxRenderer(console_renderer),
                          BlockRenderer(console_renderer, color_picker), 
                          shadow_maker);
}

InputWindowRenderer RenderFactory::create_input_window_renderer()
{
    BoxRenderer box_renderer(console_renderer);
    return InputWindowRenderer(console_renderer, box_renderer);
}
