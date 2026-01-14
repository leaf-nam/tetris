#include "engine/engine.hpp"
#include "render/terminal_renderer.hpp"
#include "input/terminal_input.hpp"
#include "network/terminal_network.hpp"
#include <Windows.h>
#include "engine/solo_engine.hpp"
#include "engine/multi_engine.hpp"
int main() {

    // 윈도우 UTF8 인코딩 설정
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    IRenderer* renderer = new TerminalRenderer();
    IInputHandler* input = new TerminalInput();
    INetwork* network = new TerminalNetwork();
    Engine *engine;
    bool isSolo = false;
    if(!isSolo)engine =new MultiEngine(input, renderer, network);
    else engine =new SoloEngine(input, renderer);

    engine->run();
    engine->stop();
    engine->finish();

    
    return 0;
}