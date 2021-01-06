BINDIR=../bin

CC=gcc

#CFLAGS= -g 
#Optimisation CFLAGS for FASTWEB Client
#CFLAGS= -O2 -m486 -pipe 
#CFLAGS= -O2 -m486 -fno-strength-reduce -Wall -pipe
#CFLAGS= -O2 -march=i686 -fno-strength-reduce -Wall -pipe
CFLAGS= -Wstrict-prototypes -O2 -fomit-frame-pointer -pipe -march=i686 -fno-strict-aliasing -fno-strength-reduce

LIBS=

OBJECTS=cfastweb.o connectsock.o connectTCP.o connectUDP.o errexit.o parseHTML.o
all: cfastweb
cfastweb: $(OBJECTS)
	$(CC) $(CFLAGS) ${EXTRA_OPTION} $(OBJECTS) -o cfastweb $(LIBS)
.c.o:
	$(CC) $(CFLAGS) ${EXTRA_OPTION} -c $<
cfastweb.o: cfastweb.c
connectsock.o: connectsock.c
connectTCP.o: connectTCP.c
connectUDP.o: connectUDP.c
errexit.o: errexit.c
parseHTML.o: parseHTML.c

install:
	install -c -s -m 0755 cfastweb $(BINDIR)
	rm -f $(OBJECTS)
	rm -f cfastweb
clean:
	rm -f $(OBJECTS)
	rm -f core 
