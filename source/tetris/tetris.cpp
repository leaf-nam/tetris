#include "board/board.hpp"
#include "input/action.hpp"
#include "input/console_input.hpp"
#include "game_rule/game_rule.hpp"

#include <thread>
#include <chrono>
#include <algorithm>
#include <random>

using namespace std;

int main(void) {
    Board board;
    Input input;
    RuleEngine rule;

    vector<int> mino_set = {0, 1, 2, 3, 4, 5, 6};
    int curr_mino = 0;
    int action;
    int score = 0, new_score;
    auto base_time = chrono::steady_clock::now();
    auto curr_time = chrono::steady_clock::now();
    auto diff = curr_time - base_time;

    board.render();
    cout << "SCORE: " << score << "\r";

    while (1)
    {
        if(!board.has_active_mino()) 
        {
            if (curr_mino >= 7)
            {
                std::shuffle(mino_set.begin(), mino_set.end(), std::mt19937(std::random_device{}()));
                curr_mino = 0;
            }
            if (!board.spawn_mino(mino_set[curr_mino++])) break;
        }

        curr_time = chrono::steady_clock::now();
        diff = curr_time - base_time;
        if (diff >= chrono::milliseconds(500))
        {
            base_time = chrono::steady_clock::now();
            board.move_mino(Action::DROP);
            board.render();
        }
        
        action = input.console_input();

        if (action != -1) 
        {
            if (action == Action::HARD_DROP) while (board.has_active_mino()) board.move_mino(Action::DROP);
            else board.move_mino(action);
            board.render();
        }
        
        new_score = rule.update_score(board);
        if (new_score) 
        {
            score += new_score;
            board.render();
            cout << "SCORE: " << score << "\r";
        }
    }

    return 0;
}