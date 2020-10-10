CC=gcc
CFLAGS=-O2 -Wall
TARGETS=main.o

main.o: seso/main.c
	$(CC) $< $(CFLAGS) -o $@

clean:
	rm $(TARGETS)

