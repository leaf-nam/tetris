#include "engine/multi_engine.hpp"
#include "board/board.hpp"
#include "game_rule/rule_factory.hpp"
#include "tetromino/tetromino_queue.hpp"
#include "util/timer.hpp"
#include "util/action.hpp"
#include "util/path.hpp"

#include <thread>
#include <chrono>
#include <algorithm>
#include <random>

using namespace std;

MultiEngine::MultiEngine(IInputHandler* input_handler, IRenderer* renderer, INetwork* network) : Engine(input_handler, renderer, network) {}

void MultiEngine::run()
{
    PathService& path = PathService::get_instance();
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

    renderer->renderIPRecv();
    while(true)
    {
        c = input_handler->scan();
        if(c == '\n')
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
    renderer->renderScore(score);
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
            renderer->renderScore(score);
            renderer->renderLevel(rule->get_level());
            renderer->renderTimer(timer.get_seconds());
            is_level_up = rule->time_and_level_update();
            network->send_udp(board, board.get_active_mino(), another_user_ip);
        }
        
        key = input_handler->scan();
        if(key)
        {
            switch(key)
            {
                case 'a': action = Action::LEFT; break;
                case 's': action = Action::DROP; break;
                case 'd': action = Action::RIGHT; break;
                case 'q': action = Action::ROTATE_CCW; break;
                case 'e': action = Action::ROTATE_CW; break;
                case 'f': action = Action::HARD_DROP; break;
                case 'w': action = Action::SWAP; break;
                default: action = -1; break;
            }
        }
        else
            action = -1;

        if (action != -1) 
        {
            rule->process(action);
            renderer->renderNextBlock(tetromino_queue.get_tetrominos());
            renderer->renderBoard(board, board.get_active_mino());
            renderer->renderHold(board.get_saved_mino());
            network->send_udp(board, board.get_active_mino(), another_user_ip);
        }
        
        new_score = rule->update_score();
        if(is_level_up)
            if(!board.insert_line(3))
            {
                renderer->renderBoard(board, board.get_active_mino());
                renderer->renderHold(board.get_saved_mino());
                break;
            }
        if (new_score || is_level_up) 
        {
            score += new_score;
            renderer->renderBoard(board, board.get_active_mino());
            renderer->renderHold(board.get_saved_mino());
            renderer->renderScore(score);
            renderer->renderLevel(rule->get_level());
            renderer->renderTimer(timer.get_seconds());
            is_level_up = false;
        }
        
        packet recv_pkt;
        if(network->recv_udp(recv_pkt))
            renderer->renderOtherBoard(recv_pkt);
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
