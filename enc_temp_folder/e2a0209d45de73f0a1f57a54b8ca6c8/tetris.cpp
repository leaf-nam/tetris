#include "engine/engine.hpp"
#include "engine/multi_engine.hpp"
#include "engine/solo_engine.hpp"
#include "input/window_input.hpp"
#include "network/window_network.hpp"
#include "render/window_renderer.hpp"

#include <Windows.h>
#include <conio.h>
int main()
{
    // 윈도우 UTF8 인코딩 설정
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    IRenderer* renderer = new WindowRenderer();
    WindowRenderer* menu_renderer =
        new WindowRenderer(); // render_menu() 함수를 사용하기위한 렌더러 추가
    IInputHandler* input = new WindowInput();
    INetwork* network = new WindowNetwork();
    Engine* engine;
    bool is_solo = false;
    bool finished = false;
    bool flag = true;
    int menu_num = 0;
    char c = 0;

    while (flag) {
        menu_renderer->render_menu(menu_num);
        // 메뉴 선택
        while (true) {
            c = _getch();
            if (c == '\r') { // c 가 엔터일때
                break;
            }
            else if (c == '\t' || c == 80)
                menu_num++; // c 가 tab 또는 down arrow (80)일때
            menu_renderer->render_menu(menu_num);
        }

        switch (menu_num % 4) {
        case 0:
            // 싱글 플레이
            menu_renderer->render_clear();
            engine = new SoloEngine(input, renderer);
            engine->run();
            engine->finish();
            renderer = new WindowRenderer();
            input = new WindowInput();
            menu_renderer->render_game_over();
            (void) _getch();
            menu_renderer->render_clear();
            menu_num = 0;
            break;
        case 1:
            // 멀티 플레이
            menu_renderer->render_clear();

            engine = new MultiEngine(input, renderer, network);
            engine->run();
            engine->finish();
            renderer = new WindowRenderer();
            input = new WindowInput();
            network = new WindowNetwork();
            menu_renderer->render_game_over();
            (void) _getch();
            menu_renderer->render_clear();
            menu_num = 0;

            break;
        case 2:
            menu_renderer->render_settings();
            break;

        default:
            return 0;
        }
    }
}