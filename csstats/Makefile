CC=gcc
#CFLAGS=-DDEBUG
CFLAGS=-Wall -O2
SRC=udp.c
OBJ=udp.o
MAIN=csstats.c
NAME=csstats

all:
	$(CC) $(CFLAGS) -c $(SRC)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(MAIN)

clean:
	rm -vf $(OBJ) $(NAME) core*
