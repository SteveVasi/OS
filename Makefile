.PHONY: all clean

CC = gcc
FLAGS = -std=c99 -Wall -pedantic -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_SVID_SOURCE -D_POSIX_C_SOURCE=200809L -g


main: main.o list.o
        $(CC) $(FLAGS) -o $@ $^

%.o: %.c %.h
        $(CC) $(FLAGS) -c -o $@ $<

clean:
        rm -f *.o main
