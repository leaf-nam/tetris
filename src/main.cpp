#include "input.h"
#include "render.h"

int main()
{
	input i(block::get_instance(), board::get_instance(), 0);
	render r(0, 0, board::get_instance());

	bool is_gameover = false;
	while (!is_gameover)
	{
		i.get_user_input();
		i.activate_block();
		r.print_plate();
	}

	return 0;
}