#include "menu_service.hpp"

Menu MenuService::update()
{
    // char in = menu_input_handler->scan();

    // if (in == '\r' || in == ' ') {
    //     switch (menu) {
    //     case Menu::SINGLE_PLAY:
    //         return AppState::SINGLE_PLAY;
    //     case Menu::MULTI_PLAY:
    //         return AppState::MULTI_PLAY;
    //     case Menu::SETTINGS:
    //         return AppState::SETTINGS;
    //     case Menu::EXIT:
    //         return AppState::EXIT;
    //     default:
    //         return AppState::EXIT;
    //     }
    // }

    // if (in == 80 || in == '\t') // down arrow
    //     return next_menu(menu);

    // else if (in == 72) // up arrow
    //     return prev_menu(menu);

    // menu_renderer->render_menu(menu);
    return Menu::SINGLE_PLAY;
}
