#ifndef __MENU_RENDERER_HPP__
#define __MENU_RENDERER_HPP__

#include "i_menu_renderer.hpp"
#include "menu.hpp"
#include "render/block_renderer.hpp"
#include "render/input_window_renderer.hpp"
#include "render/text_renderer.hpp"
#include "linux_console_renderer.hpp"

class MenuRenderer : public IMenuRenderer
{
  private:
    IPlatformRenderer* platform_renderer;
    TextRenderer text_renderer;
    BlockRenderer block_renderer;

  public:
    MenuRenderer(IPlatformRenderer*, TextRenderer, BlockRenderer);
    void render_menu_frame() override;
    void render_menu(MenuTitle) override;
};

#endif // !__MENU_RENDERER_HPP__
