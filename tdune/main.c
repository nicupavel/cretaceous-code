#include <stdio.h>

int
main()
{
	initrandom();
	map_generate_levels();
	sdl_init_graphics();
	font_init_ttf();
	sdl_init_data();
	sdl_game_loop();
	sdl_close_graphics();
	font_close_ttf();
	return(0);
}
