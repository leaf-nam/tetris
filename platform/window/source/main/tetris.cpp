#include "app_state.hpp"
#include "engine/engine.hpp"
#include "engine/multi_engine.hpp"
#include "engine/solo_engine.hpp"
#include "i_input_factory.hpp"
#include "input/window_input_factory.hpp"
#include "lobby.hpp"
#include "lobby_network/window_lobby_network.hpp"
#include "menu.hpp"
#include "network/window_network.hpp"
#include "render/color.hpp"
#include "render/window_lobby_renderer.hpp"
#include "render/window_menu_renderer.hpp"
#include "render/window_multi_renderer.hpp"
#include "render/window_render_factory.hpp"
#include "render/window_renderer.hpp"
#include "setting_manager.hpp"
#include "setting_storage.hpp"

#include <Windows.h>
#include <chrono>
#include <conio.h>
#include <thread>

static IRenderer* renderer;
static INetwork* network;
static Engine* engine;
static Setting* setting;
static Lobby* lobby;
static IInputFactory* input_factory;
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

    input_factory = new WindowInputFactory();

    char in = 0;

    while (state != AppState::EXIT) {
        switch (state) {
        case AppState::MENU:
            state = run_menu();
            break;

        case AppState::SINGLE_PLAY:
            state = run_single_game();
            break;

        case AppState::LOBBY:
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
    IMenuInputHandler* input_handler = input_factory->create_menu_input_handler();

    Menu menu(&menu_renderer, input_handler);
    menu.reload();

    AppState app_state = AppState::MENU;
    while (app_state == AppState::MENU) {
        app_state = menu.update();
    }

    delete input_handler;

    return app_state;
}

// -> setting service : setting_renderer, input_handler interface 받아서 세팅 초기화
AppState run_settings()
{
    RenderFactory render_factory = RenderFactory::getInstance();
    SettingStorage& setting_storage = SettingStorage::getInstance();
    SettingRenderer setting_renderer = render_factory.create_setting_renderer();
    ISettingInputHandler* input_handler = input_factory->create_setting_input_handler();

    SettingManager setting_manager(setting, &setting_renderer, input_handler, &setting_storage);
    setting_manager.reload();

    AppState app_state = AppState::SETTINGS;
    while (app_state == AppState::SETTINGS) {
        app_state = setting_manager.update();
    }

    delete input_handler;

    return app_state;
}

AppState run_single_game()
{
    bool is_run_continue = true;
    bool is_stop_continue = true;
    RenderFactory& render_factory = RenderFactory::getInstance();

    WindowRenderer window_renderer = render_factory.create_window_renderer();
    InputWindowRenderer input_window_renderer = render_factory.create_input_window_renderer();
    TextRenderer text_renderer = render_factory.create_text_renderer();
    BoxRenderer box_renderer = render_factory.create_box_renderer();

    renderer = &window_renderer;
    IInputHandler* input_handler = input_factory->create_input_handler();
    engine = new SoloEngine(setting, input_handler, renderer);

    renderer->render_clear();
    renderer->render_background();

    for (int i = 3; i >= 1; --i) {
        text_renderer.draw_game_start_count({20, 16}, i);
        Sleep(1000);
    }

    engine->init(false);
    while (is_run_continue)
        is_run_continue = engine->run(false);
    while (is_stop_continue)
        is_stop_continue = engine->stop(false);
    engine->finish();

    box_renderer.draw_box({10, 12}, 54, 18, "", Color::GREEN, Color::BACKGROUND);
    text_renderer.draw_game_over({14, 14});
    input_window_renderer.render_input_window({16, 22}, "Press Any Button To Finish Game.");

    (void) _getch();

    delete input_handler;
    delete engine;

    input_handler = nullptr;
    renderer = nullptr;
    engine = nullptr;

    return AppState::MENU;
}

AppState run_multi_game()
{
    bool is_server;
    bool is_run_continue = true;
    bool is_stop_continue = true;

    RenderFactory& render_factory = RenderFactory::getInstance();

    ILobbyNetwork* window_lobby_network = new WindowLobbyNetwork();
    WindowLobbyRenderer window_lobby_renderer_ = render_factory.create_lobby_renderer();
    ILobbyRenderer* window_lobby_renderer = &window_lobby_renderer_;
    ILobbyInputHandler* window_lobby_input_handler = input_factory->create_lobby_input_handler();
    Lobby* lobby =
        new Lobby(setting, window_lobby_network, window_lobby_renderer, window_lobby_input_handler);

    AppState lobby_state = lobby->start();

    switch (lobby_state) {
    case AppState::MULTI_SERVER:
        is_server = true;
        break;
    case AppState::MULTI_CLIENT:
        is_server = false;
        break;
    default: // default fallback : MENU
        delete lobby;
        delete window_lobby_network;
        delete window_lobby_input_handler;
        return AppState::MENU;
    }

    WindowMultiRenderer window_renderer = render_factory.create_window_multi_renderer();
    renderer = &window_renderer;
    IInputHandler* input_handler = input_factory->create_input_handler();
    network = new WindowNetwork();
    engine = new MultiEngine(setting, input_handler, renderer, network, lobby);

    renderer->render_clear();
    renderer->render_background();

    engine->init(is_server);
    while (is_run_continue)
        is_run_continue = engine->run(is_server);
    while (is_stop_continue)
        is_stop_continue = engine->stop(is_server);
    engine->finish();

    (void) _getch();

    delete engine;
    delete network;
    delete input_handler;
    delete lobby;
    delete window_lobby_network;
    delete window_lobby_input_handler;

    input_handler = nullptr;
    renderer = nullptr;
    network = nullptr;
    engine = nullptr;
    lobby = nullptr;

    return AppState::MENU;
}
