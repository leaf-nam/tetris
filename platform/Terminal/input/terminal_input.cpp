#include "terminal_input.hpp"
#include "api/IInputHandler.hpp"
#include <conio.h>
#include <stdio.h>
char TInput::scan()
{
	if (_kbhit()) 
	{
		return _getch();
	}
	return -1;
}

TInput::~TInput()
{
}
