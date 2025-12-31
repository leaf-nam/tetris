#include <render/render.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <windows.h>
using namespace std;

render::render(int start_y, int start_x) :start_y(start_y), start_x(start_x) {}

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

	system("cls");

	for (int y = 0; y < plate_y; ++y)
	{
		prompt(start_y + y, start_x);
		cout << '|';
		for (int x = 0; x < plate_x; ++x)
		{
			cout << current_board.get_plate_num(y, x);
		}
		cout << "|\n";
	}

	prompt(start_y + plate_y, start_x);
	for (int x = 0; x <= plate_x + 1; ++x)
	{
		cout << '-';
	}
}

void render::sleep(int seconds)
{
	std::this_thread::sleep_for(std::chrono::seconds(1));
}