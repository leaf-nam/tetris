#pragma once
#include "board.h"

class render
{
private:
	int start_x;
	int start_y;
	board current_board;
public:
	render(int start_x, int start_y, board current_board);
	void prompt(int x, int y);
	void print_plate();
};