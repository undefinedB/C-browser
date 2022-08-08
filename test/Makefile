CC=gcc
CFLAGS=-I -Wall
DEPS = program.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

program: program.o functii.o
	$(CC) -o program program.o functii.o
