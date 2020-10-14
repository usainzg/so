CC=gcc
CFLAGS=-O2 -Wall
TARGETS=main.o

main.o:
	$(CC) $< $(CFLAGS) -o $@ seso/main.c include/*.c seso/workers/*.c -lpthread

clean:
	rm $(TARGETS)