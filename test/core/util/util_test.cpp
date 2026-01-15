#include <gtest/gtest.h>
#include "util/timer.hpp"
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
