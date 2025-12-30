#include "render.h"
#include <iostream>
using namespace std;

render::render(int start_x, int start_y, board current_board) :start_x(start_x), start_y(start_y), current_board(current_board) {}

void render::print_plate()
{
	int plate_x = current_board.get_plate_x();
	int plate_y = current_board.get_plate_y();

	for (int y = 0; y < plate_y; ++y)
	{
		for (int x = 0; x < plate_x; ++x)
		{
			cout << current_board.get_plate_num(y, x);
		}
		cout << endl;
	}
}