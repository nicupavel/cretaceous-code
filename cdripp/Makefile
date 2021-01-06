CC=gcc
LD=gcc
CFLAGS=-Wall -O2

cdripp: cdripp.o
	$(LD) -o cdripp cdripp.o
cdripp.o: cdripp.c
	$(CC) $(CFLAGS) -c cdripp.c
all:
	cdripp