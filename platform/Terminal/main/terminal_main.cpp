#include "platform/Terminal/terminal_intput.hpp"
#include "engine/Engine.hpp"
#include "render/terminal_renderer.hpp"

int main() {

    TRenderer* renderer = new TRenderer();
    ConsoleInput* input = new ConsoleInput(); 

    Engine engine(input, renderer);

    engine.run();

    engine.finish(); 

    return 0;
}