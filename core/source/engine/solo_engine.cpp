#include "engine/solo_engine.hpp"

using namespace std;

SoloEngine::SoloEngine(Setting* setting, IInputHandler* input_handler, IRenderer* renderer)
    : Engine(setting, input_handler, renderer, nullptr), timer(Timer::get_instance()), tetromino_queue(TetrominoQueue::get_instance())
{
    init();
}

void SoloEngine::init()
{
    rule = create_rule("ZEN", board);
    tetromino_queue.init();
    board.init();
    timer.init();
    score = 0;

    renderer->render_clear();
    renderer->render_background();
    renderer->render_board(board, board.get_active_mino());
    renderer->render_hold(board.get_saved_mino());
    renderer->render_score(score);
    renderer->render_level(rule->get_level());
    renderer->render_timer(timer.get_seconds());
}

int SoloEngine::step()
{
    int action;
    int new_score;
    bool is_level_up = false;
    int key;

    if (!board.has_active_mino()) {
        if (!board.spawn_mino(tetromino_queue.get_new_tetromino())) return EngineState::GAME_OVER;
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
    action = key_mapper.map_key(key);

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
            return EngineState::GAME_OVER;
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

    return EngineState::RUNNING;
}

int SoloEngine::finish()
{
    delete renderer;
    delete input_handler;
    return 0;
}

SoloEngine::~SoloEngine() {}
