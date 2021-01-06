#Defs
CC=gcc
BIN=/bin
#CFLAGS= -Wstrict-prototypes -O2 -fomit-frame-pointer -pipe -march=i686 -fno-strict-aliasing -fno-strength-reduce
#CFLAGS= -O4 -D NDEBUG -fomit-frame-pointer -pipe -march=i686 -fno-strict-aliasing -fno-strength-reduce
CFLAGS= -O2
DEBUG= -g1
#Makefile
all: sensors
install:
	mv sensors $(BIN) || echo "no sensors binary found"
sensors: sensors.o
	$(CC) $(CFLAGS)  sensors.o -o sensors
sensors.o: sensors.c
	$(CC) $(CFLAGS) -c sensors.c
clean:
	/bin/rm -rfv *.o sensors


