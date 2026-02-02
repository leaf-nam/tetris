#include "menu.hpp"

int menu_size = static_cast<int>(Menu::END) - static_cast<int>(Menu::BEGIN);
int stting_menu_size = static_cast<int>(SettingMenu::END) - static_cast<int>(SettingMenu::BEGIN);

const char* MENU[4] = {"SINGLE PLAY", "MULTI PLAY", "SETTINGS", "EXIT"};
const char* SETTING_MENU[4] = {"NICKNAME", "THEME", "SHADOW", "SAVE"};

Menu next_menu(Menu m) { return static_cast<Menu>((static_cast<uint8_t>(m) + 1) % menu_size); }

Menu prev_menu(Menu m)
{
    return static_cast<Menu>((static_cast<uint8_t>(m) + (menu_size - 1)) % menu_size);
}

SettingMenu next_menu(SettingMenu m)
{
    return static_cast<SettingMenu>((static_cast<uint8_t>(m) + 1) % stting_menu_size);
}

SettingMenu prev_menu(SettingMenu m)
{
    return static_cast<SettingMenu>((static_cast<uint8_t>(m) + (stting_menu_size - 1)) %
                                    stting_menu_size);
}
