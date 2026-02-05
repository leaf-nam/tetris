#include "engine/engine.hpp"
#include "engine/multi_engine.hpp"
#include "engine/solo_engine.hpp"
#include "input/window_input.hpp"
#include "menu.hpp"
#include "network/window_network.hpp"
#include "render/color.hpp"
#include "render/menu_renderer.hpp"
#include "render/render_factory.hpp"
#include "render/window_multi_renderer.hpp"
#include "render/window_renderer.hpp"
#include "util/app_state.hpp"
#include "util/setting_storage.hpp"

#include <Windows.h>
#include <chrono>
#include <conio.h>
#include <thread>

static IInputHandler* input;
static IRenderer* renderer;
static INetwork* network;
static Engine* engine;
static Setting* setting;
static bool finished = false;

using namespace std;

AppState run_menu();
AppState run_single_game();
AppState run_multi_game();
AppState run_settings();

int main()
{
    // 윈도우 UTF8 인코딩 설정
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    SettingStorage& setting_storage = SettingStorage::getInstance();

    setting_storage.initialize("settings.txt");

    setting = new Setting(Setting(setting_storage.load()));

    Theme& theme = Theme::getInstance();
    theme.apply(static_cast<ThemeKey>(setting->color_theme));

    RenderFactory& render_factory = RenderFactory::getInstance();
    render_factory.initialize(setting);

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

// -> menu service : menu_renderer, input_handler interface 받아서 메뉴 생성
AppState run_menu()
{
    RenderFactory& render_factory = RenderFactory::getInstance();
    MenuRenderer menu_renderer = render_factory.create_menu_renderer();

    menu_renderer.render_menu_frame();
    menu_renderer.render_menu(MenuTitle::SINGLE_PLAY);
    Menu menu(&menu_renderer, new WindowInput());

    AppState app_state = AppState::MENU;
    while (app_state == AppState::MENU) {
        app_state = menu.update();
    }
    return app_state;
}

// -> setting service : setting_renderer, input_handler interface 받아서 세팅 초기화
AppState run_settings()
{
    SettingTitle menu = SettingTitle::NICKNAME;

    RenderFactory render_factory = RenderFactory::getInstance();
    MenuRenderer menu_renderer = render_factory.create_menu_renderer();
    InputWindowRenderer input_window_renderer = render_factory.create_input_window_renderer();
    SettingStorage& setting_storage = SettingStorage::getInstance();

    menu_renderer.render_settings_frame();
    menu_renderer.render_settings(menu);

    bool finish = false;
    while (!finish) {
        char in = _getch();

        // 엔터 처리
        if (in == '\r' || in == ' ') {
            switch (menu) {
            // 닉네임 변경
            case SettingTitle::NICKNAME: {
                string nickname;
                input_window_renderer.render_input_window({27, 20},
                                                          "Type your nickname.[length : 1 ~ 8]");
                getline(std::cin, nickname);

                if (!nickname.empty() && nickname.size() <= 8) {
                    setting->nick_name = nickname;
                }

                menu_renderer.render_settings_frame();
                menu_renderer.render_settings(menu);
                break;
            }

            // 테마 변경
            case SettingTitle::THEME: {
                setting->color_theme = (setting->color_theme + 1) % 4;
                Theme::getInstance().apply(static_cast<ThemeKey>(setting->color_theme));
                menu_renderer.render_settings_frame();
                break;
            }

            // 그림자 토글
            case SettingTitle::SHADOW: {
                setting->shadow_on = !setting->shadow_on;
                menu_renderer.render_settings_frame();
                break;
            }

            // 저장 및 종료
            case SettingTitle::SAVE: {
                setting_storage.save(*setting);
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
            case SettingTitle::THEME:
                setting->color_theme = (setting->color_theme + 3) % 4;
                Theme::getInstance().apply(static_cast<ThemeKey>(setting->color_theme));
                menu_renderer.render_settings_frame();
                break;
            case SettingTitle::SHADOW:
                setting->shadow_on = !setting->shadow_on;
                menu_renderer.render_settings_frame();
                break;
            default:
                break;
            }
        }

        // right arrow
        if (in == 77) {
            switch (menu) {
            case SettingTitle::THEME:
                setting->color_theme = (setting->color_theme + 1) % 4;
                Theme::getInstance().apply(static_cast<ThemeKey>(setting->color_theme));
                menu_renderer.render_settings_frame();
                break;
            case SettingTitle::SHADOW:
                setting->shadow_on = !setting->shadow_on;
                menu_renderer.render_settings_frame();
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

        menu_renderer.render_settings(menu);
    }

    return AppState::MENU;
}

AppState run_single_game()
{
    RenderFactory& render_factory = RenderFactory::getInstance();

    WindowRenderer window_renderer = render_factory.create_window_renderer();
    InputWindowRenderer input_window_renderer = render_factory.create_input_window_renderer();
    TextRenderer text_renderer = render_factory.create_text_renderer();
    BoxRenderer box_renderer = render_factory.create_box_renderer();

    renderer = &window_renderer;
    input = new WindowInput();
    engine = new SoloEngine(setting, input, renderer);

    renderer->render_clear();
    renderer->render_background();

    for (int i = 3; i >= 1; --i) {
        text_renderer.draw_game_start_count({42, 16}, i);
        Sleep(1000);
    }

    engine->run(false);
    engine->finish();

    box_renderer.draw_box({10, 12}, 54, 18, "", Color::GREEN, Color::BACKGROUND);
    text_renderer.draw_game_over({14, 14});
    input_window_renderer.render_input_window({16, 22}, "Press Any Button To Finish Game.");

    (void) _getch();

    delete input;
    delete engine;

    input = nullptr;
    renderer = nullptr;
    engine = nullptr;

    return AppState::MENU;
}

AppState run_multi_game()
{
    RenderFactory& render_factory = RenderFactory::getInstance();

    WindowMultiRenderer window_renderer = render_factory.create_window_multi_renderer();
    renderer = &window_renderer;
    input = new WindowInput();
    network = new WindowNetwork();
    engine = new SoloEngine(setting, input, renderer);

    renderer->render_clear();
    renderer->render_background();

    engine->run(true);
    engine->finish();

    (void) _getch();

    delete engine;

    input = nullptr;
    renderer = nullptr;
    network = nullptr;
    engine = nullptr;

    return AppState::MENU;
}
