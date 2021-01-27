CC=gcc
CFLAGS=-O2 -Wall
TARGETS=main.o

main.o:
	$(CC) $< $(CFLAGS) -o $@ main.c -lpthread

clean:
	rm $(TARGETS)
