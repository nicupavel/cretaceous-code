#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "sdl.h"

SDL_Surface *screen, *terrain, *trees, *buildings, *interface;

//shouldn't be global
unsigned slocx = 0;
unsigned slocy = 0;

static int
make_level(unsigned i, char *level)
{
	snprintf(level, 64, "%s%d%s", "data/level", i, ".dat");
	return 0;
}

int
sdl_init_graphics()
{
	const SDL_VideoInfo *info;
	Uint32 flags = SDL_HWSURFACE | SDL_HWACCEL | SDL_HWPALETTE | SDL_DOUBLEBUF;
	//Uint32 flags = SDL_FULLSCREEN | SDL_HWSURFACE | SDL_DOUBLEBUF;

#ifdef HAVE_OPENGL
	flags |= SDL_GLSDL;
#endif

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		return 1;

	atexit(SDL_Quit);

	screen = SDL_SetVideoMode(RESX, RESY, DEPTH, flags);

	if (screen == NULL)
		return 1;

	SDL_WM_SetCaption(APPNAME, APPNAME);

	info = SDL_GetVideoInfo();

	if (info == NULL)
		return 1;

	printf("Using hardware surfaces: %s.\n",
	       (info->hw_available ? "Yes" : "No"));
	printf("Screen using hardware surfaces: %s.\n",
	       (screen->flags & SDL_HWSURFACE ? "Yes" : "No"));
	printf("Screen using double buffering: %s.\n",
	       (screen->flags & SDL_DOUBLEBUF ? "Yes" : "No"));
	printf("Hardware to hardware blits accelerated:%s.\n",
	       (info->blit_hw ? "Yes" : "No"));
	printf("Hardware to hardware colorkey blits accelerated: %s.\n",
	       (info->blit_hw_CC ? "Yes" : "No"));
	printf("Hardware to hardware alphakey blits accelerated: %s.\n",
	       (info->blit_hw_A ? "Yes" : "No"));
	printf("Software to hardware blits accelerated: %s.\n",
	       (info->blit_sw ? "Yes" : "No"));
	printf("Software to hardware colorkey blits accelerated: %s.\n",
	       (info->blit_sw_CC ? "Yes" : "No"));
	printf("Software to hardware alphakey blits accelerated: %s.\n",
	       (info->blit_sw_A ? "Yes" : "No"));
	printf("Color fills accelerated: %s.\n",
	       (info->blit_fill ? "Yes" : "No"));

	return 0;
}

int
sdl_init_data()
{
	STATINFO("loading terrain...");
	sdl_load_graphic(&terrain, TERRAIN, 1);
	STATINFO("loading trees...");
	sdl_load_graphic(&trees, TREES, 1);
	STATINFO("loading buildings...");
	sdl_load_graphic(&buildings, BUILDINGS, 1);
	STATINFO("loading interface...");
	sdl_load_graphic(&interface, INTERFACE, 0);

	return 0;
}

int
sdl_close_graphics()
{
	SDL_FreeSurface(terrain);
	SDL_FreeSurface(trees);
	SDL_FreeSurface(buildings);
	SDL_FreeSurface(interface);
	SDL_FreeSurface(screen);

	return 0;
}

