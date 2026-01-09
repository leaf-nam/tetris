#include <Windows.h>

#include "engine/Engine.hpp"
#include "render/terminal_renderer.hpp"
#include "input/terminal_input.hpp"

int main() {

    // 윈도우 UTF8 인코딩 설정
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    IRenderer* renderer = new TerminalRenderer();
    IInputHandler* input = new TerminalInput(); 

    Engine engine(input, renderer);

    engine.run();

    engine.finish(); 

    return 0;
}