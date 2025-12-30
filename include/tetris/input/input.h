#pragma once
#include "block.h"
#include "board.h"

class input
{
private:
	char current_order;
	block current_block;
	board current_board;
public:
	static char get_async_input();
	input(block current_block, board current_board, char current_order);
	void get_user_input();
	bool activate_block();
};