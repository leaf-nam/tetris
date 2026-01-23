#include "ip_resolver_input_handler/window_ip_resolver_input_handler.hpp"

#include <string>
#include <stdio.h>
#include <conio.h>

int WindowIpResolverInputHandler::scan(char* buf, int buf_len, int is_blocking)
{
    std::string s;
    int hit_keyboard = 0;
    
    if (is_blocking == 1)
    {
        scanf_s("%s", buf, buf_len);
        hit_keyboard = 1;
    }
    else if (_kbhit() != 0)
    {
        scanf_s("%s", buf, buf_len);
        hit_keyboard = 1;
    }

    return hit_keyboard;
}

int WindowIpResolverInputHandler::scan(int* buf, int is_blocking)
{
    int hit_keyboard = 0;

    if (is_blocking == 1) {
        scanf_s("%d", buf);
        hit_keyboard = 1;
    }
    else if (_kbhit() != 0) {
        scanf_s("%d", buf);
        hit_keyboard = 1;
    }

    return hit_keyboard;
}