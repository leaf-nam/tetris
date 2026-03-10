#include "game_rule/action_resolver.hpp"
#include "game_rule/rule_blitz.hpp"
#include "game_rule/rule_forty_lines.hpp"
#include "game_rule/rule_versus.hpp"
#include "game_rule/key_mapper.hpp"     
#include "game_rule/rule_factory.hpp"
#include "game_rule/rule.hpp"
#include "game_rule/rule_zen.hpp"

#include <chrono>
#include <gtest/gtest.h>
#include <thread>
#include <vector>

TEST(ActionResolverResolveMoveTest, all)
{
    ActionResolver action_resolver;
    Pose return_value;

    return_value = action_resolver.resolve_move(3, 3, 3, Action::DROP);
    EXPECT_EQ(std::get<0>(return_value), 4);
    EXPECT_EQ(std::get<1>(return_value), 3);
    EXPECT_EQ(std::get<2>(return_value), 3);
    return_value = action_resolver.resolve_move(3, 3, 3, Action::LEFT);
    EXPECT_EQ(std::get<0>(return_value), 3);
    EXPECT_EQ(std::get<1>(return_value), 2);
    EXPECT_EQ(std::get<2>(return_value), 3);
    return_value = action_resolver.resolve_move(3, 3, 3, Action::RIGHT);
    EXPECT_EQ(std::get<0>(return_value), 3);
    EXPECT_EQ(std::get<1>(return_value), 4);
    EXPECT_EQ(std::get<2>(return_value), 3);
    return_value = action_resolver.resolve_move(3, 3, 3, Action::HARD_DROP);
    EXPECT_EQ(std::get<0>(return_value), 3);
    EXPECT_EQ(std::get<1>(return_value), 3);
    EXPECT_EQ(std::get<2>(return_value), 3);
}

TEST(ActionResolverResolveRotationTest, all)
{
    ActionResolver action_resolver;
    std::vector<Pose> return_value;

    return_value = action_resolver.resolve_rotation(3, 3, 0, MinoType::I, Action::ROTATE_CCW, true);
    EXPECT_EQ(return_value.size(), 5);
    for (int i = 0; i < KICK_TEST; ++i) {
        auto [dr, dc] = KICK_TABLE_I[0][1][i];
        EXPECT_EQ(std::get<0>(return_value[i]), 3 + dr);
        EXPECT_EQ(std::get<1>(return_value[i]), 3 + dc);
        EXPECT_EQ(std::get<2>(return_value[i]), 3);
    }

    return_value = action_resolver.resolve_rotation(3, 3, 3, MinoType::T, Action::ROTATE_CCW, true);
    EXPECT_EQ(return_value.size(), 5);
    for (int i = 0; i < KICK_TEST; ++i) {
        auto [dr, dc] = KICK_TABLE_JLSTZ[3][1][i];
        EXPECT_EQ(std::get<0>(return_value[i]), 3 + dr);
        EXPECT_EQ(std::get<1>(return_value[i]), 3 + dc);
        EXPECT_EQ(std::get<2>(return_value[i]), 2);
    }

    return_value = action_resolver.resolve_rotation(3, 3, 3, MinoType::O, Action::ROTATE_CW, false);
    EXPECT_EQ(return_value.size(), 1);
    EXPECT_EQ(std::get<0>(return_value[0]), 3);
    EXPECT_EQ(std::get<1>(return_value[0]), 3);
    EXPECT_EQ(std::get<2>(return_value[0]), 3);
}

TEST(KeyMapperMapKeyTest, all)
{
    KeyMapper key_mapper;
    Action return_value;

    return_value = key_mapper.map_key('w');
    EXPECT_EQ(return_value, Action::SWAP);

    return_value = key_mapper.map_key('u');
    EXPECT_EQ(return_value, Action::INVALID);
}

TEST(GameRuleProcessTest, all)
{
}

TEST(RuleBlitzTest, all)
{
}

TEST(RuleFortyLinesTest, all)
{
}

TEST(RuleVersusTest, all)
{
}

TEST(RuleFactoryTest, all)
{
}

TEST(RuleZenTest, all)
{
}
