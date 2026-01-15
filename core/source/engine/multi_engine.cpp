#include "engine/multi_engine.hpp"

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

MultiEngine::MultiEngine(IInputHandler* input_handler, IRenderer* renderer, INetwork* network)
    : Engine(input_handler, renderer, network)
{
}

void MultiEngine::run()
{
    Board board;
    unique_ptr<GameRule> rule = create_rule("ZEN", board);
    TetrominoQueue& tetromino_queue = TetrominoQueue::get_instance();
    Timer& timer = Timer::get_instance();

    int curr_mino = 0;
    int action;
    int score = 0, new_score;
    bool is_level_up = false, is_line_fill_complete = false;
    int key;
    int index = 0;
    char another_user_ip[1024];
    char c;

    renderer->render_ip_recv();
    while (true) {
        c = input_handler->scan();
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
            network->send_udp(board, board.get_active_mino(), -1, another_user_ip);
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
            network->send_udp(board, board.get_active_mino(), -1, another_user_ip);
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

        if (new_score != 0)
            network->send_udp(board, board.get_active_mino(), (new_score / 100), another_user_ip);

        Packet recv_pkt;
        if (network->recv_udp(recv_pkt)) {
            renderer->render_other_board(recv_pkt);
            if (recv_pkt.deleted_line > 1) {
                is_line_fill_complete = board.insert_line(recv_pkt.deleted_line - 1);
                renderer->render_board(board, board.get_active_mino());
                renderer->render_hold(board.get_saved_mino());
                if (!is_line_fill_complete) break;
            }
        }
    }
}

void MultiEngine::stop() {}

int MultiEngine::finish()
{
    delete renderer;
    delete input_handler;
    delete network;
    return 0;
}

MultiEngine::~MultiEngine() {}
