#include "engine/solo_engine.hpp"
#include "engine/multi_engine.hpp"
#if defined(__linux__)
    #include "render/linux_render.hpp"
    #include "input/linux_input.hpp"
    #include "network/linux_network.hpp"
#endif

int main()
{
    IInputHandler* input_handler = new LinuxInput();
    IRenderer* renderer = new LinuxRender;
    INetwork* network;
    Engine* engine;

#if 1
    engine = new SoloEngine(input_handler, renderer);
#else
    network = new LinuxNetwork;
    engine = new MultiEngine(input_handler, renderer, network);
#endif

    engine->run();

    engine->stop();

    engine->finish();

    return 0;
}
