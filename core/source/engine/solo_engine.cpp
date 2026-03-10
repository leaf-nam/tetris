#include "engine/solo_engine.hpp"

#include <chrono>
#include <random>
#include <thread>

using namespace std;

SoloEngine::SoloEngine(Setting* setting, IInputHandler* input_handler, IRenderer* renderer)
    : Engine(setting, input_handler, renderer, nullptr, nullptr)
{
}

void SoloEngine::init(bool is_server)
{
    rule = create_rule("ZEN", board);

    curr_mino = 0;
    score = 0;
    is_level_up = false;
    index = 0;

    renderer->render_background();
    renderer->render_board(board, board.get_active_mino());
    renderer->render_hold(board.get_saved_mino());
    renderer->render_score(score);
    renderer->render_level(rule->get_level());
    renderer->render_timer(timer.get_seconds());
}

bool SoloEngine::run(bool is_server)
{
    if (!board.has_active_mino()) {
        if (!board.spawn_mino(TetrominoQueue::get_instance().get_new_tetromino())) goto out;
        renderer->render_next_block(TetrominoQueue::get_instance().get_tetrominos());
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
    action = key_mapper.map_key(key);

    if (action != -1) {
        rule->process(action);
        renderer->render_next_block(TetrominoQueue::get_instance().get_tetrominos());
        renderer->render_board(board, board.get_active_mino());
        renderer->render_hold(board.get_saved_mino());
    }

    new_score = rule->update_score();
    if (is_level_up)
        if (!board.insert_line(3)) {
            renderer->render_board(board, board.get_active_mino());
            renderer->render_hold(board.get_saved_mino());
            goto out;
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

    return true;
out:
    return false; 
}

bool SoloEngine::stop(bool is_server) { return false; }

int SoloEngine::finish() { return 0; }

SoloEngine::~SoloEngine() {}
