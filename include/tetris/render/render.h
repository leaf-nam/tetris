#pragma once
#include <board/board.h>

class render
{
private:
	int start_x;
	int start_y;
public:
	render(int start_x, int start_y);
	void prompt(int x, int y);
	void print_plate();
};