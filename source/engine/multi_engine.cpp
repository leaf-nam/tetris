#include "engine/multi_engine.hpp"
#include "board/board.hpp"
#include "game_rule/rule_factory.hpp"
#include "game_rule/key_mapper.hpp"
#include "tetromino/tetromino_queue.hpp"
#include "util/timer.hpp"
#include "util/action.hpp"

#include <thread>
#include <chrono>
#include <algorithm>
#include <random>

using namespace std;

MultiEngine::MultiEngine(IInputHandler* input_handler, IRenderer* renderer, INetwork* network) : Engine(input_handler, renderer, network) {}

void MultiEngine::run()
{
    Board board;
    unique_ptr<GameRule> rule = create_rule("VERSUS", board);
    KeyMapper key_mapper;
    TetrominoQueue& tetromino_queue = TetrominoQueue::get_instance();
    Timer& timer = Timer::get_instance();

    packet recv_pkt;
    int curr_mino = 0;
    int action;
    int attack = 0, new_attack;
    bool is_line_fill_complete = false;
    int key;
    int index = 0;
    char another_user_ip[1024];
    char c;

    renderer->renderIPRecv();
    while(true)
    {
        c = input_handler->scan();
        if(c == '\n' || c == '\r')
            break;
        if(c != 0)
        {
            renderer->renderChar(c);
            another_user_ip[index++] = c;
        }
    }
    another_user_ip[index] = '\0';
    renderer->renderClear();

    renderer->renderBackground();
    renderer->renderBoard(board, board.get_active_mino());
    renderer->renderHold(board.get_saved_mino());
    renderer->renderScore(attack);
    renderer->renderLevel(rule->get_level());
    renderer->renderTimer(timer.get_seconds());
    while (1)
    {
        if(!board.has_active_mino())
        {
            if (!board.spawn_mino(tetromino_queue.get_new_tetromino())) break;
            renderer->renderNextBlock(tetromino_queue.get_tetrominos());
        }

        timer.set_curr_time();
        if (timer.check_500ms_time())
        {
            rule->process(Action::DROP);
            renderer->renderBoard(board, board.get_active_mino());
            renderer->renderHold(board.get_saved_mino());
            renderer->renderScore(attack);
            renderer->renderLevel(rule->get_level());
            renderer->renderTimer(timer.get_seconds());
            network->send_udp(board, board.get_active_mino(), -1, another_user_ip);
        }
        
        key = input_handler->scan();
        action = key_mapper.map_key(key);

        if (action != Action::INVALID) 
        {
            rule->process(action);
            renderer->renderNextBlock(tetromino_queue.get_tetrominos());
            renderer->renderBoard(board, board.get_active_mino());
            renderer->renderHold(board.get_saved_mino());
            network->send_udp(board, board.get_active_mino(), 0, another_user_ip);
        }
        
        new_attack = rule->update_score();

        if (new_attack) 
        {
            attack = new_attack;
            renderer->renderBoard(board, board.get_active_mino());
            renderer->renderHold(board.get_saved_mino());
            renderer->renderScore(attack);
            renderer->renderLevel(rule->get_level());
            renderer->renderTimer(timer.get_seconds());

            network->send_udp(board, board.get_active_mino(), attack, another_user_ip);
        }
        
        
        if(network->recv_udp(recv_pkt))
        {
            renderer->renderOtherBoard(recv_pkt);

            // 같은 타이밍에 서로 공격한 경우 상쇄됨
            if (recv_pkt.attack > attack)
            {
                // 예시: 상대방이 garbage를 5 던졌는데, 내가 같은 타이밍에 3 던졌으면, 3만큼은 상쇄되고 2만 적용됨
                // 반대로 상대편 측에서는 5 던졌는데, 상대가 3을 던졌으므로 garbage를 모두 상쇄하여 아무 불이익도 받지 않음
                is_line_fill_complete = board.insert_line(recv_pkt.attack - attack);
                if (!is_line_fill_complete)
                    break;
                renderer->renderBoard(board, board.get_active_mino());
                renderer->renderHold(board.get_saved_mino());
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
