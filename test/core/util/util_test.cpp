#include <gtest/gtest.h>
#include "util/timer.hpp"
#include "util/rand_gen.hpp"
#include <thread>
#include <chrono>

TEST(TimerTest, all)
{
    Timer &t = Timer::get_instance();
    t.set_curr_time();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    t.set_curr_time();
    EXPECT_EQ(t.check_500ms_time(), true);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    t.set_curr_time();
    EXPECT_EQ(t.check_500ms_time(), false);
    std::this_thread::sleep_for(std::chrono::milliseconds(1300));
    t.set_curr_time();
    EXPECT_EQ(t.get_seconds(), 2);
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    t.set_curr_time();
    EXPECT_EQ(t.get_seconds(), 2);
    std::this_thread::sleep_for(std::chrono::milliseconds(700));
    t.set_curr_time();
    EXPECT_EQ(t.get_seconds(), 3);
}

TEST(RandGenTest, all)
{
    RandGen &r = RandGen::get_instance();
    int rand_num = r.get_rand_int();
    EXPECT_TRUE((rand_num >= 0) && (rand_num <= 10000));
}
