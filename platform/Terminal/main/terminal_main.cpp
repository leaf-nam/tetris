#include "engine/Engine.hpp"
#include "render/terminal_renderer.hpp"
#include "input/terminal_input.hpp"

int main() {

    IRenderer* renderer = new TerminalRenderer();
    IInputHandler* input = new TerminalInput(); 

    Engine engine(input, renderer);

    engine.run();

    engine.finish(); 

    return 0;
}