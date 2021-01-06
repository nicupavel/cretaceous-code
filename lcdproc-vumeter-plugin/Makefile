include ../../Makefile.config

########################################################################
# You shouldn't need to touch anything below here.
########################################################################

TARGET = lcdstats
OBJ = main.o mode.o chrono.o cpu.o disk.o mem.o vu.o sensors.o
#OBJ = main.o chrono.o mode.o batt.o  cpu.o disk.o load.o mem.o
#OBJ = main.o cpu.o mem.o
LIB += -L../../shared -lLCDstuff


###################################################################
# Compilation...
#

all: $(TARGET)

$(TARGET): $(OBJ) Makefile
	$(GCC) -s $(MISC) -o $(TARGET) $(OBJ) $(LIB)

%.o: %.c %.h Makefile
	$(GCC) -c $(MISC) $<


##################################################################
# Installation
#
install: $(TARGET)
	@echo ... lcdproc main client ...
	install -m 0755 -o root -g root $(TARGET) /usr/local/bin/


##################################################################
# Other stuff...
#
clean:
	rm -f $(OBJ) $(TARGET) *~ core

edit:
	emacs . &

