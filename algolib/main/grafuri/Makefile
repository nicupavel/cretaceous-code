PA_PROJECT_ROOT = ../..
LIBDRS =
INCDRS = -I$(PA_PROJECT_ROOT)/include
LIBFLS =
CC     = gcc
LL     = gcc
CFLAGS =
LFLAGS =

SRCFLSG1 = graf-general.c\
         $(PA_PROJECT_ROOT)/source/graph.c\
         $(PA_PROJECT_ROOT)/source/list.c\
         $(PA_PROJECT_ROOT)/source/set.c
OBJFLSG1 = graf-general.o\
         $(PA_PROJECT_ROOT)/source/graph.o\
         $(PA_PROJECT_ROOT)/source/list.o\
         $(PA_PROJECT_ROOT)/source/set.o
EXEG1    = graf-general

SRCFLSG2 = graf-parcurgere.c\
	 $(PA_PROJECT_ROOT)/source/bfs.c\
	 $(PA_PROJECT_ROOT)/source/dfs.c\
	 $(PA_PROJECT_ROOT)/source/graph.c\
	 $(PA_PROJECT_ROOT)/source/list.c\
	 $(PA_PROJECT_ROOT)/source/queue.c\
	 $(PA_PROJECT_ROOT)/source/set.c
	 
OBJFLSG2 = graf-parcurgere.o\
	 $(PA_PROJECT_ROOT)/source/bfs.o\
         $(PA_PROJECT_ROOT)/source/dfs.o\
         $(PA_PROJECT_ROOT)/source/graph.o\
         $(PA_PROJECT_ROOT)/source/list.o\
	 $(PA_PROJECT_ROOT)/source/queue.o\
         $(PA_PROJECT_ROOT)/source/set.o
EXEG2    = graf-parcurgere

all: graf-general graf-parcurgere
     
graf-general: $(EXEG1)

graf-parcurgere: $(EXEG2)

$(EXEG1): $(OBJFLSG1)
	$(LL) $(LFLAGS) -o $@ $(OBJFLSG1) $(LIBDRS) $(LIBFLS)

$(EXEG2): $(OBJFLSG2)
	$(LL) $(LFLAGS) -o $@ $(OBJFLSG2) $(LIBDRS) $(LIBFLS)

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $(INCDRS) $<


clean:
	-rm $(EXEG1)
	-rm $(EXEG2)
	-rm $(OBJFLSG1)
	-rm $(OBJFLSG2)

