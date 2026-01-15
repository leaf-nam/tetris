#include "engine/solo_engine.hpp"

#include "board/board.hpp"
#include "game_rule/rule_factory.hpp"
#include "tetromino/tetromino_queue.hpp"
#include "util/action.hpp"
#include "util/timer.hpp"

#include <algorithm>
#include <chrono>
#include <random>
#include <thread>

using namespace std;

SoloEngine::SoloEngine(IInputHandler* input_handler, IRenderer* renderer)
    : Engine(input_handler, renderer, nullptr)
{
}

void SoloEngine::run()
{
    Board board;
    unique_ptr<GameRule> rule = create_rule("ZEN", board);
    TetrominoQueue& tetromino_queue = TetrominoQueue::get_instance();
    Timer& timer = Timer::get_instance();

    int curr_mino = 0;
    int action;
    int score = 0, new_score;
    bool is_level_up = false;
    int key;
    int index = 0;
    char another_user_ip[1024];
    char c;

    renderer->render_background();
    renderer->render_board(board, board.get_active_mino());
    renderer->render_hold(board.get_saved_mino());
    renderer->render_score(score);
    renderer->render_level(rule->get_level());
    renderer->render_timer(timer.get_seconds());
    while (true) {
        if (!board.has_active_mino()) {
            if (!board.spawn_mino(tetromino_queue.get_new_tetromino())) break;
            renderer->render_next_block(tetromino_queue.get_tetrominos());
        }

        timer.set_curr_time();
        if (timer.check_500ms_time()) {
            rule->process(Action::DROP);
            renderer->render_board(board, board.get_active_mino());
            renderer->render_hold(board.get_saved_mino());
            renderer->render_score(score);
            renderer->render_level(rule->get_level());
            renderer->render_timer(timer.get_seconds());
            is_level_up = rule->time_and_level_update();
        }

        key = input_handler->scan();
        if (key != 0) {
            switch (key) {
            case 'a':
                action = Action::LEFT;
                break;
            case 's':
                action = Action::DROP;
                break;
            case 'd':
                action = Action::RIGHT;
                break;
            case 'q':
                action = Action::ROTATE_CCW;
                break;
            case 'e':
                action = Action::ROTATE_CW;
                break;
            case 'f':
                action = Action::HARD_DROP;
                break;
            case 'w':
                action = Action::SWAP;
                break;
            default:
                action = -1;
                break;
            }
        }
        else
            action = -1;

        if (action != -1) {
            rule->process(action);
            renderer->render_next_block(tetromino_queue.get_tetrominos());
            renderer->render_board(board, board.get_active_mino());
            renderer->render_hold(board.get_saved_mino());
        }

        new_score = rule->update_score();
        if (is_level_up)
            if (!board.insert_line(3)) {
                renderer->render_board(board, board.get_active_mino());
                renderer->render_hold(board.get_saved_mino());
                break;
            }
        if (new_score != 0 || is_level_up) {
            score += new_score;
            renderer->render_board(board, board.get_active_mino());
            renderer->render_hold(board.get_saved_mino());
            renderer->render_score(score);
            renderer->render_level(rule->get_level());
            renderer->render_timer(timer.get_seconds());
            is_level_up = false;
        }
    }
}

void SoloEngine::stop() {}

int SoloEngine::finish()
{
    delete renderer;
    delete input_handler;
    return 0;
}

SoloEngine::~SoloEngine() {}
