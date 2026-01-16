#include "engine/engine.hpp"
#include "engine/multi_engine.hpp"
#include "engine/solo_engine.hpp"
#include "input/window_input.hpp"
#include "network/window_network.hpp"
#include "render/window_renderer.hpp"

#include <Windows.h>
int main()
{
    // 윈도우 UTF8 인코딩 설정
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    IRenderer* renderer = new WindowRenderer();
    IInputHandler* input = new WindowInput();
    INetwork* network = new WindowNetwork();
    Engine* engine;
    bool is_solo = false;
    bool finished = false;
    if (!is_solo)
        engine = new MultiEngine(input, renderer, network);
    else
        engine = new SoloEngine(input, renderer);

    engine->run();
    engine->stop();
    engine->finish();

    return 0;
}