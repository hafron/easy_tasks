CC=gcc
CFLAGS=-Wall -ansi -pedantic
CLINK=-lm -lsqlite3

all: et

et: et.o sqlite.o
	$(CC) -o et et.o sqlite.o $(CLINK)
et.o: et.c
	$(CC) -c  $(CFLAGS) et.c
sqlite.o: sqlite.c
	$(CC) -c $(CFLAGS) sqlite.c
clean:
	rm *.o et
install: et
	cp et /usr/bin/