#ifndef __MENU_HPP__
#define __MENU_HPP__

#include "app_state.hpp"
#include "i_menu_input_handler.hpp"
#include "i_menu_renderer.hpp"
#include "menu_title.hpp"
#include "setting_title.hpp"

#include <cstdint>

class Menu
{
  private:
    IMenuRenderer* renderer;
    IMenuInputHandler* input;
    MenuTitle menu;
    MenuTitle next_menu(MenuTitle);
    MenuTitle prev_menu(MenuTitle);

  public:
    Menu(IMenuRenderer*, IMenuInputHandler*);
    void reload();
    AppState update();
};

extern int menu_size;
extern const char* MENU_TITLE[4];

#endif
