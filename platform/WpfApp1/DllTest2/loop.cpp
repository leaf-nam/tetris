#include "pch.h"
#include "loop.h"
#include <thread>
#include <atomic>
#include <chrono>

static CallbackFunc g_cb = nullptr;
static std::thread g_worker;
static std::atomic<bool> g_running(false);

extern "C" {

    void RegisterCallback(CallbackFunc cb) {
        g_cb = cb;
    }

    void StartLoop() {
        g_running = true;
        g_worker = std::thread([]() {
            int cnt = 0;
            while (g_running) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                cnt++;

                if (g_cb)
                    g_cb(cnt); // C# 콜백 호출
            }
            });
    }

    void StopLoop() {
        g_running = false;
        g_cb = nullptr;
        if (g_worker.joinable())
            g_worker.join();
    }

}