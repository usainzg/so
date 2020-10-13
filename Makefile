CC=gcc
CFLAGS=-O2 -Wall
TARGETS=main.o

main.o: seso/main.c queue.o process_gen.o
	$(CC) $< $(CFLAGS) -o $@

queue.o: include/queue.c
    gcc -c queue.c

process_gen.o: seso/workers/process_gen.c
    gcc -c process_gen.c

clean:
	rm $(TARGETS)

