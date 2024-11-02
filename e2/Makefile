
CC = gcc
FLAGS = -std=c99 -Wall -pedantic -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_SVID_SOURCE -D_POSIX_C_SOURCE=200809L -g

all: generator supervisor

generator: generator.o
	$(CC) $(FLAGS) -o $@ $^

supervisor: supervisor.o
	$(CC) $(FLAGS) -o $@ $^

%.o: %.c %.h
	$(CC) $(FLAGS) -c -o $@ $<

clean: 
	rm -f *.o generator supervisor