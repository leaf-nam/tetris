#include "block.h"

block::block() :block_type(0), x(10), y(0), angle(0) {}

block block::get_instance()
{
	return current_block;
}

void block::set_block_type(int block_type)
{
	this->block_type = block_type;
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

block block::current_block();