#include "board/board.hpp"
#include "input/action.hpp"
#include "input/console_input.hpp"
#include "game_rule/game_rule.hpp"
#include "tetromino/tetromino_queue.hpp"

#include <thread>
#include <chrono>
#include <algorithm>
#include <random>

using namespace std;

int main(void) {
    Board board;
    Input input;
    RuleEngine rule;
    TetrominoQueue& tetromino_queue = TetrominoQueue::get_instance();

    int curr_mino = 0;
    int action;
    int score = 0, new_score;
    auto base_time = chrono::steady_clock::now();
    auto curr_time = chrono::steady_clock::now();
    auto diff = curr_time - base_time;
    bool is_level_up = false;

    board.render();
    cout << "SCORE: " << score << ", LEVEL: " << rule.get_level() << "\r";

    while (1)
    {
        if(!board.has_active_mino())
        {
            if (!board.spawn_mino(tetromino_queue.get_new_tetromino())) break;
            tetromino_queue.draw_tetromino_queue();
        }

        curr_time = chrono::steady_clock::now();
        diff = curr_time - base_time;
        if (diff >= chrono::milliseconds(500))
        {
            base_time = chrono::steady_clock::now();
            board.move_mino(Action::DROP);
            board.render();
            is_level_up = rule.time_and_level_update();
        }
        
        action = input.console_input();

        if (action != -1) 
        {
            if (action == Action::HARD_DROP) while (board.has_active_mino()) board.move_mino(Action::DROP);
            else board.move_mino(action);
            tetromino_queue.draw_tetromino_queue();
            board.render();
        }
        
        new_score = rule.update_score(board);
        if(is_level_up)
            if(!board.insert_line(3))
            {
                board.render();
                break;
            }
        if (new_score || is_level_up) 
        {
            score += new_score;
            board.render();
            cout << "SCORE: " << score << ", LEVEL: " << rule.get_level() << "\r";
            is_level_up = false;
        }
    }

    return 0;
}
