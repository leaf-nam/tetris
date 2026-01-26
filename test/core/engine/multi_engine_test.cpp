#include "engine/multi_engine.hpp"
#include "mock_input.hpp"
#include "mock_renderer.hpp"
#include "mock_network.hpp"

#include <gtest/gtest.h>

TEST(MultiEngineTest, all)
{
    MockInput* input = new MockInput();
    IInputHandler* input_handler = input;
    IRenderer* renderer = new MockRenderer();
    INetwork* network = new MockNetwork();
    Setting* setting = new Setting();
    setting->nick_name = "Player";
    setting->server_ip_address = "127.0.0.1";
    setting->server_port = "41234";

    input->input('\n');

    MultiEngine engine(setting, input_handler, renderer, network);

    int state;

    while (1)
    {
        input->input('f');
        state = engine.step();
        if (state == EngineState::GAME_OVER) break;
    }

    EXPECT_EQ(state, EngineState::GAME_OVER);

    engine.finish();
}