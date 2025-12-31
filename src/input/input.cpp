#include <input/input.h>
#include <iostream>
#include <future>
#include <thread>
#include <chrono>
using namespace std;

input::input(char current_order) :current_order(current_order) {}

char input::get_async_input()
{
	char input;
	cin >> input;
	return input;
}

void input::set_current_block(block current_block)
{
	this->current_block = current_block;
}

void input::get_user_input()
{
	chrono::seconds timeout(0);
	char input = 'x';
	future<char> future = async(get_async_input);
	if (future.wait_for(timeout) == future_status::ready)
		input = future.get();

	current_order = input;
}

bool input::activate_block()
{
	board& current_board = board::get_instance();
	int x = current_block.get_x();
	int y = current_block.get_y();
	bool is_gameover = false;
	bool is_block_move_stop = false;

	switch (current_order)
	{
	case 'a':
		if (current_board.gameover_check(1, -1, 0))
		{
			is_block_move_stop = true;
			is_gameover = true;
			break;
		}
		if (current_board.left_right_collision_check(1, -1, 0))
		{
			is_block_move_stop = true;
			break;
		}
		if (current_board.down_collision_check(1, -1, 0))
		{
			is_block_move_stop = true;
			break;
		}
		if (current_board.block_collision_check(1, -1, 0))
		{
			is_block_move_stop = true;
			break;
		}
		current_block.x_move(-1);
		current_block.y_move();
		break;
	case 'd':
		if (current_board.gameover_check(1, 1, 0))
		{
			is_block_move_stop = true;
			is_gameover = true;
			break;
		}
		if (current_board.left_right_collision_check(1, 1, 0))
		{
			is_block_move_stop = true;
			break;
		}
		if (current_board.down_collision_check(1, 1, 0))
		{
			is_block_move_stop = true;
			break;
		}
		if (current_board.block_collision_check(1, 1, 0))
		{
			is_block_move_stop = true;
			break;
		}
		current_block.x_move(1);
		current_block.y_move();
		break;
	case 's':
		if (current_board.gameover_check(1, 0, 90))
		{
			is_block_move_stop = true;
			is_gameover = true;
			break;
		}
		if (current_board.left_right_collision_check(1, 0, 90))
		{
			is_block_move_stop = true;
			break;
		}
		if (current_board.down_collision_check(1, 0, 90))
		{
			is_block_move_stop = true;
			break;
		}
		if (current_board.block_collision_check(1, 0, 90))
		{
			is_block_move_stop = true;
			break;
		}
		current_block.rotate(90);
		current_block.y_move();
		break;
	default:
		if (current_board.gameover_check(1, 0, 0))
		{
			is_block_move_stop = true;
			is_gameover = true;
			break;
		}
		if (current_board.left_right_collision_check(1, 0, 0))
		{
			is_block_move_stop = true;
			break;
		}
		if (current_board.down_collision_check(1, 0, 0))
		{
			is_block_move_stop = true;
			break;
		}
		if (current_board.block_collision_check(1, 0, 0))
		{
			is_block_move_stop = true;
			break;
		}
		current_block.y_move();
		break;
	}

	current_board.update(is_block_move_stop);
	if (is_block_move_stop == true)
	{
		block new_block;
		current_board.set_current_block(new_block);
		current_block = new_block;
	}
	return is_gameover;
}