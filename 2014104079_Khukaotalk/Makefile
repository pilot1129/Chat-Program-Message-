CC = gcc
CFLAGS =
LDFLAGS = -lpthread

.SUFFIXES : .c .o
.c .o :
	$(CC) -c $(CFLAGS) $<

ALL = server client

all : $(ALL)

server : server.o serv_func.h
	$(CC) -o $@ $< $(LDFLAGS)

client : client.o UI.o client_func.h
	$(CC) -o $@ client.o UI.o $(LDFLAGS)

clean : 
	rm -rf *.o $(ALL)
