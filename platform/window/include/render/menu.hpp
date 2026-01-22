#ifndef __MENU_HPP__
#define __MENU_HPP__

#include <cstdint>

enum class Menu
{
    BEGIN = 0,

    SINGLE_PLAY = BEGIN,
    MULTI_PLAY,
    SETTINGS,
    EXIT,

    END
};

enum class SettingMenu
{
    BEGIN = 0,

    NICKNAME = BEGIN,
    THEME,
    SHADOW,
    SAVE,

    END
};

extern int menu_size;

extern const char* MENU[4];
extern const char* SETTING_MENU[4];

Menu next_menu(Menu);
Menu prev_menu(Menu);
SettingMenu next_menu(SettingMenu);
SettingMenu prev_menu(SettingMenu);

#endif
