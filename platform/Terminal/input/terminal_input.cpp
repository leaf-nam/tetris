#include "terminal_input.hpp"
#include "api/IInputHandler.hpp"
#include <conio.h>
#include <stdio.h>
char TInput::scan()
{
	//버퍼에 값이 있으면 char 로 넘겨주고 없으면 -1 반환

	if (_kbhit()) 
	{
		return _getch();
	}
	return -1;
}

TInput::~TInput()
{
}
