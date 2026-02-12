#include "engine/multi_engine.hpp"

#include "board/board.hpp"
#include "game_rule/key_mapper.hpp"
#include "game_rule/rule_factory.hpp"
#include "tetromino/tetromino_queue.hpp"
#include "util/action.hpp"
#include "util/timer.hpp"

#include <algorithm>
#include <chrono>
#include <random>
#include <thread>
#include <vector>
#include <string>

using namespace std;

MultiEngine::MultiEngine(Setting* setting, IInputHandler* input_handler, IRenderer* renderer, INetwork* network, Lobby* lobby) : 
    Engine(setting, input_handler, renderer, network, lobby) {}

void MultiEngine::run(bool is_server)
{
    Board board;
    unique_ptr<GameRule> rule = create_rule("VERSUS", board);
    KeyMapper key_mapper;
    TetrominoQueue& tetromino_queue = TetrominoQueue::get_instance();
    Timer& timer = Timer::get_instance();
    std::vector<std::pair<std::string, std::string>> ids_ips = lobby->get_client_ids_ips();
    std::unordered_map<std::string, std::string> active_user = lobby->get_ids(is_server);
    PacketStruct recv_pkt;
    int curr_mino = 0;
    int action;
    int attack = 0;
    bool is_line_fill_complete = false, is_tetromino_or_board_change = false;
    int key;
    int index = 0;
    char c;

    renderer->render_background();
    renderer->render_board(board, board.get_active_mino());
    renderer->render_hold(board.get_saved_mino());
    renderer->render_score(attack);
    renderer->render_level(rule->get_level());
    renderer->render_timer(timer.get_seconds());

    while (true)
    {
        is_tetromino_or_board_change = false;

        if(!board.has_active_mino())
        {
            if (!board.spawn_mino(tetromino_queue.get_new_tetromino())) {
                attack = rule->update_score();
                if (is_server == true)
                    network->send_multi_udp(board, board.get_active_mino(), attack, 1, 0,
                                            lobby->get_my_id(), ids_ips);
                else
                    network->send_udp(board, board.get_active_mino(), attack, 1, 0,
                                      lobby->get_server_ip_address(), lobby->get_my_id());
                active_user.erase(lobby->get_my_id());
                renderer->render_game_over();
                break;
            }
            renderer->render_next_block(tetromino_queue.get_tetrominos());
            is_tetromino_or_board_change = true;
        }

        timer.set_curr_time();
        if (timer.check_500ms_time()) {
            rule->process(Action::DROP);
            renderer->render_level(rule->get_level());
            renderer->render_timer(timer.get_seconds());
            is_tetromino_or_board_change = true;
        }

        key = input_handler->scan();
        action = key_mapper.map_key(key);

        if (action != Action::INVALID) {
            rule->process(action);
            renderer->render_next_block(tetromino_queue.get_tetrominos());
            is_tetromino_or_board_change = true;
        }

        attack = rule->update_score();

        if (is_tetromino_or_board_change) {
            renderer->render_board(board, board.get_active_mino());
            renderer->render_hold(board.get_saved_mino());
            if (is_server == true)
                network->send_multi_udp(board, board.get_active_mino(), attack, 0, 0,
                                        lobby->get_my_id(), ids_ips);
            else
                network->send_udp(board, board.get_active_mino(), attack,
                                  0, 0,
                                  lobby->get_server_ip_address(), lobby->get_my_id());
        }

        if (attack > 0) renderer->render_score(attack);

        if(network->recv_udp(recv_pkt))
        {
            renderer->render_other_board(recv_pkt);

            if (recv_pkt.is_game_over == 1) {
                renderer->render_other_game_over(recv_pkt);
                active_user.erase(std::string(recv_pkt.id));
            }

            if (active_user.size() == 1)
            {
                renderer->render_win();
                attack = rule->update_score();
                if (is_server == true)
                    network->send_multi_udp(board, board.get_active_mino(), attack, 0, 1,
                                            lobby->get_my_id(), ids_ips);
                else
                    network->send_udp(board, board.get_active_mino(), attack, 0, 1,
                                      lobby->get_server_ip_address(), lobby->get_my_id());
                break;
            }

            if (is_server) network->send_relay_udp(recv_pkt, ids_ips);

            // 같은 타이밍에 서로 공격한 경우 상쇄됨
            if (recv_pkt.deleted_line > attack) {
                // 예시: 상대방이 garbage를 5 던졌는데, 내가 같은 타이밍에 3 던졌으면, 3만큼은
                // 상쇄되고 2만 적용됨 반대로 상대편 측에서는 5 던졌는데, 상대가 3을 던졌으므로
                // garbage를 모두 상쇄하여 아무 불이익도 받지 않음
                is_line_fill_complete = board.insert_line(recv_pkt.deleted_line - attack);
                if (!is_line_fill_complete) {
                    attack = rule->update_score();
                    if (is_server == true)
                        network->send_multi_udp(board, board.get_active_mino(), attack, 1, 0,
                                                lobby->get_my_id(), ids_ips);
                    else
                        network->send_udp(board, board.get_active_mino(), attack, 1, 0,
                                          lobby->get_server_ip_address(), lobby->get_my_id());
                    active_user.erase(lobby->get_my_id());
                    renderer->render_game_over();
                    break;
                }
                renderer->render_board(board, board.get_active_mino());
                renderer->render_hold(board.get_saved_mino());
            }
        }
    }

    while (active_user.size() > 0)
    {
        if (network->recv_udp(recv_pkt)) {
            if (is_server) network->send_relay_udp(recv_pkt, ids_ips);
            renderer->render_other_board(recv_pkt);
            if (recv_pkt.is_game_over == 1) {
                renderer->render_other_game_over(recv_pkt);
                active_user.erase(std::string(recv_pkt.id));
            }
            if (recv_pkt.is_win == 1) {
                renderer->render_other_win(recv_pkt);
                break;
            }
        }
    }
}

void MultiEngine::stop() {}

int MultiEngine::finish() { return 0; }

MultiEngine::~MultiEngine() {}
