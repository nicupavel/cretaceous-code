#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef HAVE_ICONV
#include <iconv.h>
#endif

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#define DEFAULT_PTSIZE	11
#define DEFAULT_FONT "data/fonts/verdana.ttf"
#define DEFAULT_TEXT	"test test test"
#define NUM_COLORS      256

TTF_Font *font;
SDL_Color white = { 0xFF, 0xFF, 0xFF, 0 };
SDL_Color black = { 0x00, 0x00, 0x00, 0 };
SDL_Color *forecol, *backcol;

int
font_init_ttf()
{

	if (TTF_Init() < 0) {
		fprintf(stderr, "Couldn't initialize TTF: %s\n",
			SDL_GetError());
		return 1;
	}

	atexit(TTF_Quit);

	font = TTF_OpenFont(DEFAULT_FONT, DEFAULT_PTSIZE);

	if (font == NULL) {
		fprintf(stderr, "Couldn't load %d pt font from %s: %s\n",
			DEFAULT_PTSIZE, DEFAULT_FONT, SDL_GetError());
		return(1);
	}
	
	TTF_SetFontStyle(font, TTF_STYLE_NORMAL);
	return(0);
}

int
font_write_string(SDL_Surface * screen, char * string, int x, int y)
{
	SDL_Rect dest;
	SDL_Surface *text;

	forecol = &white;
	
	text = TTF_RenderText_Blended(font, string, *forecol);
	if (text != NULL) {
		dest.x = x;
		dest.y = y;
		dest.w = text->w;
		dest.h = text->h;
		SDL_BlitSurface(text, NULL, screen, &dest);
		SDL_FreeSurface(text);
	} else {
		fprintf(stderr, "Couldn't render text: %s\n", SDL_GetError());
		return(1);
	}
	return(0);
}

int
font_close_ttf()
{
	TTF_CloseFont(font);
	return(0);
}
