
PA_PROJECT_ROOT = ../..

LIBDRS =
INCDRS = -I$(PA_PROJECT_ROOT)/include
LIBFLS = -lm
SRCFLS = sortari-general.c\
         $(PA_PROJECT_ROOT)/source/issort.c\
         $(PA_PROJECT_ROOT)/source/mgsort.c\
         $(PA_PROJECT_ROOT)/source/qksort.c\
         $(PA_PROJECT_ROOT)/source/ctsort.c\
         $(PA_PROJECT_ROOT)/source/rxsort.c

OBJFLS = sortari-general.o\
         $(PA_PROJECT_ROOT)/source/issort.o\
         $(PA_PROJECT_ROOT)/source/mgsort.o\
         $(PA_PROJECT_ROOT)/source/qksort.o\
         $(PA_PROJECT_ROOT)/source/ctsort.o\
         $(PA_PROJECT_ROOT)/source/rxsort.o

EXE    = sortari-general

CC     = gcc
LL     = gcc
CFLAGS =
LFLAGS =

all: $(EXE)

$(EXE): $(OBJFLS)
	$(LL) $(LFLAGS) -o $@ $(OBJFLS) $(LIBDRS) $(LIBFLS)

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $(INCDRS) $<

clean:
	-rm $(EXE)
	-rm $(OBJFLS)