int
sdl_game_loop()
{

	SDL_Event event;
	map_properties *map = NULL;
	unsigned i = 1;
	unsigned loadlevel = 1;
	unsigned shutdown = 0;
	unsigned showmapstats = 0;
	int start_time, end_time;
	int fps = 0;
	char *level;

	level = (char *) malloc(64 * sizeof (char));

	SDL_EnableKeyRepeat(150, 10);
	start_time = time(NULL);
	while (!shutdown) {
		while (SDL_PollEvent(&event) != 0) {
			SDL_keysym keysym;
			switch (event.type) {
			case SDL_KEYDOWN:
				keysym = event.key.keysym;

				if (keysym.sym == SDLK_q)
					shutdown = 1;
				if (keysym.sym == SDLK_n)
					if (i < 4) {
						i++;
						slocx = slocy = 0;
						loadlevel = 1;
					}
				if (keysym.sym == SDLK_p)
					if (i > 1) {
						i--;
						slocx = slocy = 0;
						loadlevel = 1;
					}
				if (keysym.sym == SDLK_RIGHT)
					if (slocx < map->width - MAX_SHOWX)
						slocx++;
				if (keysym.sym == SDLK_LEFT)
					if (slocx > 0)
						slocx--;
				if (keysym.sym == SDLK_UP)
					if (slocy > 0)
						slocy--;
				if (keysym.sym == SDLK_DOWN)
					if (slocy < map->height - MAX_SHOWY)
						slocy++;
				if (keysym.sym == SDLK_f)
					SDL_WM_ToggleFullScreen(screen);
				if (keysym.sym == SDLK_s)
					showmapstats = 1;
				break;

			case SDL_QUIT:
				shutdown = 1;
			}
		}
		if (loadlevel) {
			STATINFO("loading map ...");
			loadlevel = 0;
			make_level(i, level);
			if (map != NULL)
				map_free_map(map);
			map = map_load_map(level);
		}
		
		sdl_render_map(map, slocx, slocy);
		sdl_render_interface(map);
		SDL_Flip(screen);
		fps++;
		if (showmapstats) {
			map_get_statistics(map);
			showmapstats = 0;
		}
	}

	end_time = time(NULL);
	if (end_time == start_time)
		end_time++;

	printf("Drew %i frames in %i seconds, for a framerate of %.2f fps.\n",
	       fps, end_time - start_time,
	       (float) fps / (float) (end_time - start_time));

	free(level);
	return 0;
}

static int
sdl_draw_tile(SDL_Surface * tiles, int tile, int x, int y, int opacity)
{
	SDL_Rect src, dest;

	src.x = (tile % (tiles->w / TILE_W)) * TILE_W;
	src.y = (tile / (tiles->w / TILE_W)) * TILE_H;
	src.w = TILE_W;
	src.h = TILE_H;

	dest.x = x;
	dest.y = y;
	dest.w = TILE_W;
	dest.h = TILE_H;

	if (opacity > 0)
		SDL_SetAlpha(tiles, SDL_SRCALPHA, opacity);

	SDL_BlitSurface(tiles, &src, screen, &dest);
	return 0;
}

static int
sdl_load_graphic(SDL_Surface ** surface, char *filename, int alpha)
{
	SDL_Surface *temp = NULL;
	temp = IMG_Load(filename);
	if (temp == NULL) {
		printf("Error loading %s\n", filename);
		return 1;
	}

	if (alpha)
		*surface = SDL_DisplayFormatAlpha(temp);
	else
		*surface = SDL_DisplayFormat(temp);

	SDL_FreeSurface(temp);

	if (surface == NULL) {
		printf("Error converting to display format\n");
		return 1;
	}

	return 0;
}

int
sdl_render_map(map_properties * map, unsigned slocx, unsigned slocy)
{
	location_properties *location;
	unsigned i, j, x, y;
	unsigned stile, s;

	for (i = slocy, y = 0; i < map->height && i < slocy + MAX_SHOWY;
	     i++, y++)
		for (j = slocx, x = 0; j < map->width && j < slocx + MAX_SHOWX;
		     j++, x++) {
			location = map->map[i][j];
			sdl_draw_tile(terrain, location->terrain, x * TILE_W,
				      y * TILE_H, 0);
			if (location->structure != -1) {
				s = location->structure;
				stile = 0;
				while (s--)
					stile +=
					    sdl_structure_properties[s].width;
				sdl_draw_tile(buildings,
					      stile + location->offsetx +
					      ((buildings->w *
						location->offsety) / TILE_W),
					      x * TILE_W, y * TILE_H, 0);
				continue;
			}

		}
	
	return 0;
}

