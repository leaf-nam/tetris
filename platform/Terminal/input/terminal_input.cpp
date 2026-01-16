#include "terminal_input.hpp"

#include <conio.h>
#include <stdio.h>

int TerminalInput::scan()
{
    char c = '\0';

    while (_kbhit() != 0) {
        c = _getch(); // �Է¹��۸� ����� ������ ���� ��ȯ

        if (c == 224)
        {
            c = _getch();
            if (c == 72) return Arrow::UP;
            if (c == 80) return Arrow::DOWN;
            if (c == 75) return Arrow::LEFT;
            if (c == 77) return Arrow::RIGHT;
        } 
    }

    return c;
}

TerminalInput::~TerminalInput() {}
