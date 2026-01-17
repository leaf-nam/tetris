#include "input/window_input.hpp"

#include <conio.h>
#include <stdio.h>

int WindowInput::scan()
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

WindowInput::~WindowInput() {}
