#pragma once
#include <block/block.h>

class board
{
private:
	int plate_y;
	int plate_x;
	int plate[20][20];
	int deleted_line_count;
	block current_block;
	board();
public:
	static board& get_instance();
	board(const board&) = delete;
	board& operator=(const board&) = delete;
	void set_current_block(block current_block);
	void update(bool is_block_move_stop);
	bool upper_collision_check(int y_move, int x_move, int angle_move);
	bool left_right_collision_check(int y_move, int x_move, int angle_move);
	bool down_collision_check(int y_move, int x_move, int angle_move);
	bool block_collision_check(int y_move, int x_move, int angle_move);
	bool gameover_check(int x_move, int y_move, int angle_move);
	void line_delete();
	int get_plate_x();
	int get_plate_y();
	int get_plate_num(int y, int x);
};