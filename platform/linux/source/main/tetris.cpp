#include "engine/multi_engine.hpp"
#include "engine/solo_engine.hpp"
#include "input/linux_input.hpp"
#include "network/linux_network.hpp"
#include "render/linux_render.hpp"

int main()
{
    IInputHandler* input_handler = new LinuxInput();
    IRenderer* renderer = new LinuxRender;
    INetwork* network;
    Engine* engine;

    Setting* setting = new Setting();
    setting->nick_name = "Player";
    setting->color_theme = ColorTheme::DRACULA;
    setting->server_ip_address = "127.0.0.1";
    setting->server_port = "41234";

#if 1
    engine = new SoloEngine(setting, input_handler, renderer);
#else
    network = new LinuxNetwork;
    engine = new MultiEngine(input_handler, renderer, network);
#endif

    engine->run();

    engine->stop();

    engine->finish();

    return 0;
}
