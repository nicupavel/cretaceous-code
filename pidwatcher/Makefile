CC=gcc
MODCFLAGS := -O2 -Wall -DMODULE -D__KERNEL__ 
EXTRA_MODCFLAGS := -Wstrict-prototypes -Wno-trigraphs -fomit-frame-pointer -fno-strict-aliasing -fno-common -pipe -mpreferred-stack-boundary=2 -O3
#EXTRA_MODCFLAGS = -DDEBUG -DALL
INCLUDEFLAGS := -I/usr/src/linux/include
pidwatcher.o:	pidwatcher.c 
		$(CC) $(INCLUDEFLAGS) $(MODCFLAGS) -c pidwatcher.c
		
pspidwatcher:		pspidwatcher.c
		$(CC) $(EXTRA_MODCFLAGS) pspidwatcher.c -o pspidwatcher
clean:		
	rm -rvf pidwatcher.o
install:
	install -d /lib/modules/pidwatcher
	install -d /dev/pidwatcher
	rm -rf /dev/pidwatcher/pidwatcher
	install pidwatcher.o /lib/modules/pidwatcher/
	mknod  /dev/pidwatcher/pidwatcher c 254 128

