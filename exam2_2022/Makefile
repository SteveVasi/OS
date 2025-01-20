
CC = gcc
CFLAGS = -std=c99 -Wall -pedantic -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_SVID_SOURCE -D_POSIX_C_SOURCE=200809L -g -I -pthread
LDFLAGS = -pthread -lrt

all: client server

client: client.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

server: server.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean: 
	rm -f *.o client server