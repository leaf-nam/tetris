#include "engine/multi_engine.hpp"

#include <chrono>
#include <random>
#include <thread>
#include <vector>
#include <string>

using namespace std;

MultiEngine::MultiEngine(Setting* setting, IInputHandler* input_handler, IRenderer* renderer, INetwork* network, Lobby* lobby) : 
    Engine(setting, input_handler, renderer, network, lobby) {}

void MultiEngine::init(bool is_server)
{
    rule = create_rule("VERSUS", board);
    ids_ips = lobby->get_client_ids_ips();
    active_user = lobby->get_ids(is_server);

    for (auto [key, value] : active_user) {
        if (key == lobby->get_my_id()) continue;
        active_user_time_checker[key] = 0;
    }

    curr_mino = 0;
    attack = 0;
    is_line_fill_complete = false;
    is_tetromino_or_board_change = false;
    index = 0;
    is_game_over = 0;
    is_win = 0;

    renderer->render_background();
    renderer->render_board(board, board.get_active_mino());
    renderer->render_hold(board.get_saved_mino());
    renderer->render_score(attack);
    renderer->render_level(rule->get_level());
    renderer->render_timer(timer.get_seconds());
}

bool MultiEngine::run(bool is_server)
{
    is_tetromino_or_board_change = false;

    if(!board.has_active_mino())
    {
        if (!board.spawn_mino(TetrominoQueue::get_instance().get_new_tetromino())) {
            attack = rule->update_score();
            is_game_over = 1;
            is_win = 0;
            if (is_server == true)
                network->send_multi_udp(board, board.get_active_mino(), attack, is_game_over,
                                        is_win, lobby->get_my_id(), ids_ips);
            else
                network->send_udp(board, board.get_active_mino(), attack, is_game_over, is_win,
                                  lobby->get_server_ip_address(), lobby->get_my_id());
            active_user.erase(lobby->get_my_id());
            renderer->render_game_over();
            goto out;
        }
        renderer->render_next_block(TetrominoQueue::get_instance().get_tetrominos());
        is_tetromino_or_board_change = true;
    }

    timer.set_curr_time();
    if (timer.check_500ms_time()) {
        for (auto it = active_user_time_checker.begin(); it != active_user_time_checker.end();) {
            it->second++;
            if (it->second >= 7) {
                active_user.erase(it->first);
                renderer->render_other_timeout(it->first);
                it = active_user_time_checker.erase(it);
            }
            else {
                ++it;
            }
        }

        if (active_user.size() == 1) {
            renderer->render_win();
            attack = rule->update_score();
            is_game_over = 0;
            is_win = 1;
            if (is_server == true)
                network->send_multi_udp(board, board.get_active_mino(), attack, is_game_over,
                                        is_win, lobby->get_my_id(), ids_ips);
            else
                network->send_udp(board, board.get_active_mino(), attack, is_game_over, is_win,
                                  lobby->get_server_ip_address(), lobby->get_my_id());
            active_user.erase(lobby->get_my_id());
            goto out;
        }

        rule->process(Action::DROP);
        renderer->render_level(rule->get_level());
        renderer->render_timer(timer.get_seconds());
        is_tetromino_or_board_change = true;
    }

    key = input_handler->scan();
    action = key_mapper.map_key(key);

    if (action != Action::INVALID) {
        rule->process(action);
        renderer->render_next_block(TetrominoQueue::get_instance().get_tetrominos());
        is_tetromino_or_board_change = true;
    }

    attack = rule->update_score();

    if (is_tetromino_or_board_change) {
        renderer->render_board(board, board.get_active_mino());
        renderer->render_hold(board.get_saved_mino());
        is_game_over = 0;
        is_win = 0;
        if (is_server == true)
            network->send_multi_udp(board, board.get_active_mino(), attack, is_game_over, is_win,
                                    lobby->get_my_id(), ids_ips);
        else
            network->send_udp(board, board.get_active_mino(), attack, is_game_over, is_win,
                              lobby->get_server_ip_address(), lobby->get_my_id());
    }

    if (attack > 0) renderer->render_score(attack);

    if(network->recv_udp(recv_pkt))
    {
        if (active_user.find(recv_pkt.id) == active_user.end() || strcmp(lobby->get_my_id(), recv_pkt.id) == 0)
            return true;
        else
            active_user_time_checker[recv_pkt.id] = 0;
        renderer->render_other_board(recv_pkt);

        if (recv_pkt.is_game_over == 1) {
            renderer->render_other_game_over(recv_pkt);
            active_user.erase(recv_pkt.id);
        }

        if (active_user.size() == 1)
        {
            renderer->render_win();
            attack = rule->update_score();
            is_game_over = 0;
            is_win = 1;
            if (is_server == true)
                network->send_multi_udp(board, board.get_active_mino(), attack, is_game_over,
                                        is_win, lobby->get_my_id(), ids_ips);
            else
                network->send_udp(board, board.get_active_mino(), attack, is_game_over, is_win,
                                  lobby->get_server_ip_address(), lobby->get_my_id());
            active_user.erase(lobby->get_my_id());
            goto out;
        }

        if (is_server) network->send_relay_udp(recv_pkt, ids_ips);

        // 같은 타이밍에 서로 공격한 경우 상쇄됨
        if (recv_pkt.deleted_line > attack) {
            // 예시: 상대방이 garbage를 5 던졌는데, 내가 같은 타이밍에 3 던졌으면, 3만큼은
            // 상쇄되고 2만 적용됨 반대로 상대편 측에서는 5 던졌는데, 상대가 3을 던졌으므로
            // garbage를 모두 상쇄하여 아무 불이익도 받지 않음
            is_line_fill_complete = board.insert_line(recv_pkt.deleted_line - attack);
            if (!is_line_fill_complete) {
                renderer->render_game_over();
                attack = rule->update_score();
                is_game_over = 1;
                is_win = 0;
                if (is_server == true)
                    network->send_multi_udp(board, board.get_active_mino(), attack, is_game_over,
                                            is_win, lobby->get_my_id(), ids_ips);
                else
                    network->send_udp(board, board.get_active_mino(), attack, is_game_over, is_win,
                                      lobby->get_server_ip_address(), lobby->get_my_id());
                active_user.erase(lobby->get_my_id());
                goto out;
            }
            renderer->render_board(board, board.get_active_mino());
            renderer->render_hold(board.get_saved_mino());
        }
    }

    return true;
out:
    return false;
}

