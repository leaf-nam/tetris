#include "terminal-input.hpp"

#include <conio.h>
#include <stdio.h>
char TerminalInput::scan()
{
    char c = '\0';

    while (_kbhit() != 0) {
        c = _getch(); // �Է¹��۸� ����� ������ ���� ��ȯ
    }
    return c;
}

TerminalInput::~TerminalInput() {}
