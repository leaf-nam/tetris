#include "input/window_input.hpp"

#include <conio.h>
#include <stdio.h>
char WindowInput::scan()
{
    char c = '\0';

    while (_kbhit() != 0) {
        c = _getch(); // 입력버퍼를 지우며 마지막 값만 반환
    }
    return c;
}

WindowInput::~WindowInput() {}
