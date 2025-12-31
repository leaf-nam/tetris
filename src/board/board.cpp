#include <board/board.h>

board::board() :deleted_line_count(0), plate_y(20), plate_x(20)
{
	for (int y = 0; y < plate_y; ++y)
	{
		for (int x = 0; x < plate_x; ++x)
		{
			plate[y][x] = 0;
		}
	}
}

board& board::get_instance()
{
	static board instance;
	return instance;
}

void board::set_current_block(block current_block)
{
	this->current_block = current_block;
}

bool board::upper_collision_check(int y_move, int x_move, int angle_move)
{
	int block_type = current_block.get_block_type();
	int angle = (current_block.get_angle() + angle_move) % 360;
	int x = current_block.get_x() + x_move;
	int y = current_block.get_y() + y_move;
	switch (block_type)
	{
	case 1:
		if (angle == 0 || angle == 180)
		{
			if (y < 0 || y + 1 < 0)
				return true;
		}
		else if (angle == 90 || angle == 270)
		{
			if (y < 0 || y - 1 < 0 || y - 2 < 0)
				return true;
		}
		break;
	default:
		break;
	}
	return false;
}

bool board::left_right_collision_check(int y_move, int x_move, int angle_move)
{
	int block_type = current_block.get_block_type();
	int angle = (current_block.get_angle() + angle_move) % 360;
	int x = current_block.get_x() + x_move;
	int y = current_block.get_y() + y_move;
	switch (block_type)
	{
	case 1:
		if (angle == 0 || angle == 180)
		{

			if (x + 1 < 0 || x + 1 >= plate_x || x < 0 || x >= plate_x || x - 1 < 0 || x - 1 >= plate_x)
				return true;
		}
		else if (angle == 90 || angle == 270)
		{
			if (x - 1 < 0 || x - 1 >= plate_x || x < 0 || x >= plate_x)
				return true;
		}
		break;
	default:
		break;
	}
	return false;
}

bool board::down_collision_check(int y_move, int x_move, int angle_move)
{
	int block_type = current_block.get_block_type();
	int angle = (current_block.get_angle() + angle_move) % 360;
	int x = current_block.get_x() + x_move;
	int y = current_block.get_y() + y_move;
	switch (block_type)
	{
	case 1:
		if (angle == 0 || angle == 180)
		{
			if (y >= plate_y || y + 1 >= plate_y)
				return true;
		}
		else if (angle == 90 || angle == 270)
		{
			if (y >= plate_y || y - 1 >= plate_y || y - 2 >= plate_y)
				return true;
		}
		break;
	default:
		break;
	}
	return false;
}

bool board::block_collision_check(int y_move, int x_move, int angle_move)
{
	int block_type = current_block.get_block_type();
	int angle = (current_block.get_angle() + angle_move) % 360;
	int x = current_block.get_x() + x_move;
	int y = current_block.get_y() + y_move;
	switch (block_type)
	{
	case 1:
		if (angle == 0 || angle == 180)
		{
			if (plate[y][x] > 0 || plate[y][x + 1] > 0 || plate[y + 1][x] > 0 || plate[y + 1][x - 1] > 0)
				return true;
		}
		else if (angle == 90 || angle == 270)
		{
			if (plate[y][x] > 0 || plate[y - 1][x] > 0 || plate[y - 1][x - 1] > 0 || plate[y - 2][x - 1] > 0)
				return true;
		}
		break;
	default:
		break;
	}
	return false;
}

bool board::gameover_check(int y_move, int x_move, int angle_move)
{
	int x = current_block.get_x();
	int y = current_block.get_y();
	if (upper_collision_check(y_move, x_move, angle_move))
		return true;
	else if ((x == 10 && y == 0) && block_collision_check(0, 0, 0))
		return true;

	return false;
}

void board::update(bool is_block_move_stop)
{
	int block_type = current_block.get_block_type();
	int angle = current_block.get_angle();
	int x = current_block.get_x();
	int y = current_block.get_y();
	int plate_x = this->plate_x;
	int plate_y = this->plate_y;

	block_type = (is_block_move_stop ? block_type : block_type + 7);

	for (int j = 0; j < plate_y; ++j)
	{
		for (int i = 0; i < plate_x; ++i)
		{
			if (plate[j][i] > 7)
				plate[j][i] = 0;
		}
	}

	switch (block_type)
	{
	case 1:
		if (angle == 0 || angle == 180)
		{
			plate[y][x] = block_type;
			plate[y][x + 1] = block_type;
			plate[y + 1][x] = block_type;
			plate[y + 1][x - 1] = block_type;
		}
		else if (angle == 90 || angle == 270)
		{
			plate[y][x] = block_type;
			plate[y - 1][x] = block_type;
			plate[y - 1][x - 1] = block_type;
			plate[y - 2][x - 1] = block_type;
		}
		break;
	default:
		break;
	}
}

void board::line_delete()
{
	int cnt = 0;
	int line_cnt = 0;
	for (int y = 0; y < plate_y; ++y)
	{
		cnt = 0;
		for (int x = 0; x < plate_x; ++x)
		{
			if (plate[y][x] > 0)
				cnt++;
		}

		if (cnt == plate_x)
			line_cnt++;
	}

	for (int y = line_cnt; y < plate_y; ++y)
	{
		for (int x = 0; x < plate_x; ++x)
		{
			plate[y - line_cnt][x] = plate[y][x];
			plate[y][x] = 0;
		}
	}

	deleted_line_count += line_cnt;
}

int board::get_plate_x()
{
	return plate_x;
}

int board::get_plate_y()
{
	return plate_y;
}

int board::get_plate_num(int y, int x)
{
	return plate[y][x];
}