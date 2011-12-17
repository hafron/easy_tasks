CC=gcc
CFLAGS=-Wall -ansi -pedantic
CLINK=-lm
easy_tasks: main.o sqlite.o
	$(CC) -o easy_task main.o sqlite.o $(CLINK)
main.o: main.c
	$(CC) -c  $(CFLAGS) main.c
sqlite.o: sqlite.c
	$(CC) -c  $(CFLAGS) sqlite.c

