#include "engine/engine.hpp"
#include "engine/multi_engine.hpp"
#include "engine/solo_engine.hpp"
#include "input/window_input.hpp"
#include "network/window_network.hpp"
#include "render/color.hpp"
#include "render/menu.hpp"
#include "render/menu_renderer.hpp"
#include "render/window_renderer.hpp"

#include <Windows.h>
#include <conio.h>

static MenuRenderer* menu_renderer;
static IInputHandler* input;
static IRenderer* renderer;
static Engine* engine;
static Setting* setting;
static bool finished = false;

enum class AppState
{
    MENU,
    SINGLE_PLAY,
    MULTI_PLAY,
    SETTINGS,
    EXIT
};

AppState run_menu();
AppState run_single_game();
AppState run_multi_game();
AppState run_settings();

int main()
{
    // 윈도우 UTF8 인코딩 설정
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);

    setting = new Setting();
    menu_renderer = new MenuRenderer(*setting);

    setting->nick_name = "Player";
    setting->shadow_on = true;
    setting->color_theme = 0;
    setting->server_ip_address = "127.0.0.1";
    setting->server_port = "41234";

    Theme::getInstance();
    AppState state = AppState::MENU;

    char in = 0;

    while (state != AppState::EXIT) {
        switch (state) {
        case AppState::MENU:
            state = run_menu();
            break;

        case AppState::SINGLE_PLAY:
            state = run_single_game();
            break;

        case AppState::MULTI_PLAY:
            state = run_multi_game();
            break;

        case AppState::SETTINGS:
            state = run_settings();
            break;

        default:
            state = AppState::EXIT;
        }
    }
}

AppState run_menu()
{
    Menu menu = Menu::SINGLE_PLAY;

    menu_renderer->render_clear();
    menu_renderer->render_menu_frame();
    menu_renderer->render_menu(menu);

    while (true) {
        char in = _getch();

        if (in == '\r' || in == ' ') {
            switch (menu) {
            case Menu::SINGLE_PLAY:
                return AppState::SINGLE_PLAY;
            case Menu::MULTI_PLAY:
                return AppState::MULTI_PLAY;
            case Menu::SETTINGS:
                return AppState::SETTINGS;
            case Menu::EXIT:
                return AppState::EXIT;
            default:
                return AppState::EXIT;
            }
        }

        if (in == 80 || in == '\t') // down arrow
            menu = next_menu(menu);

        else if (in == 72) // up arrow
            menu = prev_menu(menu);

        else if (in == 27) // esc
            return AppState::EXIT;

        menu_renderer->render_menu(menu);
    }
}

AppState run_settings()
{
    SettingMenu menu = SettingMenu::NICKNAME;

    menu_renderer->render_clear();
    menu_renderer->render_settings_frame();
    menu_renderer->render_settings(menu, *setting);

    bool finish = false;
    while (!finish) {
        char in = _getch();

        // 엔터 처리
        if (in == '\r' || in == ' ') {
            switch (menu) {
            case SettingMenu::NICKNAME: {
                string nickname;
                getline(std::cin, nickname);
                if (!nickname.empty()) {
                    setting->nick_name = nickname;
                }
                break;
            }
            case SettingMenu::THEME: {
                setting->color_theme = (setting->color_theme + 1) % 4;
                Theme::getInstance().apply(static_cast<ThemeKey>(setting->color_theme));
                menu_renderer->render_settings_frame();
                break;
            }
            case SettingMenu::SHADOW: {
                setting->shadow_on = !setting->shadow_on;
                break;
            }
            case SettingMenu::SAVE: {
                finish = true;
                break;
            }
            default:
                break;
            }
        }

        // left arrow
        if (in == 75) {
            switch (menu) {
            case SettingMenu::THEME:
                setting->color_theme = (setting->color_theme + 3) % 4;
                Theme::getInstance().apply(static_cast<ThemeKey>(setting->color_theme));
                menu_renderer->render_settings_frame();
                break;
            case SettingMenu::SHADOW:
                setting->shadow_on = !setting->shadow_on;
                break;
            default:
                break;
            }
        }

        // right arrow
        if (in == 77) {
            switch (menu) {
            case SettingMenu::THEME:
                setting->color_theme = (setting->color_theme + 1) % 4;
                Theme::getInstance().apply(static_cast<ThemeKey>(setting->color_theme));
                menu_renderer->render_settings_frame();
                break;
            case SettingMenu::SHADOW:
                setting->shadow_on = !setting->shadow_on;
                break;
            default:
                break;
            }
        }

        // down arrow
        if (in == 80 || in == '\t') menu = next_menu(menu);

        // up arrow
        else if (in == 72)
            menu = prev_menu(menu);

        // esc
        else if (in == 27)
            return AppState::EXIT;

        menu_renderer->render_settings(menu, *setting);
    }

    return AppState::MENU;
}

AppState run_single_game()
{
    renderer = new WindowRenderer(*setting);
    input = new WindowInput();
    engine = new SoloEngine(setting, input, renderer);

    renderer->render_clear();
    engine->run();
    engine->finish();

    (void) _getch();

    return AppState::MENU;
}

AppState run_multi_game()
{
    menu_renderer->render_clear();

    renderer = new WindowRenderer(*setting);
    INetwork* network = new WindowNetwork();

    engine = new MultiEngine(setting, input, renderer, network);
    engine->run();
    engine->finish();
    menu_renderer->render_game_over();

    (void) _getch();

    return AppState::MENU;
}
