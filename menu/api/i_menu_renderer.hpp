#ifndef __I_MENU_RENDERER_HPP__
#define __I_MENU_RENDERER_HPP__

#include "menu_title.hpp"

class IMenuRenderer
{
  public:
    virtual void render_menu_frame() = 0;
    virtual void render_menu(MenuTitle) = 0;
};

#endif // !__I_MENU_RENDERER_HPP__
