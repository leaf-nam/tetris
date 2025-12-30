#include "input.h"

input::input(block current_block, board current_board, char current_order) :current_block(current_block), current_board(current_board), current_order(current_order) {}

void input::get_user_input()
{
	cin >> current_order;
}

bool input::activate_block()
{
	int x = current_block.get_x();
	int y = current_block.get_y();
	bool is_gameover = false;

	switch (current_order)
	{
	case 'a':
		if (current_board.gameover_check(1, -1, 0))
		{
			is_gameover = true;
			break;
		}
		if (current_board.left_right_collision_check(1, -1, 0))
			break;
		if (current_board.down_collision_check(1, -1, 0))
			break;
		if (current_board.block_collision_check(1, -1, 0))
			break;
		current_block.x_move(-1);
		current_block.y_move(1);
		break;
	case 'd':
		if (current_board.gameover_check(1, 1, 0))
		{
			is_gameover = true;
			break;
		}
		if (current_board.left_right_collision_check(1, 1, 0))
			break;
		if (current_board.down_collision_check(1, 1, 0))
			break;
		if (current_board.block_collision_check(1, 1, 0))
			break;
		current_block.x_move(1);
		current_block.y_move(1);
		break;
	case 's':
		if (current_board.gameover_check(1, 0, 90))
		{
			is_gameover = true;
			break;
		}
		if (current_board.left_right_collision_check(1, 0, 90))
			break;
		if (current_board.down_collision_check(1, 0, 90))
			break;
		if (current_board.block_collision_check(1, 0, 90))
			break;
		current_block.rotate(90);
		current_block.y_move(1);
		break;
	}

	if (is_gameover == false)
	{
		current_board.update();
	}
	return is_gameover;
}