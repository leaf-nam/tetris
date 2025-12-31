#include <block/block.h>

block::block() :block_type(0), x(9), y(-1), angle(0)
{
	random_set_block_type();
}

void block::random_set_block_type()
{
	this->block_type = 1;
}

void block::y_move()
{
	y++;
}

void block::rotate(int angle_num)
{
	this->angle = (this->angle + angle_num) % 360;
}

void block::x_move(int move_num)
{
	x += move_num;
}

int block::get_x()
{
	return x;
}

int block::get_y()
{
	return y;
}

int block::get_block_type()
{
	return block_type;
}

int block::get_angle()
{
	return angle;
}