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
	void print_plate();
};