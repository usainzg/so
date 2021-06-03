CC=gcc
CFLAGS=-Wall
TARGETS=main.o
FILES=main.c

main.o:
	$(CC) $< $(CFLAGS) -o $@ $(FILES) -lm -lpthread

clean:
	rm $(TARGETS)
