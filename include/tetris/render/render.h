#pragma once
#include <board/board.h>

class render
{
private:
	int start_y;
	int start_x;
public:
	render(int start_y, int start_x);
	void prompt(int x, int y);
	void print_plate();
};