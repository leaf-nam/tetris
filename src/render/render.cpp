#include <render/render.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <windows.h>
using namespace std;

render::render(int start_y, int start_x) :start_y(start_y), start_x(start_x)
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(h_console, &info);
	default_console_color = info.wAttributes;
}

render& render::get_instance(int start_y, int start_x)
{
	static render instance(start_y, start_x);
	return instance;
}

void render::prompt(int y, int x)
{
	COORD locate = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), locate);
}

void render::print_plate()
{
	board& current_board = board::get_instance();
	int plate_x = current_board.get_plate_x();
	int plate_y = current_board.get_plate_y();

	for (int y = 0; y < plate_y; ++y)
	{
		prompt(start_y + y, start_x);
		cout << '|';
		for (int x = 0; x < plate_x; ++x)
		{
			print_color_by_number(current_board.get_plate_num(y, x));
		}
		cout << "|\n";
	}

	prompt(start_y + plate_y, start_x);
	for (int x = 0; x <= plate_x + 1; ++x)
	{
		cout << '-';
	}

	prompt(start_y + plate_y + 1, start_x);
	cout << "deleted line: " << current_board.get_deleted_line_count();
}

void render::sleep(int milliseconds)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

void render::set_color(WORD color)
{
	HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h_console, color);
}

void render::print_color_by_number(int num)
{
	
	switch (num)
	{
	case 0:
		cout << "бс";
		break;
	case 1:
	case 8:
		set_color(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		cout << "бс";
		break;
	case 2:
	case 9:
		set_color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		cout << "бс";
		break;
	case 3:
	case 10:
		set_color(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		cout << "бс";
		break;
	case 4:
	case 11:
		set_color(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		cout << "бс";
		break;
	case 5:
	case 12:
		set_color(FOREGROUND_RED | FOREGROUND_INTENSITY);
		cout << "бс";
		break;
	case 6:
	case 13:
		set_color(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		cout << "бс";
		break;
	case 7:
	case 14:
		set_color(FOREGROUND_RED | FOREGROUND_GREEN);
		cout << "бс";
		break;
	}
	set_color(default_console_color);
}