CC=gcc
CFLAGS=-Wall -g -I/usr/include/gnome-xml -DDEBUG 
LDFLAGS=-L/usr/lib/mysql -L/usr/lib -lxml -lmysqlclient -lpthread 
SOURCES= config.c list.c sql.c tcp.c sighandler.c security.c client.c util.c
OBJ=config.o list.o sql.o tcp.o sighandler.o security.o client.o util.o
all:
	$(CC) $(CFLAGS) -c $(SOURCES)
	$(CC) $(CFLAGS) $(LDFLAGS) -o cserver cserver.c $(OBJ)
clean:	
	rm -fv $(OBJ) ./cserver ./core*
