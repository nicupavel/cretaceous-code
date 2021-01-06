CC=gcc
CFLAGS= -O3 -Wall `sdl-config --cflags` -mcpu=pentium4 -march=pentium4
CFLAGS-GL=-DHAVE_OPENGL
CFLAGS-DEBUG=-pg -g1
LDFLAGS= `sdl-config --libs` -L/usr/lib -lSDL_image -lSDL_ttf
LDFLAGS-GL = -lGL -lGLU -lm
LDFLAGS-DEBUG=-pg
LDFLAGS-STATIC=--static
SRC=map.c rand.c sdl.c font.c
SRC-GL=glSDL.c
OBJ= map.o rand.o sdl.o font.o
OBJ-GL=glSDL.o
MAIN=main.c
PROJNAME=tdune-sdl
PROJNAME-GL=tdune-gl

all: opengl sdl

opengl:
	$(CC) $(CFLAGS) $(CFLAGS-GL) -c $(SRC) $(SRC-GL)
	$(CC) $(LDFLAGS) $(LDFLAGS-GL) -o $(PROJNAME-GL) $(MAIN) $(OBJ) $(OBJ-GL)

sdl:
	$(CC) $(CFLAGS) -c $(SRC)
	$(CC) $(LDFLAGS)  -o $(PROJNAME) $(MAIN) $(OBJ)
	

clean:
	rm -rv *.o $(PROJNAME) $(PROJNAME-GL)