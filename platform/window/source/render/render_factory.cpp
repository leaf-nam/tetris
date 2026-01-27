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
    return MenuRenderer(setting, console_renderer, create_text_renderer(), create_block_renderer());
}

WindowRenderer RenderFactory::create_window_renderer()
{
    return WindowRenderer(setting, console_renderer, color_picker, create_text_renderer(),
                          create_box_renderer(), create_block_renderer(), shadow_maker);
}

WindowMultiRenderer RenderFactory::create_window_multi_renderer()
{
    return WindowMultiRenderer(setting, console_renderer, color_picker, create_text_renderer(),
                               create_box_renderer(), create_block_renderer(), shadow_maker);
}

InputWindowRenderer RenderFactory::create_input_window_renderer()
{
    return InputWindowRenderer(console_renderer, create_box_renderer());
}

TextRenderer RenderFactory::create_text_renderer()
{
    return TextRenderer(console_renderer, color_picker);
}

BoxRenderer RenderFactory::create_box_renderer() { return BoxRenderer(console_renderer); }

BlockRenderer RenderFactory::create_block_renderer()
{
    return BlockRenderer(console_renderer, color_picker);
}
