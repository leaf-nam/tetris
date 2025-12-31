#pragma once
#include <block/block.h>
#include <board/board.h>

class input
{
private:
	char current_order;
	block current_block;
public:
	input(char current_order);
	static char get_async_input();
	void set_current_block(block current_block);
	void get_user_input();
	bool activate_block();
};