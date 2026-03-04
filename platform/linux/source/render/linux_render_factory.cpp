#include "render/linux_render_factory.hpp"

void RenderFactory::initialize(Setting* setting)
{
    this->setting = setting;
    this->platform_renderer = new ConsoleRenderer();
    this->color_picker = ColorPicker();
    this->shadow_maker = ShadowMaker();
}

MenuRenderer RenderFactory::create_menu_renderer()
{
    return MenuRenderer(platform_renderer, create_text_renderer(), create_block_renderer());
}

SettingRenderer RenderFactory::create_setting_renderer()
{
    return SettingRenderer(setting, platform_renderer, create_text_renderer(),
                           create_block_renderer(), create_input_window_renderer());
}

LinuxRenderer RenderFactory::create_linux_renderer()
{
    return LinuxRenderer(setting, platform_renderer, color_picker, create_text_renderer(),
                          create_box_renderer(), create_block_renderer(), shadow_maker);
}

LinuxMultiRenderer RenderFactory::create_linux_multi_renderer()
{
    return LinuxMultiRenderer(setting, platform_renderer, color_picker, create_text_renderer(),
                               create_box_renderer(), create_block_renderer(), shadow_maker);
}

InputWindowRenderer RenderFactory::create_input_window_renderer()
{
    return InputWindowRenderer(platform_renderer, create_box_renderer());
}

TextRenderer RenderFactory::create_text_renderer()
{
    return TextRenderer(platform_renderer, color_picker);
}

BoxRenderer RenderFactory::create_box_renderer() { return BoxRenderer(platform_renderer); }

BlockRenderer RenderFactory::create_block_renderer()
{
    return BlockRenderer(platform_renderer, color_picker);
}