bool MultiEngine::stop(bool is_server)
{
    int three_sec_over = 0;

    if (active_user.size() > 0)
    {
        if (network->recv_udp(recv_pkt)) {
            if (active_user.find(recv_pkt.id) != active_user.end()) {
                active_user_time_checker[recv_pkt.id] = 0;

                if (is_server) network->send_relay_udp(recv_pkt, ids_ips);
                renderer->render_other_board(recv_pkt);
                if (recv_pkt.is_game_over == 1) {
                    renderer->render_other_game_over(recv_pkt);
                    active_user.erase(recv_pkt.id);
                }
                if (recv_pkt.is_win == 1) {
                    renderer->render_other_win(recv_pkt);
                    goto out;
                }
            }
        }

        timer.set_curr_time();
        if (timer.check_500ms_time()) {
            for (auto it = active_user_time_checker.begin(); it != active_user_time_checker.end();) {
                it->second++;
                if (it->second >= 7) {
                    active_user.erase(it->first);
                    renderer->render_other_timeout(it->first);
                    it = active_user_time_checker.erase(it);
                }
                else {
                    ++it;
                }
            }
            if (is_server)
                network->send_multi_udp(board, board.get_active_mino(), 0, is_game_over,
                                        is_win, lobby->get_my_id(), ids_ips);
            else
                network->send_udp(board, board.get_active_mino(), 0, is_game_over, is_win,
                                  lobby->get_server_ip_address(), lobby->get_my_id());
        }
    }
    else
        goto out;

    return true;
out:
    while (three_sec_over < 7) {
        timer.set_curr_time();
        if (timer.check_500ms_time()) {
            if (is_server)
                network->send_multi_udp(board, board.get_active_mino(), 0, is_game_over, is_win,
                                        lobby->get_my_id(), ids_ips);
            else
                network->send_udp(board, board.get_active_mino(), 0, is_game_over, is_win,
                                  lobby->get_server_ip_address(), lobby->get_my_id());
            three_sec_over++;
        }
    }
    return false;
}

int MultiEngine::finish() { return 0; }

MultiEngine::~MultiEngine() {}
