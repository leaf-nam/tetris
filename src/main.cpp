#include <input/input.h>
#include <render/render.h>
#include <iostream>

int main()
{
	input i;
	render r(5, 40);
	block new_block;

	i.set_current_block(new_block);
	board::get_instance().set_current_block(new_block);

	bool is_gameover = false;
	while (!is_gameover)
	{
		i.get_user_input();
		i.activate_block();
		r.print_plate();
	}

	return 0;
}