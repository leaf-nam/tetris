#ifndef __TIMER_HPP__
#define __TIMER_HPP__

#include <chrono>

class Timer
{
    private:
    std::chrono::steady_clock::time_point base_time;
    std::chrono::steady_clock::time_point base_500ms_time;
    std::chrono::steady_clock::time_point curr_time;
    Timer();

    public:
    static Timer& get_instance();
	Timer(const Timer&) = delete;
	Timer& operator=(const Timer&) = delete;
    void set_curr_time();
    bool check_500ms_time();
    int get_seconds();
};

#endif
