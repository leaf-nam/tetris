#include "board/board.hpp"
#include "input/action.hpp"
#include "input/console_input.hpp"
#include "game_rule/game_rule.hpp"
#include "tetromino/tetromino_queue.hpp"
#include "render/terminal_renderer.hpp"
#include <thread>
#include <chrono>
#include <algorithm>
#include <random>


using namespace std;

int main(void)
{
   Board board;
    Input input;
    RuleEngine rule;
    TetrominoQueue& tetromino_queue = TetrominoQueue::get_instance();
    TRenderer renderer;
    
    renderer.clear();

    renderer.display();
    while (1)
    {
        renderer.draw(&board, tetromino_queue);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
}
