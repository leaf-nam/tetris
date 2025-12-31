#include <input/input.h>
#include <iostream>
#include <conio.h>
using namespace std;

input::input()
{
	// 0 => y, 1 => x, 2 => angle
	operation['a' - 'a'][0] = 1;
	operation['a' - 'a'][1] = -1;
	operation['a' - 'a'][2] = 0;

	operation['d' - 'a'][0] = 1;
	operation['d' - 'a'][1] = 1;
	operation['d' - 'a'][2] = 0;

	operation['s' - 'a'][0] = 1;
	operation['s' - 'a'][1] = 0;
	operation['s' - 'a'][2] = 1;

	operation['x' - 'a'][0] = 1;
	operation['x' - 'a'][1] = 0;
	operation['x' - 'a'][2] = 0;
}

input& input::get_instance()
{
	static input instance;
	return instance;
}

void input::set_current_block(block* current_block)
{
	this->current_block = current_block;
}

void input::get_user_input()
{
	current_order = 'x';

	while (_kbhit())
		current_order = _getch();
}

bool input::activate_block()
{
	board& current_board = board::get_instance();
	int x = current_block->get_x();
	int y = current_block->get_y();
	int y_move_num;
	bool is_gameover = false;
	bool is_block_move_stop = false;

	if (current_order == 'w')
	{
		y_move_num = current_board.get_collision_upper_y_move();
		if (current_board.gameover_check(y_move_num, 0, 0))
		{
			is_block_move_stop = true;
			is_gameover = true;
		}
		else
		{
			current_block->y_move(y_move_num);
		}
	}
	else if (current_board.gameover_check(operation[current_order - 'a'][0], operation[current_order - 'a'][1], operation[current_order - 'a'][2]))
	{
		is_block_move_stop = true;
		is_gameover = true;
	}
	else if (current_board.left_right_collision_check(operation[current_order - 'a'][0], operation[current_order - 'a'][1], operation[current_order - 'a'][2]))
		current_block->y_move();
	else if (current_board.down_collision_check(operation[current_order - 'a'][0], operation[current_order - 'a'][1], operation[current_order - 'a'][2]))
		is_block_move_stop = true;
	else if (current_board.block_collision_check(operation[current_order - 'a'][0], 0, 0))
		is_block_move_stop = true;
	else if (!current_board.block_collision_check(operation[current_order - 'a'][0], operation[current_order - 'a'][1], operation[current_order - 'a'][2]))
	{
		if(current_order == 'a')
			current_block->x_move(-1);
		else if(current_order == 'd')
			current_block->x_move(1);
		else if(current_order == 's')
			current_block->rotate(1);
		current_block->y_move();
	}
	else
		current_block->y_move();

	current_board.update(is_block_move_stop);
	current_board.line_delete();

	if (is_block_move_stop == true)
	{
		delete this->current_block;
		block* new_block = new block();
		current_board.set_current_block(new_block);
		current_block = new_block;
	}
	return is_gameover;
}

char input::get_current_order()
{
	return current_order;
}