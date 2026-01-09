#include "terminal_input.hpp"

#include <conio.h>
#include <stdio.h>
char TerminalInput::scan()
{
	char c = '\0';
	
		while (_kbhit()) { 
			c =_getch();//입력버퍼를 지우며 마지막 값만 반환
		}
		return c;

}

TerminalInput::~TerminalInput()
{
}
