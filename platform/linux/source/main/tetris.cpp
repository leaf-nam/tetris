#include "app_state.hpp"
#include "engine/engine.hpp"
#include "engine/multi_engine.hpp"
#include "engine/solo_engine.hpp"
#include "input/linux_input.hpp"
#include "menu.hpp"
#include "lobby.hpp"
#include "lobby_input_handler/linux_lobby_input_handler.hpp"
#include "lobby_network/linux_lobby_network.hpp"
#include "lobby_renderer/linux_lobby_renderer.hpp"
#include "network/linux_network.hpp"
#include "render/color.hpp"
#include "render/linux_menu_renderer.hpp"
#include "render/linux_multi_renderer.hpp"
#include "render/linux_render_factory.hpp"
#include "render/linux_renderer.hpp"
#include "setting_manager.hpp"
#include "setting_storage.hpp"

#include <termios.h>
#include <chrono>
#include <thread>

static IInputHandler* input;
static IRenderer* renderer;
static INetwork* network;
static Engine* engine;
static Setting* setting;
static Lobby* lobby;
static bool finished = false;

using namespace std;

AppState run_menu();
AppState run_single_game();
AppState run_multi_game();
AppState run_settings();
void wait_key();
void restore_terminal_setting();

int main()
{
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

    restore_terminal_setting();

    return 0;
}

void wait_key()
{
    struct termios oldt, newt;
    char ch;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    read(STDIN_FILENO, &ch, 1);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

void restore_terminal_setting()
{
    struct termios oldt, newt;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag |= ICANON | ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    printf("\033[?25h");
    fflush(stdin);
}

// -> menu service : menu_renderer, input_handler interface 받아서 메뉴 생성
AppState run_menu()
{
    RenderFactory& render_factory = RenderFactory::getInstance();
    MenuRenderer menu_renderer = render_factory.create_menu_renderer();

    Menu menu(&menu_renderer, new LinuxInput());
    menu.reload();

    AppState app_state = AppState::MENU;
    while (app_state == AppState::MENU) {
        app_state = menu.update();
    }

    return app_state;
}

// -> setting service : setting_renderer, input_handler interface 받아서 세팅 초기화
AppState run_settings()
{
    RenderFactory render_factory = RenderFactory::getInstance();
    SettingStorage& setting_storage = SettingStorage::getInstance();
    SettingRenderer setting_renderer = render_factory.create_setting_renderer();

    SettingManager setting_manager(setting, &setting_renderer, new LinuxInput(), &setting_storage);
    setting_manager.reload();

    AppState app_state = AppState::SETTINGS;
    while (app_state == AppState::SETTINGS) {
        app_state = setting_manager.update();
    }

    return app_state;
}

AppState run_single_game()
{
    RenderFactory& render_factory = RenderFactory::getInstance();

    LinuxRenderer linux_renderer = render_factory.create_linux_renderer();
    InputWindowRenderer input_window_renderer = render_factory.create_input_window_renderer();
    TextRenderer text_renderer = render_factory.create_text_renderer();
    BoxRenderer box_renderer = render_factory.create_box_renderer();

    renderer = &linux_renderer;
    input = new LinuxInput();
    engine = new SoloEngine(setting, input, renderer);

    renderer->render_clear();
    renderer->render_background();

    for (int i = 3; i >= 1; --i) {
        text_renderer.draw_game_start_count({42, 16}, i);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    engine->run(false);
    engine->finish();

    box_renderer.draw_box({10, 12}, 54, 18, "", Color::GREEN, Color::BACKGROUND);
    text_renderer.draw_game_over({14, 14});
    input_window_renderer.render_input_window({16, 22}, "Press Any Button To Finish Game.");

    wait_key();

    delete input;
    delete engine;

    input = nullptr;
    renderer = nullptr;
    engine = nullptr;

    return AppState::MENU;
}

AppState run_multi_game()
{
    ILobbyInputHandler* linux_lobby_input_handler = new LinuxLobbyInputHandler();
    ILobbyNetwork* linux_lobby_network = new LinuxLobbyNetwork();
    ILobbyRenderer* linux_lobby_renderer = new LinuxLobbyRenderer();
    lobby = new Lobby(linux_lobby_network, linux_lobby_renderer, linux_lobby_input_handler);
    bool is_server = lobby->start();

    RenderFactory& render_factory = RenderFactory::getInstance();

    LinuxMultiRenderer linux_renderer = render_factory.create_linux_multi_renderer();
    renderer = &linux_renderer;
    input = new LinuxInput();
    network = new LinuxNetwork();
    engine = new MultiEngine(setting, input, renderer, network, lobby);

    renderer->render_clear();
    renderer->render_background();

    engine->run(is_server);
    engine->finish();

    wait_key();

    delete engine;
    delete network;
    delete input;
    delete lobby;
    delete linux_lobby_network;
    delete linux_lobby_renderer;
    delete linux_lobby_input_handler;

    input = nullptr;
    renderer = nullptr;
    network = nullptr;
    engine = nullptr;
    lobby = nullptr;

    return AppState::MENU;
}
