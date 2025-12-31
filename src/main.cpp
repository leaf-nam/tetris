#include <input/input.h>
#include <render/render.h>
#include <iostream>

int main()
{
	input& i = input::get_instance();
	render& r = render::get_instance(5, 40);
	block* new_block = new block();

	i.set_current_block(new_block);
	board::get_instance().set_current_block(new_block);

	bool is_gameover = false;
	while (!is_gameover)
	{
		i.get_user_input();
		is_gameover = i.activate_block();
		r.print_plate();
		r.sleep(250);
	}

	return 0;
}