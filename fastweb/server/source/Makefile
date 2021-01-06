BINDIR=../bin

CC=egcs

#CFLAGS= -g 
#Optimisation CFLAGS for FASTWEB Client
#CFLAGS= -O2 -m486 -pipe 
#CFLAGS= -O2 -m486 -fno-strength-reduce -Wall -pipe
#CFLAGS= -O2 -march=i686 -fno-strength-reduce -Wall -pipe
CFLAGS= -Wstrict-prototypes -O2 -fomit-frame-pointer -pipe -march=i686 -fno-strict-aliasing -fno-strength-reduce

LIBS=

OBJECTS=fastwebd.o passivesock.o passiveTCP.o passiveUDP.o services.o errexit.o
all: fastwebd
fastwebd: $(OBJECTS)
	$(CC) $(CFLAGS) ${EXTRA_OPTION} $(OBJECTS) -o fastwebd $(LIBS)
.c.o:
	$(CC) $(CFLAGS) ${EXTRA_OPTION} -c $<
fastwebd.o: fastwebd.c
passivesock: passivesock.c
passiveTCP.o: passiveTCP.c
passiveUDP.o: passiveUDP.c
errexit.o: errexit.c
services.o: services.c

install:
	install -c -s -m 0755 fastwebd $(BINDIR)
	rm -f $(OBJECTS)
	rm -f fastwebd
clean:
	rm -f $(OBJECTS)
	rm -f core 
