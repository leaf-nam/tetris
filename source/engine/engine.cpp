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

Engine::Engine(IInputHandler* input_handler, IRenderer* renderer, INetwork* network) : input_handler(input_handler), renderer(renderer), network(network) {}
