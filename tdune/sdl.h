
#ifdef HAVE_OPENGL
#include "glSDL.h"
#define APPNAME "TDune OpenGL"
#else
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#define APPNAME "TDune SDL"
#endif

#include "map.h"
#include "rand.h"
#include "font.h"

#include "debug.h"

#define RESX 1024
#define RESY 768
#define DEPTH 32

#define INTERFACEX 128
#define INTERFACEY 16

#define TILE_W 32
#define TILE_H 32

#define MAX_SHOWX RESX/TILE_W
#define MAX_SHOWY RESY/TILE_H

#define TERRAIN "data/graphics/terrain.dat"
#define TREES   "data/graphics/trees.dat"
#define INTERFACE   "data/graphics/interface.dat"
#define BUILDINGS "data/graphics/buildings.dat"


#define ALPHA 164

Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
//    rmask = 0x000000ff;
//    gmask = 0x0000ff00;
//    bmask = 0x00ff0000;
//    amask = 0xff000000;
#endif

struct _terain_properties sdl_terrain_properties[] = {
    { MOUNTAIN,  "Mountains",     10,    5,   -10,  1, 1 },
    { HILL,      "Hills",          5,    3,    -5,  1, 1 },
    { PLAIN,     "Plains",         0,    0,    15,  1, 1 },
    { SAND,      "Sand",           5,    5,    20,  1, 1 },
    { ROUGH,     "Rough Terrain",  5,    5,    10,  1, 1 },
    { ROAD,      NULL,             0,    0,    30,  1, 1 },
    { RIVER,     "River",         -5,  -10,   -20,  0, 1 },
    { LAKE,      "Lake",         -10,  -10,   -20,  0, 1 },
    { BUILDING,  NULL,             0,    0,     0,  0, 0 },
    { -1,        NULL,             0,    0,     0,  0, 0 } 
};

struct _structure_properties sdl_structure_properties[] = {
    { BARRACKS,        "Infantry Barracks",              4,   3,   0,   20,   1000,  1 },
    { FACTORY,         "Vehicle Factory",                5,   5,   0,   50,   1500,  1 },
    { BASE,            "Command Center",                 3,   3,   50,   0,   3000,  1 },
    { POWERPLANT,      "A Nuclear Power Plant",          3,   3,   100,  0,   1000,  0 },
    { RESOURCE_CENTER, "Advanced Resource Conversion",   3,   6,   0,   20,   1000,  1 },
    { -1,              NULL,                             0,   0,   0,    0,      0,  0 }
};

static int sdl_load_graphic(SDL_Surface **surface, char *filename, int alpha);
static int sdl_draw_tile(SDL_Surface *tiles, int tile, int x, int y, int opacity);
static int make_level(unsigned i, char *level);
static Uint32 sdl_get_pixel(SDL_Surface * surface, int x, int y);
static void sdl_put_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

int sdl_render_trees(SDL_Surface *screen, map_properties * map);
int sdl_render_map(map_properties * map, unsigned slocx, unsigned slocy);
int sdl_render_interface(map_properties * map);
int sdl_close_graphics(void);
