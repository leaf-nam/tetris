#pragma once
#include <block/block.h>
#include <board/board.h>

using namespace std;

class input
{
private:
	char current_order;
	block* current_block;
	input();
public:
	static input& get_instance();
	input(const input&) = delete;
	input& operator=(const input&) = delete;
	void set_current_block(block* current_block);
	void get_user_input();
	bool activate_block();
	char get_current_order();
};