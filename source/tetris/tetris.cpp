#include "engine/engine.hpp"
#if defined(__linux__)
    #include "render/linux_render.hpp"
    #include "input/linux_input.hpp"
    #include "network/linux_network.hpp"
#endif

int main()
{
    IInputHandler* input_handler = new LinuxInput();
    IRenderer* renderer = new LinuxRender;
    INetwork* network = new LinuxNetwork;   

    Engine engine(input_handler, renderer, network);

    engine.run();

    engine.stop();

    engine.finish();

    return 0;
}
