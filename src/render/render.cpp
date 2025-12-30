#include "render.h"
#include <iostream>
#include <window.h>
using namespace std;

render::render(int start_x, int start_y, board current_board) :start_x(start_x), start_y(start_y), current_board(current_board) {}

void render::prompt(int x, int y)
{
	COORD locate = { x - 1, y - 1 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), locate);
}

void render::print_plate()
{
	int plate_x = current_board.get_plate_x();
	int plate_y = current_board.get_plate_y();

	system("cls");

	for (int y = 0; y < plate_y; ++y)
	{
		prompt(start_x, start_y + y);
		for (int x = 0; x < plate_x; ++x)
		{
			cout << current_board.get_plate_num(y, x);
		}
		cout << '\n';
	}
}