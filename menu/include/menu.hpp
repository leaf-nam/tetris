#ifndef __MENU_HPP__
#define __MENU_HPP__

#include "i_menu_input_handler.hpp"
#include "i_menu_renderer.hpp"
#include "menu_title.hpp"
#include "setting_title.hpp"

#include <cstdint>
#include <util/app_state.hpp>

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
    AppState update();
};

extern int menu_size;
extern const char* MENU_TITLE[4];
extern const char* SETTING_TITLE[4];

Menu next_menu(Menu);
Menu prev_menu(Menu);
SettingTitle next_menu(SettingTitle);
SettingTitle prev_menu(SettingTitle);

#endif
