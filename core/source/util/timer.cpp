#include "util/timer.hpp"
#include <chrono>

Timer::Timer()
{
    base_time = std::chrono::steady_clock::now();
    base_500ms_time = base_time;
    curr_time = base_500ms_time;    
}

Timer& Timer::get_instance()
{
    static Timer instance;
    return instance;
}

void Timer::set_curr_time()
{
    curr_time = std::chrono::steady_clock::now();
}

bool Timer::check_500ms_time()
{
    bool is_500ms_pass = false;
    std::chrono::steady_clock::duration diff_time;
    diff_time = curr_time - base_500ms_time;
    
    if (diff_time >= std::chrono::milliseconds(500))
    {
        base_500ms_time = std::chrono::steady_clock::now();
        is_500ms_pass = true;
    }

    return is_500ms_pass;
}

int Timer::get_seconds()
{
    std::chrono::steady_clock::duration diff_time;
    diff_time = curr_time - base_time;

    return std::chrono::duration_cast<std::chrono::seconds>(diff_time).count();
}
