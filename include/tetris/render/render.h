#pragma once
#include <board/board.h>
#include <windows.h>

class render
{
private:
	int start_y;
	int start_x;
	WORD default_console_color;
	render(int start_y, int start_x);
public:
	static render& get_instance(int start_y, int start_x);
	render(const render&) = delete;
	render& operator=(const render&) = delete;
	void prompt(int x, int y);
	void print_plate();
	void sleep(int seconds);
	void set_color(WORD color);
	void print_color_by_number(int num);
};