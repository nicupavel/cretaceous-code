#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef HAVE_ICONV
#include <iconv.h>
#endif

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#define DEFAULT_PTSIZE	10
#define DEFAULT_FONT "data/fonts/sans.ttf"
#define DEFAULT_TEXT	"The quick brown fox jumped over the lazy dog"
#define NUM_COLORS      256

int font_init_ttf();
int font_write_string(SDL_Surface *screen, char * string, int x, int y);
int font_close_ttf();

