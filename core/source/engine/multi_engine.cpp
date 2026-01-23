#include "engine/multi_engine.hpp"

using namespace std;

MultiEngine::MultiEngine(Setting* setting, IInputHandler* input_handler, IRenderer* renderer,
                         INetwork* network)
    : Engine(setting, input_handler, renderer, network), timer(Timer::get_instance()), tetromino_queue(TetrominoQueue::get_instance())
{
    init();
}

void MultiEngine::init()
{
    int index = 0;

    rule = create_rule("ZEN", board);
    tetromino_queue.init();
    board.init();
    timer.init();
    attack = 0;

    renderer->render_ip_recv();
    while (true) {
        char c = input_handler->scan();
        if (c == '\n' || c == '\r') break;
        if (c != 0) {
            renderer->render_char(c);
            another_user_ip[index++] = c;
        }
    }
    another_user_ip[index] = '\0';

    renderer->render_clear();
    renderer->render_background();
    renderer->render_board(board, board.get_active_mino());
    renderer->render_hold(board.get_saved_mino());
    renderer->render_score(attack);
    renderer->render_level(rule->get_level());
    renderer->render_timer(timer.get_seconds());
}

int MultiEngine::step()
{
    PacketStruct recv_pkt;
    int action;
    bool is_line_fill_complete = false;
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
        renderer->render_level(rule->get_level());
        renderer->render_timer(timer.get_seconds());
    }

    key = input_handler->scan();
    action = key_mapper.map_key(key);

    if (action != Action::INVALID) {
        rule->process(action);
        renderer->render_next_block(tetromino_queue.get_tetrominos());
        renderer->render_board(board, board.get_active_mino());
        renderer->render_hold(board.get_saved_mino());
    }

    attack = rule->update_score();

    network->send_udp(board, board.get_active_mino(), attack, another_user_ip);

    if (attack > 0) renderer->render_score(attack);

    if (network->recv_udp(recv_pkt)) {
        renderer->render_other_board(recv_pkt);

        // 같은 타이밍에 서로 공격한 경우 상쇄됨
        if (recv_pkt.deleted_line > attack) {
            // 예시: 상대방이 garbage를 5 던졌는데, 내가 같은 타이밍에 3 던졌으면, 3만큼은
            // 상쇄되고 2만 적용됨 반대로 상대편 측에서는 5 던졌는데, 상대가 3을 던졌으므로
            // garbage를 모두 상쇄하여 아무 불이익도 받지 않음
            is_line_fill_complete = board.insert_line(recv_pkt.deleted_line - attack);
            if (!is_line_fill_complete) return EngineState::GAME_OVER;
            renderer->render_board(board, board.get_active_mino());
            renderer->render_hold(board.get_saved_mino());
        }
    }

    return EngineState::RUNNING;
}

int MultiEngine::finish()
{
    delete renderer;
    delete input_handler;
    delete network;
    return 0;
}

MultiEngine::~MultiEngine() {}
