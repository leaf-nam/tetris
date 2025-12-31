#pragma once
#include <block/block.h>
#include <board/board.h>

using namespace std;

class input
{
private:
	char current_order;
	block* current_block;
public:
	void set_current_block(block* current_block);
	void get_user_input();
	bool activate_block();
	char get_current_order();
};