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
	block();
	static block current_block;
public:
	static block get_instance();
	void random_set_block_type();
	void y_move();
	void rotate(int angle_num);
	void x_move(int move_num);
	int get_x();
	int get_y();
	int get_angle();
	int get_block_type();
	void init_block();
};