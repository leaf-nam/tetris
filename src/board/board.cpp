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

	init_tetromino();
}

void board::init_tetromino()
{
	// first: type of tetromino, second: angles, third: block, four: y, x plus or minus(default y x is upper center)
	// 1 => I, 2 => L, 3 => J, 4 => T, 5 => O, 6 => Z, 7 => S;
	// I
	tetromio[1] = { {
		{{{0, 0}, {1, 0}, {2, 0}, {3, 0}}},
		{{{0, -1}, {0, 0}, {0, 1}, {0, 2}}},
		{{{0, 0}, {1, 0}, {2, 0}, {3, 0}}},
		{{{0, -1}, {0, 0}, {0, 1}, {0, 2}}},
	} };

	// L
	tetromio[2] = { {
		{{{0, 0}, {1, 0}, {2, 0}, {2, 1}}},
		{{{0, -1}, {0, 0}, {0, 1}, {1, -1}}},
		{{{0, 0}, {0, 1}, {1, 1}, {2, 1}}},
		{{{0, 1}, {1, -1}, {1, 0}, {1, 1}}},
	} };

	// J
	tetromio[3] = { {
		{{{0, 0}, {1, 0}, {2, 0}, {2, -1}}},
		{{{0, -1}, {1, -1}, {1, 0}, {1, 1}}},
		{{{0, 0}, {0, -1}, {1, -1}, {2, -1}}},
		{{{0, -1}, {0, 0}, {0, 1}, {1, 1}}},
	} };

	// T
	tetromio[4] = { {
		{{{1, -1}, {1, 0}, {1, 1}, {0, 0}}},
		{{{0, -1}, {1, -1}, {1, 0}, {2, -1}}},
		{{{0, -1}, {0, 0}, {0, 1}, {1, 0}}},
		{{{0, 0}, {1, -1}, {1, 0}, {2, 0}}},
	} };

	// O
	tetromio[5] = { {
		{{{0, 0}, {0, 1}, {1, 0}, {1, 1}}},
		{{{0, 0}, {0, 1}, {1, 0}, {1, 1}}},
		{{{0, 0}, {0, 1}, {1, 0}, {1, 1}}},
		{{{0, 0}, {0, 1}, {1, 0}, {1, 1}}},
	} };

	// Z
	tetromio[6] = { {
		{{{0, -1}, {0, 0}, {1, 0}, {1, 1}}},
		{{{0, 0}, {1, -1}, {1, 0}, {2, -1}}},
		{{{0, -1}, {0, 0}, {1, 0}, {1, 1}}},
		{{{0, 0}, {1, -1}, {1, 0}, {2, -1}}},
	} };

	// S
	tetromio[7] = { {
		{{{0, 0}, {0, 1}, {1, -1}, {1, 0}}},
		{{{0, -1}, {1, -1}, {1, 0}, {2, 0}}},
		{{{0, 0}, {0, 1}, {1, -1}, {1, 0}}},
		{{{0, -1}, {1, -1}, {1, 0}, {2, 0}}},
	} };
}

board& board::get_instance()
{
	static board instance;
	return instance;
}

void board::set_current_block(block* current_block)
{
	this->current_block = current_block;
}

bool board::upper_collision_check(int y_move, int x_move, int angle_move)
{
	int block_type = current_block->get_block_type();
	int angle = (current_block->get_angle() + angle_move) % 4;
	int x = current_block->get_x() + x_move;
	int y = current_block->get_y() + y_move;

	if (y + tetromio[block_type][angle][0][0] < 0 ||
		y + tetromio[block_type][angle][1][0] < 0 ||
		y + tetromio[block_type][angle][2][0] < 0 ||
		y + tetromio[block_type][angle][3][0] < 0)
		return true;
	else
		return false;
}

bool board::left_right_collision_check(int y_move, int x_move, int angle_move)
{
	int block_type = current_block->get_block_type();
	int angle = (current_block->get_angle() + angle_move) % 4;
	int x = current_block->get_x() + x_move;
	int y = current_block->get_y() + y_move;

	if ((x + tetromio[block_type][angle][0][1] < 0 || x + tetromio[block_type][angle][0][1] >= plate_x) ||
		(x + tetromio[block_type][angle][1][1] < 0 || x + tetromio[block_type][angle][1][1] >= plate_x) ||
		(x + tetromio[block_type][angle][2][1] < 0 || x + tetromio[block_type][angle][2][1] >= plate_x) ||
		(x + tetromio[block_type][angle][3][1] < 0 || x + tetromio[block_type][angle][3][1] >= plate_x))
		return true;
	else
		return false;
}

