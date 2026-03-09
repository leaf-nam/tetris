#include "input/window_lobby_input_handler.hpp"

#include <string>
#include <stdio.h>
#include <conio.h>

int WindowLobbyInputHandler::scan()
{
    int c = '\0';

    while (_kbhit() != 0) {
        c = _getch();

        if (c == 224) {
            c = _getch();
            if (c == 72) return Arrow::KEY_UP;
            if (c == 80) return Arrow::KEY_DOWN;
            if (c == 75) return Arrow::KEY_LEFT;
            if (c == 77) return Arrow::KEY_RIGHT;
        }
    }

    return (int) c;
}

int WindowLobbyInputHandler::scan(char* buf, int buf_len, int is_blocking)
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

int WindowLobbyInputHandler::scan(int* buf, int is_blocking)
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
