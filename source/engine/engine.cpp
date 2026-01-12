#include "engine/engine.hpp"
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

Engine::Engine(IInputHandler* input_handler, IRenderer* renderer, INetwork* network) : input_handler(input_handler), renderer(renderer), network(network) {}

void Engine::run()
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
    const char* another_user_ip = "127.0.0.1";

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

void Engine::stop() {}

int Engine::finish()
{
    delete renderer;
    delete input_handler;
    return 0;
}

Engine::~Engine() {}
