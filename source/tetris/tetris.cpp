#include "engine/engine.hpp"
#if defined(__linux__)
    #include "render/linux_render.hpp"
    #include "input/linux_input.hpp"
#endif

int main()
{
    IInputHandler* input_handler = new LinuxInput();
    IRenderer* renderer = new LinuxRender;    

    Engine engine(input_handler, renderer);

    engine.run();

    engine.stop();

    engine.finish();

    return 0;
}
