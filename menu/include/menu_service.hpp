#include "i_menu_input_handler.hpp"
#include "i_menu_renderer.hpp"
#include "menu.hpp"

class MenuService
{
  private:
    IMenuRenderer* menu_renderer;
    IMenuInputHandler* menu_input_handler;
    Menu menu;

  public:
    MenuService(IMenuRenderer* a1) : menu_renderer(a1) {};
    Menu update();
};