bool board::down_collision_check(int y_move, int x_move, int angle_move)
{
	int block_type = current_block->get_block_type();
	int angle = (current_block->get_angle() + angle_move) % 4;
	int x = current_block->get_x() + x_move;
	int y = current_block->get_y() + y_move;

	if (y + tetromio[block_type][angle][0][0] >= plate_y ||
		y + tetromio[block_type][angle][1][0] >= plate_y ||
		y + tetromio[block_type][angle][2][0] >= plate_y ||
		y + tetromio[block_type][angle][3][0] >= plate_y)
		return true;
	else
		return false;
}

bool board::block_collision_check(int y_move, int x_move, int angle_move)
{
	int block_type = current_block->get_block_type();
	int angle = (current_block->get_angle() + angle_move) % 4;
	int x = current_block->get_x() + x_move;
	int y = current_block->get_y() + y_move;
	
	if ((plate[y + tetromio[block_type][angle][0][0]][x + tetromio[block_type][angle][0][1]] > 0 &&
		plate[y + tetromio[block_type][angle][0][0]][x + tetromio[block_type][angle][0][1]] < 8) ||
		(plate[y + tetromio[block_type][angle][1][0]][x + tetromio[block_type][angle][1][1]] > 0 &&
		plate[y + tetromio[block_type][angle][1][0]][x + tetromio[block_type][angle][1][1]] < 8) ||
		(plate[y + tetromio[block_type][angle][2][0]][x + tetromio[block_type][angle][2][1]] > 0 &&
		plate[y + tetromio[block_type][angle][2][0]][x + tetromio[block_type][angle][2][1]] < 8) ||
		(plate[y + tetromio[block_type][angle][3][0]][x + tetromio[block_type][angle][3][1]] > 0 &&
		plate[y + tetromio[block_type][angle][3][0]][x + tetromio[block_type][angle][3][1]] < 8))
		return true;
	else
		return false;
}

bool board::gameover_check(int y_move, int x_move, int angle_move)
{
	int x = current_block->get_x();
	int y = current_block->get_y();
	if (upper_collision_check(y_move, x_move, angle_move))
		return true;
	else if ((x == 9 && y == -1) && block_collision_check(x_move, y_move, angle_move))
		return true;

	return false;
}

void board::update(bool is_block_move_stop)
{
	int original_block_type;
	int block_type = current_block->get_block_type();
	int angle = current_block->get_angle();
	int x = current_block->get_x();
	int y = current_block->get_y();
	int plate_x = this->plate_x;
	int plate_y = this->plate_y;

	original_block_type = block_type;
	block_type = (is_block_move_stop ? block_type : block_type + 7);

	for (int j = 0; j < plate_y; ++j)
	{
		for (int i = 0; i < plate_x; ++i)
		{
			if (plate[j][i] > 7)
				plate[j][i] = 0;
		}
	}

	plate[y + tetromio[original_block_type][angle][0][0]][x + tetromio[original_block_type][angle][0][1]] = block_type;
	plate[y + tetromio[original_block_type][angle][1][0]][x + tetromio[original_block_type][angle][1][1]] = block_type;
	plate[y + tetromio[original_block_type][angle][2][0]][x + tetromio[original_block_type][angle][2][1]] = block_type;
	plate[y + tetromio[original_block_type][angle][3][0]][x + tetromio[original_block_type][angle][3][1]] = block_type;
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
			if (plate[y][x] > 0 && plate[y][x] < 8)
				cnt++;
		}

		if (cnt == plate_x)
			line_cnt++;
	}

	if (line_cnt > 0)
	{
		for (int y = plate_y - 1; y >= plate_y - 1 - line_cnt; --y)
		{
			for (int x = 0; x < plate_x; ++x)
			{
				plate[y][x] = plate[y - line_cnt][x];
				plate[y - line_cnt][x] = 0;
			}
		}

		deleted_line_count += line_cnt;
	}
}

int board::get_collision_upper_y_move()
{
	int block_type = current_block->get_block_type();
	int angle = current_block->get_angle();
	int x = current_block->get_x();
	int y = current_block->get_y();
	int plate_y = this->plate_y;
	int y_move;

	for (y_move = 0; y_move < plate_y; ++y_move)
	{
		if (down_collision_check(y_move, 0, 0) || block_collision_check(y_move, 0, 0))
			break;
	}
	y_move -= 1;

	return y_move;
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

int board::get_deleted_line_count()
{
	return deleted_line_count;
}