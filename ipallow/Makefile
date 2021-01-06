CC=gcc3
MODCFLAGS := -O2 -Wall -DMODULE -D__KERNEL__ 
EXTRA_MODCFLAGS := -Wstrict-prototypes -Wno-trigraphs -fomit-frame-pointer -fno-strict-aliasing -fno-common -pipe -mpreferred-stack-boundary=2 -march=i686
INCLUDEFLAGS := -I/usr/src/linux/include
ipallow:	ipallow.c 
		$(CC) $(INCLUDEFLAGS) $(MODCFLAGS) $(EXTRA_MODCFLAGS) -c ipallow.c
		
                                     
