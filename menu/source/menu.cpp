#include "menu.hpp"

int menu_size = static_cast<int>(MenuTitle::END) - static_cast<int>(MenuTitle::BEGIN);

const char* MENU_TITLE[4] = {"SINGLE PLAY", "MULTI PLAY", "SETTINGS", "EXIT"};

Menu::Menu(IMenuRenderer* a1, IMenuInputHandler* a2) : renderer(a1), input(a2)
{
    menu = MenuTitle::SINGLE_PLAY;
}

void Menu::reload()
{
    renderer->render_menu_frame();
    renderer->render_menu(MenuTitle::SINGLE_PLAY);
}

AppState Menu::update()
{
    int in = input->scan();

    if (in == '\r' || in == ' ') {
        switch (menu) {
        case MenuTitle::SINGLE_PLAY:
            return AppState::SINGLE_PLAY;
        case MenuTitle::MULTI_PLAY:
            return AppState::MULTI_PLAY;
        case MenuTitle::SETTINGS:
            return AppState::SETTINGS;
        case MenuTitle::EXIT:
            return AppState::EXIT;
        default:
            return AppState::EXIT;
        }
    }

    // down arrow
    if (in == 1001 || in == '\t') {
        menu = next_menu(menu);
        renderer->render_menu(menu);
    }

    // up arrow
    if (in == 1000) {
        menu = prev_menu(menu);
        renderer->render_menu(menu);
    }

    if (in == 27) // esc
        return AppState::EXIT;

    return AppState::MENU;
}

MenuTitle Menu::next_menu(MenuTitle m)
{
    return static_cast<MenuTitle>((static_cast<uint8_t>(m) + 1) % menu_size);
}

MenuTitle Menu::prev_menu(MenuTitle m)
{
    return static_cast<MenuTitle>((static_cast<uint8_t>(m) + (menu_size - 1)) % menu_size);
}
