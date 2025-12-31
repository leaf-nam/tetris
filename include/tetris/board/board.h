#pragma once
#include <block/block.h>
#include <array>

using namespace std;

class board
{
private:
	int plate_y;
	int plate_x;
	int plate[20][20];
	array<array<array<array<int, 2>, 4>, 4>, 8> tetromio;
	int deleted_line_count;
	block* current_block;
	board();
public:
	static board& get_instance();
	board(const board&) = delete;
	board& operator=(const board&) = delete;
	void init_tetromino();
	void set_current_block(block* current_block);
	void update(bool is_block_move_stop);
	bool upper_collision_check(int y_move, int x_move, int angle_move);
	bool left_right_collision_check(int y_move, int x_move, int angle_move);
	bool down_collision_check(int y_move, int x_move, int angle_move);
	bool block_collision_check(int y_move, int x_move, int angle_move);
	bool gameover_check(int x_move, int y_move, int angle_move);
	void line_delete();
	int get_collision_upper_y_move();
	int get_plate_x();
	int get_plate_y();
	int get_plate_num(int y, int x);
	int get_deleted_line_count();
};