#include "engine/engine.hpp"
#include "engine/multi_engine.hpp"
#include "engine/solo_engine.hpp"
<<<<<<< HEAD:platform/Terminal/main/terminal-main.cpp
#include "input/terminal-input.hpp"
#include "network/terminal-network.hpp"
#include "render/terminal-renderer.hpp"

#include <Windows.h>
=======
#include "input/terminal_input.hpp"
#include "network/terminal_network.hpp"
#include "render/terminal_renderer.hpp"

// #include <Windows.h>
>>>>>>> c7e1a86a78dd0967e2bebb2f8d87d13b5ead2365:platform/Terminal/main/terminal_main.cpp
int main()
{
    // 윈도우 UTF8 인코딩 설정
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    IRenderer* renderer = new TerminalRenderer();
    IInputHandler* input = new TerminalInput();
    INetwork* network = new TerminalNetwork();
    Engine* engine;
    bool is_solo = false;
    if (!is_solo)
        engine = new MultiEngine(input, renderer, network);
    else
        engine = new SoloEngine(input, renderer);

    engine->run();
    engine->stop();
    engine->finish();

    return 0;
}