CC = gcc
CFLAGS       = -fPIC 
LDFLAGS      = -shared
TARGET  = highjack.so
SOURCES = libc-highjack.c
OBJECTS = $(SOURCES:.c=.o)

PREFIX = $(DESTDIR)/usr/local
BINDIR = $(PREFIX)/bin

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(DEBUGFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJECTS)