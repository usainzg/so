CC=gcc
CFLAGS=-O2 -Wall
TARGETS=main.o

main.o:
	$(CC) $< $(CFLAGS) -o $@ seso/main.c include/queue.c seso/workers/process_gen.c -lpthread

clean:
	rm $(TARGETS)