#include "engine/engine.hpp"

#include "board/board.hpp"
#include "game_rule/rule_factory.hpp"
#include "tetromino/tetromino_queue.hpp"
#include "util/action.hpp"
#include "util/timer.hpp"
// #include "util/path.hpp"

#include <algorithm>
#include <chrono>
#include <random>
#include <thread>

Engine::Engine(Setting* setting, IInputHandler* input_handler, IRenderer* renderer,
               INetwork* network)
    : setting(setting), input_handler(input_handler), renderer(renderer), network(network)
{
}
