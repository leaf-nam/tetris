#include "engine/solo_engine.hpp"
#include "mock_input.hpp"
#include "mock_renderer.hpp"

#include <gtest/gtest.h>

TEST(SoloEngineTest, all)
{
    MockInput* input = new MockInput();
    IInputHandler* input_handler = input;
    IRenderer* renderer = new MockRenderer();
    Setting* setting = new Setting();
    setting->nick_name = "Player";
    setting->server_ip_address = "127.0.0.1";
    setting->server_port = "41234";

    SoloEngine engine(setting, input_handler, renderer);

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