int
sdl_render_interface(map_properties * map)
{
	int i, x, y;
	char *info;
	SDL_Rect src, dest;
	SDL_Surface *minimap, *temp;
	location_properties *location;
	Uint32 pixel;

	for (i = 0; i < RESX - INTERFACEX; i += TILE_W) {
		src.x = 0;
		src.y = 0;
		src.w = TILE_W;
		src.h = 16;

		dest.x = i;
		dest.y = 0;
		dest.w = TILE_W;
		dest.h = 16;

		SDL_SetAlpha(interface, SDL_SRCALPHA, ALPHA);
		SDL_BlitSurface(interface, &src, screen, &dest);
	}

	for (i = 0; i < RESY; i += TILE_H) {
		src.x = 0;
		src.y = 0;
		src.w = INTERFACEX;
		src.h = TILE_H;

		dest.x = RESX - INTERFACEX;
		dest.y = i;
		dest.w = INTERFACEX;
		dest.h = TILE_H;

		SDL_SetAlpha(interface, SDL_SRCALPHA, ALPHA);
		SDL_BlitSurface(interface, &src, screen, &dest);
	}

	
	info = (char *) malloc(1024 * sizeof (char));
	snprintf(info, 1024, "Level: %d Description: %s Size: %dx%d",
		 map->level, map->description, map->width, map->height);
	font_write_string(screen, info, 0, 0);
	free(info);

	temp =
	    SDL_CreateRGBSurface(SDL_HWSURFACE, map->width, map->height, DEPTH,
				 rmask, gmask, bmask, amask);

	SDL_LockSurface(temp);
	for (y = 0; y < map->height; y += 2)
		for (x = 0; x < map->width; x += 2) {
			location = map->map[y][x];
			pixel = (location->terrain << 6) ^ 0x22;
			sdl_put_pixel(temp, x, y, pixel);
			sdl_put_pixel(temp, x, y + 1, pixel);
			sdl_put_pixel(temp, x + 1, y, pixel);
			sdl_put_pixel(temp, x + 1, y + 1, pixel);
		}
	SDL_UnlockSurface(temp);
	minimap = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
	/* minimap blackbox container */
	dest.x = RESX - INTERFACEX;
	dest.y = TILE_H * 2;
	dest.w = INTERFACEX;
	dest.h = INTERFACEX;
	SDL_FillRect(screen, &dest, SDL_MapRGB(screen->format, 0, 0, 0));
	/* The minimap */
	dest.x = RESX - INTERFACEX + (INTERFACEX - map->width) / 2;
	dest.y = TILE_H * 2 + (INTERFACEX - map->height) / 2;
	dest.w = map->width;
	dest.h = map->height;
	SDL_BlitSurface(minimap, NULL, screen, &dest);
	/* curent view location on minimap */
	src.x = 0;
	src.y = 0;
	src.w = MAX_SHOWX;
	src.h = MAX_SHOWY;

	dest.x = RESX - INTERFACEX + (INTERFACEX - map->width) / 2 + slocx;
	dest.y = TILE_H * 2 + (INTERFACEX - map->height) / 2 + slocy;
	dest.w = MAX_SHOWX;
	dest.h = MAX_SHOWY;

	SDL_SetAlpha(interface, SDL_SRCALPHA, ALPHA);
	SDL_BlitSurface(interface, &src, screen, &dest);

	SDL_FreeSurface(minimap);

	return 0;
}

static Uint32
sdl_get_pixel(SDL_Surface * surface, int x, int y)
{
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *) surface->pixels + y * surface->pitch + x * bpp;

	switch (bpp) {
	case 1:
		return *p;
	case 2:
		return *(Uint16 *) p;
	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return p[0] << 16 | p[1] << 8 | p[2];
		else
			return p[0] | p[1] << 8 | p[2] << 16;
	case 4:
		return *(Uint32 *) p;
	default:
		return 0;

	}
}

static void
sdl_put_pixel(SDL_Surface * surface, int x, int y, Uint32 pixel)
{
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *) surface->pixels + y * surface->pitch + x * bpp;

	switch (bpp) {
	case 1:
		*p = pixel;
		break;
	case 2:
		*(Uint16 *) p = pixel;
		break;
	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
			p[0] = (pixel >> 16) & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = pixel & 0xff;
		} else {
			p[0] = pixel & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = (pixel >> 16) & 0xff;
		}
		break;
	case 4:
		*(Uint32 *) p = pixel;
		break;
	}
}
