CC=gcc
CFLAGS=-Wall -g
TARGETS=main
FILES=main.c

main.o:
	$(CC) $< $(CFLAGS) -o $(TARGETS) $(FILES) -lm -lpthread

clean:
	rm $(TARGETS)
