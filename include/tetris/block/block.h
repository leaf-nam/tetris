#pragma once

/*
* block_type == 1
* **
*  **
*/

class block
{
private:
	int block_type;
	int x;
	int y;
	int angle;
public:
	block();
	void random_set_block_type();
	void y_move();
	void y_move(int move);
	void rotate(int angle_num);
	void x_move(int move_num);
	int get_x();
	int get_y();
	int get_angle();
	int get_block_type();
};